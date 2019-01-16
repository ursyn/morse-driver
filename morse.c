#include "morse.h"

struct buffer *buffers = NULL;

char* letters[] = {
    ".-..-", // spacja = 32
    "-----", // 0 = 48
    ".----", // 1 = 49
    "..---", // 2 = 50
    "...--", // 3 = 51
    "....-", // 4 = 52
    ".....", // 5 = 53
    "-....", // 6 = 54
    "--...", // 7 = 55
    "---..", // 8 = 56
    "----.", // 9 = 57
    ".-",    // A = 65, a = 97
    "-...",  // B = 66, b = 98
    "-.-.",  // C = 67, c = 99
    "-..",   // D = 68, d = 100
    ".",     // E = 69, e = 101
    "..-.",  // F = 70, f = 102
    "--.",   // G = 71, g = 103
    "....",  // H = 72, h = 104
    "..",    // I = 73, i = 105
    ".---",  // J = 74, j = 106
    "-.-",   // K = 75, k = 107
    ".-..",  // L = 76, l = 108
    "--",    // M = 77, m = 109
    "-.",    // N = 78, n = 110
    "---",   // O = 79, o = 111
    ".--.",  // P = 80, p = 112
    "--.-",  // Q = 81, q = 113
    ".-.",   // R = 82, r = 114
    "...",   // S = 83, s = 115
    "-",     // T = 84, t = 116
    "..-",   // U = 85, u = 117
    "...-",  // V = 86, v = 118
    ".--",   // W = 87, w = 119
    "-..-",  // X = 88, x = 120
    "-.--",  // Y = 89, y = 121
    "--.."   // Z = 90, z = 122
};

int spaceIndex = 0;
int numberIndex = 1;
int letterIndex = 11;

/*
  * Ilosc wyswietlonych sygnalow znaku n/5
 */
int displayedSignalsCount = 0;

/*
 * Zmiana koloru komorki konsoli
 */
static void on_timer_expire(unsigned long dn) {
    char *consoleCell = (char*)(vc_cons[fg_console].d->vc_origin + (2 * dn + 1));
    char signal;
    if(*consoleCell == 0) {
        if(buffers[dn].bufferCount > 0) {
            signal = buffers[dn].buffer[buffers[dn].start];
            buffers[dn].start++;
            buffers[dn].bufferCount--;
            displayedSignalsCount++;
            if(displayedSignalsCount >= 5) {
                displayedSignalsCount = 0;
                wake_up(&buffers[dn].writeQueue);
            }
            switch(signal) {
            case '.':
                *consoleCell = 16 * (dn + 1) + dn + 1;
                buffers[dn].timerDelay = buffers[dn].dotLength;
                break;
            case '-':
                *consoleCell = 16 * (dn + 1) + dn + 1;
                buffers[dn].timerDelay = buffers[dn].dashLength;
                break;
            case 'x':
                buffers[dn].timerDelay = buffers[dn].spaceLength;
                break;
            default:
                buffers[dn].timerDelay = buffers[dn].dotLength;
                break;
            }
        }
        buffers[dn].timer.expires = jiffies + buffers[dn].timerDelay;
        add_timer(&buffers[dn].timer);
    } else if(buffers[dn].bufferCount > 0) {
        *consoleCell = 0;
        buffers[dn].timerDelay = buffers[dn].dotLength;
        buffers[dn].timer.expires = jiffies + buffers[dn].timerDelay;
        add_timer(&buffers[dn].timer);
    } else {
        buffers[dn].timerDelay = buffers[dn].spaceLength;
        buffers[dn].timer.expires = jiffies + buffers[dn].timerDelay;
        add_timer(&buffers[dn].timer);
    }
}

int morse_open(struct inode *inode, struct file *file) {
    int dn = MINOR(inode->i_rdev);
    if(dn > 7) {
        printk("Sterownik obsluguje jedynie osiem urzadzen o roznych numerach podrzednych\n", dn);
        return -EIO;
    }
    down(&buffers[dn].semaphore);
    MOD_INC_USE_COUNT;
    buffers[dn].useCount++;
    if(buffers[dn].useCount == 1) {
        buffers[dn].buffer = kmalloc(buffers[dn].bufferSize, GFP_KERNEL);
        buffers[dn].bufferCount = buffers[dn].start = buffers[dn].end = 0;
    }
    up(&buffers[dn].semaphore);
    return 0;
}

