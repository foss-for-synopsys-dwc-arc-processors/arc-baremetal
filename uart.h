#ifndef __UART_H__
#define __UART_H__

#ifdef NO_UART
#define printf(fmt, args...)
#define uart_print_s(p)
#define uart_print_n(n)
#else
extern void uart_print_s(const char *p);
extern void uart_print_n(int n);
extern void uart_print_s_n(const char *p, int n);
#define printf(str)		uart_print_s(str)
#define printf2(str, n)		uart_print_s_n(str, n)
#endif

#endif
