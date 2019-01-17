all:
	gcc -DMODULE -D__KERNEL__ -O2 -c morse.c
	gcc -o morse_test morse_test.c
	gcc -o morse_ioctl morse_ioctl.c
	
test: all
	-rmmod morse
	insmod morse
	-mknod /dev/morse0 c 109 0
	-mknod /dev/morse1 c 109 1
	-mknod /dev/morse2 c 109 2
	-mknod /dev/morse3 c 109 3
	-mknod /dev/morse4 c 109 4
	-mknod /dev/morse5 c 109 5
	-mknod /dev/morse6 c 109 6
	-mknod /dev/morse7 c 109 7
	./morse_test
	
clean:
	rm -rf morse.o morse_test.o morse_ctrl.o
	-rm -rf /dev/morse*
	-rmmod morse
