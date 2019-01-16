#ifndef MORSE_H
#define MORSE_H

#include <asm/semaphore.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/malloc.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include </usr/src/linux/drivers/char/console_struct.h>
#include "morse_ioctl.h"

#define DEFAULT_BUFFER_SIZE 256
#define MIN_BUFFER_SIZE 0
#define MAX_BUFFER_SIZE 1024

struct buffer {
	char *buffer;
	int bufferCount;
	int bufferSize;
	int start;
	int end;
	int useCount;
    int timerDelay;
	int dotLength;
	int dashLength;
	int spaceLength;
	struct semaphore semaphore;
	struct timer_list timer;
    struct wait_queue *writeQueue;
};

static void on_timer_expire(unsigned long dn);
int morse_open(struct inode *inode, struct file *file);
void morse_release(struct inode *inode, struct file *file);
int morse_write(struct inode *inode, struct file *file, const char *pB, int count);
int morse_ioctl(struct inode *inode, struct file *file, unsigned int command, unsigned long data);
void morse_init(void);
int init_module();
void cleanup_module();

#endif  /* MORSE_H */
