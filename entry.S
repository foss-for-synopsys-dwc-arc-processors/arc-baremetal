;****************************************************************
; Vector Table
;****************************************************************

	.section .vector,"a",@progbits
	.align 4

; Exception Vectors [0 - 15]
.rept 16
	.word unhandled
.endr

; Core Private Reserved Interrupts [16..23] timer, pct, ....
.word irq16_isr
.word unhandled
.word unhandled
.word irq19_isr
.word unhandled
.word unhandled
.word unhandled
.word unhandled

; External "Common" Interrupts hooked up via ARConnect IDU [24..C]
.word irq24_isr

; External Private Interrupts [C+1..] hooked up directly
.word unhandled

;****************************************************************
; stack (for 2 cores currently )
;****************************************************************

	.section  .bss

.global STACKCPUS
.align 8192
.type   STACKCPUS, @object
.size   STACKCPUS, 8192 * 2
STACKCPUS:
    .zero 8192 * 2

;****************************************************************
; Entry point
;****************************************************************

.macro ARC_ENTRY name
	.globl \name
	.align 4
	\name:
.endm

.macro ARC_EXIT name
	.size \name, .-\name
.endm

	.section .text, "ax",@progbits

ARC_ENTRY _stext

	mov fp, 0

	; opencoded smp_processor_id() from arc.h
	; AUX_IDENTITY Reg [ 3  2  1  0 ]
	;		           ^^^	    => 0 for Master, !0 otherwise
	lr   r0, [identity]
	lsr  r0, r0, 8
	bmsk r0, r0, 7
	brne r0, 0, .Lsecondary

	mov  sp, STACKCPUS + 8192
	bl   arc_cpu_init

#ifndef NO_UART
        bl  uart_open
#endif

	b   tx_primary_core_entry

.Lsecondary:
	add  r1, r0, 1
	mpy  r1, STACKCPUS, r1
	mov  sp, r1
	bl   arc_cpu_init

	b    tx_additional_core_entry

ARC_EXIT _stext

;****************************************************************
; Low level interrupt handlers
;****************************************************************

ARC_ENTRY irq16_isr
	bl arc_timer_isr
	rtie
ARC_EXIT irq16_isr

ARC_ENTRY irq19_isr
	bl arc_ici_isr
	rtie
ARC_EXIT irq19_isr

ARC_ENTRY irq24_isr
	bl uart_isr
	rtie
ARC_EXIT irq24_isr

ARC_ENTRY unhandled
	flag 1
ARC_EXIT unhandled