void morse_release(struct inode *inode, struct file *file) {
    int dn = MINOR(inode->i_rdev);
    if (dn < 0) {
        printk("Sterownik obsluguje jedynie osiem urzadzen o roznych numerach podrzednych\n", dn);
    } else {
        MOD_DEC_USE_COUNT;
        buffers[dn].useCount--;
        if (buffers[dn].useCount == 0) {
            kfree(buffers[dn].buffer);
        }
    }
}

int morse_write(struct inode *inode, struct file *file, const char *pB, int count) {
    int dn = MINOR(inode->i_rdev);
    int i;
    int j;
    char letter;
    for(i = 0; i < count; i++) {
        letter = get_user(pB + i);
        while (buffers[dn].bufferSize - buffers[dn].bufferCount < 6) {
            interruptible_sleep_on(&buffers[dn].writeQueue);
            if (current->signal & ~current->blocked) {
                if (i==0) {
                    return -ERESTARTSYS;
				} else {
					return i;
				}
            }
        }
        if(letter >= 65 && letter <= 90) {
            letter -= (65 - letterIndex);
        } else if(letter >= 97 && letter <= 122) {
            letter -= (97 - letterIndex);
        } else if(letter >= 48 && letter <= 57) {
            letter -= (48 - numberIndex);
        } else if(letter == 32) {
            letter = spaceIndex;
        }
        for(j = 0; j < 6; j++) {
            if(letters[letter][j] == '\0') {
                buffers[dn].buffer[buffers[dn].end]='x';
            } else {
                buffers[dn].buffer[buffers[dn].end]=letters[letter][j];
            }
            buffers[dn].bufferCount++;
            buffers[dn].end++;
            if (buffers[dn].end == buffers[dn].bufferSize) {
                buffers[dn].end = 0;
            }
            if(letters[letter][j] == '\0') {
                break;
            }
        }

    }
    return count;
}

