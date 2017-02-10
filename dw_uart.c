#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "dw_uart_reg.h"

void dw_uart_initDevice(DW_UART_STRUCT_PTR uartRegs, uint32_t ip_clk, int baudrate) 
{
    uartRegs->MCR.DATA32 = 0;
    uartRegs->IIR.DATA32 = 0x07;
   uartRegs->LCR.DATA32  = 0x83;
   uartRegs->DATA.DATA32 = (ip_clk / (16*baudrate));  //DLL div = CPU_clock / 16 * baudrate. 
   uartRegs->IER.DATA32  = 0x00;                      //DLH
   uartRegs->LCR.DATA32  = 0x03;
    uartRegs->IER.DATA32 = 0x01;
}

#define AXS101_UART_BASE 0xF0000000U
#define AXS101_UART_BAUDRATE                                (115200)
#define AXS101_REF_CLOCK                                    (50000000)
#define MAX_DEBUG_MSG_FPGA	            (128)


void uart_open(void)
{
	DW_UART_STRUCT_PTR   uart  = (DW_UART_STRUCT_PTR)  (AXS101_UART_BASE);
        dw_uart_initDevice(uart, AXS101_REF_CLOCK, AXS101_UART_BAUDRATE);

}

void dw_uart_print(DW_UART_STRUCT_PTR uartRegs, const char * pBuf)
{
	unsigned int i = MAX_DEBUG_MSG_FPGA;

   unsigned char byte = *pBuf++;
   while(byte && i--)
   {
      // wait when busy
      while((uartRegs->USR.DATA32 & DW_UART_USR_BUSY))
      {;}
            
      //if fifo stat is true
      if ((uartRegs->CPR & DW_UART_PARAM_FIFO_STAT) == DW_UART_PARAM_FIFO_STAT)
      {
         while( !(uartRegs->USR.DATA32 & DW_UART_USR_TFNF))
         {;}      
      }
         
      // transmitt data byte
      uartRegs->DATA.DATA32 = byte;
      
      byte = *pBuf++;

      //use sync(), due to multiple outstanding transactions      
      //_sync();
   }
}

void uart_print_s(const char *str)
{
	DW_UART_STRUCT_PTR   uart  = (DW_UART_STRUCT_PTR)  (AXS101_UART_BASE);
	dw_uart_print(uart, str);
}

void uart_put_char(unsigned char ch)
{
	char str[2];

	str[0] = ch;
	str[1] = '\0';

	uart_print_s(str);
}

char dw_uart_getChar (DW_UART_STRUCT_PTR uartRegs)
{
   return ((char)uartRegs->DATA.DATA32);
}

char uart_get_char(void)
{
	char ch;

	DW_UART_STRUCT_PTR   uart  = (DW_UART_STRUCT_PTR)  (AXS101_UART_BASE);

	ch = dw_uart_getChar(uart);
	return ch;
}

void uart_isr(void)
{
	char ch = uart_get_char();

	uart_put_char(ch);
	if (ch == '\r')
		uart_put_char('\n');
		
}

void uart_print_s_n(const char *str, int n)
{
	uart_print_s(str);
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
