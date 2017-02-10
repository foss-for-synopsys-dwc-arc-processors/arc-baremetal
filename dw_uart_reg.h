/**************************************************************************
***
*** File: dw_apb_uart_reg.h
***
*** Comments:
***   This file contains register access structure and associated bit
***   definations.
***
***************************************************************************/

#ifndef __DW_UART_REG_H__
#define __DW_UART_REG_H__

/* DW UART bit definitions */

/* IER */
#define DW_UART_IER_DATA_AVAIL                   (0x01)
#define DW_UART_IER_XMIT_EMPTY                   (0x02)
#define DW_UART_IER_LINE_STATUS                  (0x04)
#define DW_UART_IER_MDM_STATUS                   (0x08)
#define DW_UART_IER_PTIME                        (0x80)

/* IIR */
/* IIR READ */
#define DW_UART_IIR_IP                           (0x01)
#define DW_UART_IIR_MASK                         (0x0E)
#define DW_UART_IIR_READ_FIFO_ENABLE             (0xC0)

/* Possible interrupt IIR_MASK values */
#define DW_UART_IIR_MDM_STATUS                   (0x00)
#define DW_UART_IIR_XMIT_EMPTY                   (0x02)
#define DW_UART_IIR_DATA_AVAIL                   (0x04)
#define DW_UART_IIR_LINE_STATUS                  (0x06)
#define DW_UART_IIR_RX_TIMEOUT                   (0x0C)
#define DW_UART_IIR_INT_ID_MASK                  (0x0f)

/* IIR WRITE */
#define DW_UART_IIR_FIFO_ENABLE                  (0x01)
#define DW_UART_IIR_RCVR_FIFO_RESET              (0x02)
#define DW_UART_IIR_XMIT_FIFO_RESET              (0x04)
#define DW_UART_IIR_DMA_MODE_SELECT              (0x08)
#define DW_UART_IIR_RCV_TRIGGER_MASK             (0xC0)

/* Values for IIR receive trigger */
#define DW_UART_IIR_TRIGGER_LEVEL_1_CHAR         (0x00)
#define DW_UART_IIR_TRIGGER_LEVEL_1_4_FULL       (0x40)
#define DW_UART_IIR_TRIGGER_LEVEL_1_2_FULL       (0x80)
#define DW_UART_IIR_TRIGGER_LEVEL_2_LESS_FULL    (0xC0)

/* LCR */
#define DW_UART_LCR_WORD_LEN_MASK                (0x03)
#define DW_UART_LCR_STOP_BIT_MASK                (0x04)
#define DW_UART_LCR_PARITY_MASK                  (0x18)
#define DW_UART_LCR_STICK_PARITY                 (0x20)
#define DW_UART_LCR_BREAK                        (0x40)
#define DW_UART_LCR_DLAB                         (0x80)

/* Word length values */
#define DW_UART_LCR_WORD_LEN5                    (0x00)
#define DW_UART_LCR_WORD_LEN6                    (0x01)
#define DW_UART_LCR_WORD_LEN7                    (0x02)
#define DW_UART_LCR_WORD_LEN8                    (0x03)

/* stop bit values */
#define DW_UART_LCR_1_STOP_BIT                   (0x00)
#define DW_UART_LCR_2_STOP_BIT                   (0x04)

/* Parity bit values */
#define DW_UART_LCR_PARITY_NONE                  (0x00)
#define DW_UART_LCR_PARITY_ODD                   (0x08)
#define DW_UART_LCR_PARITY_EVEN                  (0x18)

/* MCR */
#define DW_UART_MCR_DTR                          (0x01)
#define DW_UART_MCR_RTS                          (0x02)
#define DW_UART_MCR_IRQ_ENABLE                   (0x08)
#define DW_UART_MCR_LOOPBACK                     (0x10)
#define DW_UART_MCR_AFCE                         (0x20)
#define DW_UART_MCR_SIRE                         (0x40)

/* LSR */
#define DW_UART_LSR_DR                           (0x01)
#define DW_UART_LSR_OVERRUN                      (0x02)
#define DW_UART_LSR_PARITYERR                    (0x04)
#define DW_UART_LSR_FRAMEERR                     (0x08)
#define DW_UART_LSR_BREAKRCVD                    (0x10)
#define DW_UART_LSR_TXD_EMPTY                    (0x20)
#define DW_UART_LSR_TX_STATUS                    (0x40)
#define DW_UART_LSR_RX_FIFOERR                   (0x80)

/* MSR */
#define DW_UART_MSR_DCTS                         (0x01)
#define DW_UART_MSR_DDSR                         (0x02)
#define DW_UART_MSR_TERI                         (0x04)
#define DW_UART_MSR_DDCD                         (0x08)
#define DW_UART_MSR_CTS                          (0x10)
#define DW_UART_MSR_DSR                          (0x20)
#define DW_UART_MSR_RIC                          (0x40)
#define DW_UART_MSR_DCD                          (0x80)

