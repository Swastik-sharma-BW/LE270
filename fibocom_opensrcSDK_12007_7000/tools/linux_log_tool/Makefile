#arm gcc tool
#CROSS_COMPILE = ~/ARM_Linux_GCC/bin/arm-none-linux-gnueabi-


CC = ${CROSS_COMPILE}gcc

CFLAGS = -g -Wall

TARGET = log_capture

C_SOURCES = log_capture.c

default: clean
	${CC} ${CFLAGS} -s ${C_SOURCES} -o ${TARGET} 

clean:
	rm -f ${TARGET} 

