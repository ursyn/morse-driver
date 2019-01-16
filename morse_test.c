#include <fcntl.h>
#include <linux/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "morse_ioctl.h"

#define MORSE_PATH0 "/dev/morse0"
#define MORSE_PATH1 "/dev/morse1"
#define MORSE_PATH2 "/dev/morse2"
#define MORSE_PATH3 "/dev/morse3"
#define MORSE_PATH4 "/dev/morse4"
#define MORSE_PATH5 "/dev/morse5"
#define MORSE_PATH6 "/dev/morse6"
#define MORSE_PATH7 "/dev/morse7"

ioctl_set_buffersize(int file, int size) {
    int result;
    result = ioctl(file, IOCTL_SET_BUFFER_SIZE, size);
    if (result < 0) {
        printf("Nie udalo sie ustawic rozmiaru bufora. Wynik: %d\n", result);
    } else {
        printf("Ustawiono rozmiar bufora: %d\n", size);
    }
}

ioctl_get_buffersize(int file) {
    int result;
    int receivedSize;
    result = ioctl(file, IOCTL_GET_BUFFER_SIZE, &receivedSize);
    if (result < 0) {
        printf("Nie udalo sie pobrac rozmiaru bufora. Wynik: %d\n", result);
    } else {
        printf("Odczytano rozmiar bufora: %d\n", receivedSize);
    }
}

ioctl_set_dot_length(int file, int size) {
    int result;
    result = ioctl(file, IOCTL_SET_DOT_LENGTH, size);
    if (result < 0) {
        printf("Nie udalo sie ustawic dlugosci kropki. Wynik: %d\n", result);
    } else {
        printf("Ustawiono dlugosc kropki: %d\n", size * HZ);
    }
}

ioctl_get_dot_length(int file) {
    int result;
    int receivedSize;
    result = ioctl(file, IOCTL_GET_DOT_LENGTH, &receivedSize);
    if (result < 0) {
        printf("Nie udalo sie pobrac dlugosci kropki. Wynik: %d\n", result);
    } else {
        printf("Odczytano dlugosc kropki: %d\n", receivedSize);
    }
}

ioctl_set_dash_length(int file, int size) {
    int result;
    result = ioctl(file, IOCTL_SET_DASH_LENGTH, size);
    if (result < 0) {
        printf("Nie udalo sie ustawic dlugosci kreski. Wynik: %d\n", result);
    } else {
        printf("Ustawiono dlugosc kreski: %d\n", size * HZ);
    }
}

ioctl_get_dash_length(int file) {
    int result;
    int receivedSize;
    result = ioctl(file, IOCTL_GET_DASH_LENGTH, &receivedSize);
    if (result < 0) {
        printf("Nie udalo sie pobrac dlugosci kreski. Wynik: %d\n", result);
    } else {
        printf("Odczytano dlugosc kreski: %d\n", receivedSize);
    }
}

ioctl_set_space_length(int file, int size) {
    int result;
    result = ioctl(file, IOCTL_SET_SPACE_LENGTH, size);
    if (result < 0) {
        printf("Nie udalo sie ustawic dlugosci spacji. Wynik: %d\n", result);
    } else {
        printf("Ustawiono dlugosc spacji: %d\n", size * HZ);
    }
}

ioctl_get_space_length(int file) {
    int result;
    int receivedSize;
    result = ioctl(file, IOCTL_GET_SPACE_LENGTH, &receivedSize);
    if (result < 0) {
        printf("Nie udalo sie pobrac dlugosci spacji. Wynik: %d\n", result);
    } else {
        printf("Odczytano dlugosc spacji: %d\n", receivedSize);
    }
}

