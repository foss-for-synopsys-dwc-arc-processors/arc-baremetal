# pass NO_UART=1 from command line if you dont wish to build for UART
ifdef NO_UART
  ifeq ("$(origin NO_UART)", "command line")
	EXTRA_CFLAGS = -DNO_UART
  endif
endif

#EXTRA_CFLAGS += -DNO_IRQ

CC	 = arc-linux-gcc
CFLAGS   = -mcpu=archs ${EXTRA_CFLAGS} -O2 -fomit-frame-pointer -mno-sdata
LDFLAGS  = -T baremetal.lds -static -nostartfiles

OBJS	 = entry.o arc.o test.o

ifndef NO_UART
  ifeq ("$(origin DW_UART)", "command line")
	OBJS	+= dw_uart.o
  else
	OBJS	+= arc_uart.o
  endif
endif

%.o:	%.S arc.h
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c arc.h
	$(CC) $(CFLAGS) -c -o $@ $<

img : $(OBJS)
	$(CC) $(LDFLAGS) -o img.elf $(OBJS)

clean:
	rm -f $(OBJS) img.elf

