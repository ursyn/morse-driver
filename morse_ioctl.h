#ifndef MORSE_IOCTL_H
#define MORSE_IOCTL_H

#include <linux/ioctl.h>

/* Use 'm' (109) as magic number */
#define MORSE_MAJOR 'm'
#define MORSE_NAME "morse"

#define IOCTL_SET_BUFFER_SIZE _IOR(MORSE_MAJOR, 0, int)
#define IOCTL_GET_BUFFER_SIZE _IOR(MORSE_MAJOR, 1, int)
#define IOCTL_SET_DOT_LENGTH _IOR(MORSE_MAJOR, 2, int)
#define IOCTL_GET_DOT_LENGTH _IOR(MORSE_MAJOR, 3, int)
#define IOCTL_SET_DASH_LENGTH _IOR(MORSE_MAJOR, 4, int)
#define IOCTL_GET_DASH_LENGTH _IOR(MORSE_MAJOR, 5, int)
#define IOCTL_SET_SPACE_LENGTH _IOR(MORSE_MAJOR, 6, int)
#define IOCTL_GET_SPACE_LENGTH _IOR(MORSE_MAJOR, 7, int)

#endif /* MORSE_IOCTL_H */