main() {
    int file, result;
    int value = 512;

    file = open(MORSE_PATH0, O_RDWR);
    if(file < 0) {
        printf("Nie udalo sie otworzyc %s\n", MORSE_PATH0);
        exit(-1);
    } else {
        printf("Otwarto %s\n", MORSE_PATH0);
    }
    ioctl_get_buffersize(file);
    ioctl_set_buffersize(file, value);
    ioctl_get_buffersize(file);
    value = 800;
    ioctl_set_buffersize(file, value);
    ioctl_get_buffersize(file);
    value = -10;
    ioctl_set_buffersize(file, value);
    ioctl_get_buffersize(file);
    value = 1024;
    ioctl_set_buffersize(file, value);
    ioctl_get_buffersize(file);
    value = 2048;
    ioctl_set_buffersize(file, value);
    ioctl_get_buffersize(file);
    close(file);

    file = open(MORSE_PATH1, O_RDWR);
    if(file < 0) {
        printf("Nie udalo sie otworzyc %s\n", MORSE_PATH1);
        exit(-1);
    } else {
        printf("Otwarto %s\n", MORSE_PATH1);
    }
    ioctl_get_buffersize(file);
    ioctl_get_dot_length(file);
    ioctl_get_dash_length(file);
    ioctl_get_space_length(file);
    value = 3;
    ioctl_set_dot_length(file, value);
    ioctl_get_dot_length(file);
    value = 2;
    ioctl_set_dash_length(file, value);
    ioctl_get_dash_length(file);
    value = 4;
    ioctl_set_dash_length(file, value);
    ioctl_get_dash_length(file);
    value = 5;
    ioctl_set_space_length(file, value);
    ioctl_get_space_length(file);
    value = 6;
    ioctl_set_dash_length(file, value);
    ioctl_get_dash_length(file);
    close(file);

    file = open(MORSE_PATH4, O_RDWR);
    if(file < 0) {
        printf("Nie udalo sie otworzyc %s\n", MORSE_PATH4);
        exit(-1);
    } else {
        printf("Otwarto %s\n", MORSE_PATH4);
    }
    ioctl_get_buffersize(file);
    ioctl_get_dot_length(file);
    ioctl_get_dash_length(file);
    ioctl_get_space_length(file);
    value = 456;
    ioctl_set_buffersize(file, value);
    ioctl_get_buffersize(file);
    value = 7;
    ioctl_set_dot_length(file, value);
    ioctl_get_dot_length(file);
    ioctl_get_space_length(file);
    value = 15;
    ioctl_set_space_length(file, value);
    ioctl_get_space_length(file);
    value = 10;
    ioctl_set_dash_length(file, value);
    ioctl_get_dash_length(file);
    value = 7;
    ioctl_set_dot_length(file, value);
    ioctl_get_dot_length(file);
    value = 4;
    ioctl_set_dash_length(file, value);
    ioctl_get_dash_length(file);
    value = 3;
    ioctl_set_space_length(file, value);
    ioctl_get_space_length(file);
    value = 4;
    ioctl_set_dash_length(file, value);
    ioctl_get_dash_length(file);
    close(file);

    file = open(MORSE_PATH6, O_RDWR);
    if(file < 0) {
        printf("Nie udalo sie otworzyc %s\n", MORSE_PATH6);
        exit(-1);
    } else {
        printf("Otwarto %s\n", MORSE_PATH6);
    }
    ioctl_get_buffersize(file);
    ioctl_get_dot_length(file);
    ioctl_get_dash_length(file);
    ioctl_get_space_length(file);
    value = 5;
    ioctl_set_space_length(file, value);
    value = 4;
    ioctl_set_dash_length(file, value);
    value = 2;
    ioctl_set_dot_length(file, value);
    close(file);

    file = open(MORSE_PATH1, O_RDWR);
    if(file < 0) {
        printf("Nie udalo sie otworzyc %s\n", MORSE_PATH1);
        exit(-1);
    } else {
        printf("Otwarto %s\n", MORSE_PATH1);
    }
    ioctl_get_buffersize(file);
    ioctl_get_dot_length(file);
    ioctl_get_dash_length(file);
    ioctl_get_space_length(file);
    close(file);

    return 0;
}
