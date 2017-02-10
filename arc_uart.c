
#include "uart.h"

#define UART_BASE0         0xc0fc1000
/* #define UART_BASE0         0xf0000000 */

/* Bits for UART Status Reg (R/W) */
#define RXIENB  0x04    // Receive Interrupt Enable
#define TXIENB  0x40    // Transmit Interrupt Enable

#define RXEMPTY 0x20    // Receive FIFO Empty: No char received
#define TXEMPTY 0x80    // Transmit FIFO Empty, thus char can be written into

#define RXFULL  0x08    // Receive FIFO full
#define RXFULL1 0x10    // Receive FIFO has space for 1 char (tot space=4)

#define RXFERR  0x01    // Frame Error: Stop Bit not detected
#define RXOERR  0x02    // OverFlow Err: Char recv but RXFULL still set


/* UART Register set:
 * Word aligned, but only 8 bits wide
 */
typedef volatile struct {
  unsigned char id0 __attribute__((aligned(4)));
  unsigned char id1 __attribute__((aligned(4)));
  unsigned char id2 __attribute__((aligned(4)));
  unsigned char id3 __attribute__((aligned(4)));
  unsigned char data __attribute__((aligned(4)));
  unsigned char status __attribute__((aligned(4)));
  unsigned char baudl __attribute__((aligned(4)));
  unsigned char baudh __attribute__((aligned(4)));
} arc_uart_reg;

static inline arc_uart_reg *const RBASE(int line)
{                                                       
    arc_uart_reg *p = (arc_uart_reg *) UART_BASE0;      
    asm ("; fix %0": "+r" (p));                         
    return p;                                           
}

#define UART_REG_GET(u, reg)    	(RBASE(u)->reg)
#define UART_REG_SET(u, reg, val)   (RBASE(u)->reg = (val))

#define UART_GET_STATUS(uart)		UART_REG_GET(uart,status)
#define UART_SET_DATA(uart, val)   	UART_REG_SET(uart,data,val)
#define UART_SET_BAUDH(uart, val)  	UART_REG_SET(uart,baudh,val)
#define UART_SET_BAUDL(uart, val)  	UART_REG_SET(uart,baudl,val)


static const int uart_baud = 115200;
static const int uart_clk = 70000000;
static int uart=0;

void uart_print_s(const char *p);

void uart_open()
{
	int line = 0;
    unsigned int baud_h, baud_l, hw_val;

    /* Formula for ARC UART is: hw-val = ((CLK/(BAUD*4)) -1)
     * spread over two 8-bit registers
     */
    hw_val = uart_clk / (uart_baud*4) -1;
    baud_l = hw_val & 0xFF;
    baud_h = (hw_val >> 8) & 0xFF;

	UART_SET_BAUDH(uart, baud_h);
	UART_SET_BAUDL(uart, baud_l);

	uart_print_s("\r\nUART ready\n");
}

void uart_put_char(int ch)
{
    while (!(UART_GET_STATUS(uart) & TXEMPTY));

	UART_SET_DATA(uart, ch);
}

/****************************************************/

void uart_print_s(const char *p)
{
    while (*p != '\0') {
        if (*p == '\n')
            uart_put_char('\r');

        uart_put_char(*p++);
    }
}

void uart_print_s_n(const char *p, int n)
{
	uart_print_s(p);
	uart_print_n(n);
	uart_put_char('\r');
	uart_put_char('\n');
}

static char dec_to_hex[] = {'0','1','2','3','4','5','6','7','8','9',
                            'A','B','C','D','E','F'};

void uart_print_n(int n)
{
	int i, nibble;
	int leading_zeroes = 1;
	int zero_ok = 1;
	unsigned int num = n;

    /* If @num is Zero don't print it */
    if ( zero_ok || num != 0 ) {

    	uart_put_char('0');
    	uart_put_char('x');

        /* break num into 8 nibbles */
        for (i = 7; i >=0; i--) {
            nibble = (num >> (i << 2)) & 0xF;
            if ( nibble == 0 ) {
                if ( leading_zeroes && i != 0 ) continue;
            }
            else {
                if ( leading_zeroes )
                    leading_zeroes = 0;
            }
            uart_put_char(dec_to_hex[nibble]);
        }
    }

    uart_put_char(' ');
}

void uart_isr(void)
{

}
