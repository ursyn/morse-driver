#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "morse_ioctl.h"

/*
 * Program do zmiany nastepujacych parametrow nadajnika Morsa:
 * [0] numer nadajnika [0-7]
 * [1] typ parametru:
 *  b - zmiana rozmiaru bufora
 *  . - zmiana dlugosci trwania kropki
 *  - - zmiana dlugosci trwania kreski
 *  x - zmiana dlugosci trwania spacji
 * [2] wartosc parametru
 * Zwracana wartosc: inna niz 0 w przypadku wystapienia bledu (zalezna od implementacji morse.c)
 * 
 * Przyklad: ./morse_ioctl 0 b 600
 */
int main(int argc, char* argv[]) {
    int result;
    int file_desc;
    int number = atoi(argv[1]);
    char param = argv[1][2];
    int value = atoi(argv[3]);
    char *base_filename = "/dev/morse";
    char *full_filename = malloc(snprintf(NULL, 0, "%s %d", base_filename, number) + 1);
    sprintf(full_filename, "%s%d", base_filename, number);
    file_desc = open(full_filename, O_RDWR);
    switch(param) {
    case 'b':
        result = ioctl(file_desc, IOCTL_SET_BUFFER_SIZE, value);
        if(result < 0) {
            printf("Nie udalo sie ustawic wielkosci bufora. Blad: %d\n", result);
            return result;
        } else {
            printf("Poprawnie ustawiono wiekosc bufora: %d\n", value);
        }
        break;
    case '.':
        result = ioctl(file_desc, IOCTL_SET_DOT_LENGTH, value);
        if(result < 0) {
            printf("Nie udalo sie ustawic dlugosci kropki. Blad: %d\n", result);
            return result;
        } else {
            printf("Poprawnie ustawiono wiekosc kropki: %d\n", value);
        }
        break;
    case '-':
        result = ioctl(file_desc, IOCTL_SET_DASH_LENGTH, value);
        if(result < 0) {
            printf("Nie udalo sie ustawic dlugosci kreski. Blad: %d\n", result);
            return result;
        } else {
            printf("Poprawnie ustawiono wiekosc kreski: %d\n", value);
        }
        break;
    case 'x':
        result = ioctl(file_desc, IOCTL_SET_SPACE_LENGTH, value);
        if(result < 0) {
            printf("Nie udalo sie ustawic dlugosci spacji. Blad: %d\n", result);
            return result;
        } else {
            printf("Poprawnie ustawiono wiekosc spacji: %d\n", value);
        }
        break;
    }
    return 0;
}
