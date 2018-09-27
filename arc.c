#include "arc.h"
#include "uart.h"

/*********************************************************************
 * Core Intc setup
 *********************************************************************/

#define AUX_STAT32	0x00a

#define AUX_IVT_BASE	0x025
#define AUX_VOL		0x05e

#define AUX_IRQ_CTRL	0x00E
#define AUX_IRQ_SEL	0x40b
#define AUX_IRQ_PRIO	0x206

#define INTC_DEF_PRIO	1

void arc_core_intc_init(void)
{
	unsigned int stat32;

	struct {
		unsigned int save_nr_gpr_pairs:5, res:4,
			     save_blink:1, save_lp_regs:1, save_u_to_u:1,
			     res2:1, save_idx_regs:1, res3:18;
	} ictrl;

	*(unsigned int *)&ictrl = 0;

	/* setup auto register save on interrupts */

	ictrl.save_nr_gpr_pairs = 7;	/* r0 to r13 */
	ictrl.save_blink = 1;
	ictrl.save_lp_regs = 1;		/* LP_COUNT, LP_START, LP_END */
	ictrl.save_u_to_u = 0;		/* user ctxt saved on kernel stack */
	ictrl.save_idx_regs = 1;	/* JLI, LDI, EI */

	_sr(*(unsigned int *)&ictrl, AUX_IRQ_CTRL);

	/* setup default interruption threshhold */
	stat32 = _lr(AUX_STAT32) | (INTC_DEF_PRIO << 1);
	asm volatile("kflag	%0\n" : :"r"(stat32));
}

void arc_cpu_init(void)
{
        char *nm[] = { "cpu0", "cpu1", "cpu2", "cpu3" };

	extern char _vec_tbl[];
	_sr((unsigned int)_vec_tbl, AUX_IVT_BASE);
	
	/* 0xc000_0000 in uncached */
	_sr(0xc0000000, AUX_VOL);

	arc_core_intc_init();

	/* setup irqs to interrupt at default interruption threshhold */
	_sr(IRQ_TIMER, AUX_IRQ_SEL);
        _sr(INTC_DEF_PRIO, AUX_IRQ_PRIO);

	_sr(IRQ_IPI, AUX_IRQ_SEL);
        _sr(INTC_DEF_PRIO, AUX_IRQ_PRIO);

	uart_print_s(nm[smp_processor_id()]);
}

static void (*tmr_callbk[4])(int arg);
static void (*ici_callbk[4])(int arg);

void arc_register_isr(int irq, void (*fn)(int), int arg)
{
	if (irq == IRQ_TIMER)
		tmr_callbk[smp_processor_id()] = fn;
	else if (irq == IRQ_IPI)
		ici_callbk[smp_processor_id()] = fn;
	else
		arc_halt();
}

/*********************************************************************
 * Timers
 *********************************************************************/

#define AUX_TIM0_CNT	0x021
#define AUX_TIM0_CTRL	0x022
#define AUX_TIM0_LIMIT	0x023

void arc_timer_setup(unsigned int cycles)
{
	_sr(cycles, AUX_TIM0_LIMIT);	/* interupt after CNT == @cycles */
	_sr(0,      AUX_TIM0_CNT);	/* initial CNT */
	_sr(0x3,    AUX_TIM0_CTRL);	/* Interrupt enable, count only when NOT halted */
}

void arc_timer_isr(void)
{
	void (*fn)(int);

	_sr(0x3,    AUX_TIM0_CTRL);	/* ACK Interrupt (also re-enables) */

	fn = tmr_callbk[smp_processor_id()];
	if (fn)
		(*fn)(IRQ_TIMER);
}

/*********************************************************************
 * Inter Core interruts
 *********************************************************************/

#define AUX_MCIP_BCR		0x0d0
#define AUX_MCIP_CMD		0x600
#define AUX_MCIP_WDATA		0x601
#define AUX_MCIP_READBK		0x602

#define CMD_ICI_GENERATE_IRQ	0x1
#define CMD_ICI_GENERATE_ACK	0x2
#define CMD_ICI_READ_STATUS	0x3
#define CMD_ICI_CHECK_SOURCE	0x4

static inline void __mcip_cmd(unsigned int cmd, unsigned int param)
{
	struct mcip_cmd {
		unsigned int cmd:8, param:16, pad:8;
	} buf;

	buf.pad = 0;
	buf.cmd = cmd;
	buf.param = param;

	_sr(*(unsigned int *)&buf, AUX_MCIP_CMD);
}
/* ARConnect accesses across cores needs to be serialized */
static spinlock_t mcip_lock;

void arc_ici_send(unsigned int cpu)
{
	int ipi_pend;

	/* ARConnect ICI doesn't allow interrupting to self */
	if (cpu == smp_processor_id())
		arc_halt();

	spinlock_acquire(&mcip_lock);

	__mcip_cmd(CMD_ICI_READ_STATUS, cpu);
	ipi_pend = _lr(AUX_MCIP_READBK);
	if (!ipi_pend)
		__mcip_cmd(CMD_ICI_GENERATE_IRQ, cpu);

	spinlock_release(&mcip_lock);
}

void arc_ici_isr(void)
{
	unsigned int senders, c;
	void (*fn)(int);

	spinlock_acquire(&mcip_lock);

	__mcip_cmd(CMD_ICI_CHECK_SOURCE, 0);
	
	senders = _lr(AUX_MCIP_READBK);	/* 1,2,4,8... */

	/* TODO: don't support interrupt coalescing yet */
	c = __ffs(senders);			/* 0,1,2,3 */
	__mcip_cmd(CMD_ICI_GENERATE_ACK, c);

	spinlock_release(&mcip_lock);

	fn = ici_callbk[smp_processor_id()];
	if (fn)
		(*fn)(IRQ_TIMER);

}