/* FCR */
#define DW_UART_FCR_FEN                          (0x01)
#define DW_UART_FCR_RFR                          (0x02)
#define DW_UART_FCR_TFR                          (0x04)
#define DW_UART_FCR_DMS                          (0x08)
#define DW_UART_FCR_RTL                          (0xC0)

/* USR */
#define DW_UART_USR_BUSY                         (0x01)
#define DW_UART_USR_TFNF                         (0x02)
#define DW_UART_USR_TFE                          (0x04)
#define DW_UART_USR_RFNE                         (0x08)
#define DW_UART_USR_RFF                          (0x10)

/* SFE */
#define DW_UART_SFE_SHADOW_FIFO_ENABLE           (0x01)

/* SRR */
#define DW_UART_SRR_UR                           (0x01)
#define DW_UART_SRR_RFR                          (0x02)
#define DW_UART_SRR_XFR                          (0x04)

/* SRT */
#define DW_UART_SRT_TRIGGER_LEVEL_1_CHAR         (0x00)
#define DW_UART_SRT_TRIGGER_LEVEL_1_4_FULL       (0x01)
#define DW_UART_SRT_TRIGGER_LEVEL_1_2_FULL       (0x02)
#define DW_UART_SRT_TRIGGER_LEVEL_2_LESS_FULL    (0x03)

/* STET*/
#define DW_UART_STET_FIFO_EMPTY                  (0x00)
#define DW_UART_STET_2_CHARS_IN_FIFO             (0x01)
#define DW_UART_STET_1_4_FULL                    (0x02)
#define DW_UART_STET_1_2_FULL                    (0x03)

/* PARAM */
#define DW_UART_PARAM_FIFO_STAT                  (0x00000400U)

#if (LITTLE_ENDIAN)
   typedef union reg_union
   {
      uint32_t DATA32;
      struct reg_struct
      {
         uint32_t DATA : 8;
         uint32_t RESERVED : 24;
      }REG;
   } REG_UNION;
#else /* If Processor is Big Endian */
   typedef union reg_union
   {
      uint32_t DATA32;
      struct reg_struct
      {
         uint32_t RESERVED : 24;
         uint32_t DATA : 8;
      }REG;
   } REG_UNION;
#endif

typedef union reserved_union
{
   unsigned DATA32;
}RESERVED_UNION;

typedef volatile struct dw_uart_reg
{
   REG_UNION DATA;                  /* data in/out and DLL */
   REG_UNION IER;                   /* Interrupt enable register and DLH */
   REG_UNION IIR;                   /* Interrupt Id register */
   REG_UNION LCR;                   /* Line control Register */
   REG_UNION MCR;                   /* Modem control register */
   REG_UNION LSR;                   /* Line Status Register */
   REG_UNION MSR;                   /* Modem status Register */
   REG_UNION SCRATCHPAD;            /* Uart scratchpad register */
   REG_UNION LPDLL;                 /* Low Power Divisor Latch (Low) Register */
   REG_UNION LPDLH;                 /* Low Power Divisor Latch (High) Register */
   RESERVED_UNION RES1[2];          /* Reserved */
   REG_UNION SHR[16];               /* Shadow data register */
   REG_UNION FAR;                   /* FIFO Access register */
   REG_UNION TFR;                   /* Transmit FIFO Read */
   REG_UNION RFW;                   /* Receive FIFO write */
   REG_UNION USR;                   /* UART status register */
   REG_UNION TFL;                   /* Transmit FIFO level */
   REG_UNION RFL;                   /* Receive FIFO level */
   REG_UNION SRR;                   /* Software reset register */
   REG_UNION SRTS;                  /* Shadow request to send */
   REG_UNION SBCR;                  /* Shadow break control */
   REG_UNION SDMAM;                 /* Shadow DMA mode */
   REG_UNION SFE;                   /* Shadow FIFO enable */
   REG_UNION SRT;                   /* Shadow RCVR Trigger */
   REG_UNION STET;                  /* Shadow TX empty register */
   REG_UNION HTX;                   /* Halt TX */
   REG_UNION DMASA;                 /* DMA Software ACK */
   RESERVED_UNION RES2[18];         /* Reserved */
   uint32_t  CPR;                   /* Component parameter register */
   REG_UNION UCV;                   /* UART Component Version */
   REG_UNION CTR;                   /* Component typw register */
} DW_UART_STRUCT, *DW_UART_STRUCT_PTR;

#endif //__DW_UART_REG_H__

/* EOF */

