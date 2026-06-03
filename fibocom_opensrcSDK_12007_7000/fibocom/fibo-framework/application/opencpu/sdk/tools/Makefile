INC_PATH=$(shell pwd)
CC=gcc


CFLAGS = -g 
INC = -I$(INC_PATH)/inc

LIBS = -L$(INC_PATH)/lib
SOURCES = $(wildcard $(INC_PATH)/*.c)

OBJS = $(SOURCES:.c=.o)
%.o:%.c
	@$(CC) $(CFLAGS) -g -c $< -o $@ $(INC)
	@echo "build $@ ..."

headset: headset.o crc32.o
	@$(CC) $^ -o $@ $(LIBS)

entry: entry.o
	@$(CC) $^ -o $@ $(LIBS)
	
all: headset ptable
	
.PHONY: clean
clean:
	@-rm -rf $(OBJS) headset ptable