int morse_ioctl(struct inode *inode, struct file *file,
                unsigned int command, unsigned long data) {
    int dn = MINOR(inode->i_rdev);
    int error = 0;
    int temp;
    int len;
    int i;
    char *updatedBuffer;
    switch (command) {
    case IOCTL_SET_BUFFER_SIZE:
        temp=(int)data;
        if(temp < MIN_BUFFER_SIZE) {
            printk("Zbyt maly rozmiar bufora. Rozmiar: %d, minimalny: %d.\n", temp, MIN_BUFFER_SIZE);
            return -EINVAL;
        } else if(temp > MAX_BUFFER_SIZE) {
            printk("Zbyt duzy rozmiar bufora. Rozmiar: %d, maksymalny: %d.\n", temp, MAX_BUFFER_SIZE);
            return -EINVAL;
        } else {
            if((error = verify_area(VERIFY_WRITE, (void *)data, _IOC_SIZE(data))) < 0) {
                printk("Proba zapisu do niepoprawnego miejsca w pamieci. Blad: %d\n", error);
                return error;
            }
            if(buffers[dn].start > temp || buffers[dn].end > temp || buffers[dn].bufferCount > temp) {
                printk("Dane w pamieci do zwolnienia podczas zmniejszania bufora. Operacja niemozliwa.\n");
                return -ENOMEM;
            }
            down(&buffers[dn].semaphore);
            updatedBuffer = kmalloc(temp, GFP_KERNEL);
            for(i = 0; i < buffers[dn].bufferCount; i++) {
                updatedBuffer[i] = buffers[dn].buffer[buffers[dn].start];
                buffers[dn].start++;
                if(buffers[dn].start == buffers[dn].bufferSize) {
                    buffers[dn].start = 0;
                }
            }
            buffers[dn].bufferSize = temp;
            buffers[dn].start = 0;
            buffers[dn].end = buffers[dn].bufferCount;
            kfree(buffers[dn].buffer);
            buffers[dn].buffer = updatedBuffer;
            up(&buffers[dn].semaphore);
        }
        break;
    case IOCTL_GET_BUFFER_SIZE:
        if((error = verify_area(VERIFY_WRITE, (int *)data, _IOC_SIZE(command))) < 0) {
            printk("Proba odczytu z niepoprawnego miejsca w pamieci. Blad: %d\n", error);
            return error;
        }
        put_user(buffers[dn].bufferSize, (int *)data);
        break;
    case IOCTL_SET_DOT_LENGTH:
        temp = (int)data;
        if((error = verify_area(VERIFY_WRITE, (void *)data, _IOC_SIZE(data))) < 0) {
            printk("Proba zapisu do niepoprawnego miejsca w pamieci. Blad: %d\n", error);
            return error;
        }
        if(temp * HZ / 10 > buffers[dn].dashLength) {
            printk("Kropka nie moze trwac dluzej niz kreska.\n");
            return -EINVAL;
        } else {
            buffers[dn].dotLength = temp * HZ / 10;
        }
        break;
    case IOCTL_GET_DOT_LENGTH:
        if((error = verify_area(VERIFY_WRITE, (int *)data, _IOC_SIZE(command))) < 0) {
            printk("Proba odczytu z niepoprawnego miejsca w pamieci. Blad: %d\n", error);
            return error;
        }
        put_user(buffers[dn].dotLength, (int *)data);
        break;
    case IOCTL_SET_DASH_LENGTH:
        temp = (int)data;
        if((error = verify_area(VERIFY_WRITE, (void *)data, _IOC_SIZE(data))) < 0) {
            printk("Proba zapisu do niepoprawnego miejsca w pamieci. Blad: %d\n", error);
            return error;
        }
        if(temp * HZ  / 10 < buffers[dn].dotLength) {
            printk("Kreska nie moze trwac krocej niz kropka.\n");
            return -EINVAL;
        } else if(temp * HZ / 10 > buffers[dn].spaceLength) {
            printk("Kreska nie moze trwac dluzej niz spacja.\n");
            return -EINVAL;
        } else {
            buffers[dn].dashLength = temp * HZ / 10;
        }
        break;
    case IOCTL_GET_DASH_LENGTH:
        if((error = verify_area(VERIFY_WRITE, (int *)data, _IOC_SIZE(command))) < 0) {
            printk("Proba odczytu z niepoprawnego miejsca w pamieci. Blad: %d\n", error);
            return error;
        }
        put_user(buffers[dn].dashLength, (int *)data);
        break;
    case IOCTL_SET_SPACE_LENGTH:
        temp = (int)data;
        if((error = verify_area(VERIFY_WRITE, (void *)data, _IOC_SIZE(data))) < 0) {
            printk("Proba zapisu do niepoprawnego miejsca w pamieci. Blad: %d\n", error);
            return error;
        }
        if(temp * HZ / 10 < buffers[dn].dashLength || temp * HZ  / 10 < buffers[dn].dotLength) {
            printk("Pauza nie moze trwac krocej niz kreska.\n");
            return -EINVAL;
        } else {
            buffers[dn].spaceLength = temp * HZ / 10;
        }
        break;
    case IOCTL_GET_SPACE_LENGTH:
        if((error = verify_area(VERIFY_WRITE, (int *)data, _IOC_SIZE(command))) < 0) {
            printk("Proba odczytu z niepoprawnego miejsca w pamieci. Blad: %d\n", error);
            return error;
        }
        put_user(buffers[dn].spaceLength, (int *)data);
        break;
    default:
        return -ENOTTY;
    }
    return 0;
}

struct file_operations morse_ops = {
	write: morse_write,
	open: morse_open,
	release: morse_release,
	ioctl: morse_ioctl
};


void morse_init(void) {
    int i;
    printk("Inicjalizacja nadajnika Morsa\n");
	buffers = kmalloc(8 * sizeof(struct buffer), GFP_KERNEL);
    for (i = 0; i < 8; i++) {
        init_waitqueue(&buffers[i].writeQueue);
        buffers[i].useCount = 0;
        buffers[i].bufferSize = DEFAULT_BUFFER_SIZE;
        buffers[i].semaphore = MUTEX;
        buffers[i].dotLength = 2 * HZ / 10;
        buffers[i].dashLength = 6 * HZ / 10;
        buffers[i].spaceLength = 6 * HZ / 10;
        buffers[i].timerDelay = buffers[i].dotLength;
        init_timer(&buffers[i].timer);
        buffers[i].timer.data = (unsigned long)i;
        buffers[i].timer.function = on_timer_expire;
        buffers[i].timer.expires = jiffies + buffers[i].timerDelay;
        add_timer(&buffers[i].timer);
    }
    register_chrdev(MORSE_MAJOR,MORSE_NAME, &morse_ops);
}

int init_module() {
    morse_init();
    return 0;
}

void cleanup_module() {
    int i;
    for(i = 0; i < 8; i++) {
        del_timer(&buffers[i].timer);
    }
	kfree(buffers);
    unregister_chrdev(MORSE_MAJOR, MORSE_NAME);
}
