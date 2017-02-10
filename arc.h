#ifndef __ARC_HDR__
#define __ARC_HDR__

#ifndef NULL
#define NULL 0
#endif

#define _lr(reg)	__builtin_arc_lr(reg)
#define _sr(data, reg)	__builtin_arc_sr(data, reg)

#define IRQ_TIMER	16
#define IRQ_IPI		19

static inline int __ffs(unsigned long x)
{
	int n;

	asm volatile(
	"	ffs.f	%0, %1		\n"  /* 0..31; Z set if src 0 */
	"	mov.z	%0, 0		\n"  /* return 0 if 0 */
	: "=r"(n)
	: "r"(x)
	: "cc");

	return n;
}

#define AUX_ID		0x004

static inline int smp_processor_id()
{
	unsigned int id = _lr(AUX_ID);
	return (id >> 8) & 0xFF;
}

static inline void arc_halt(void)
{
  asm volatile("flag 1\n");
}

/* no need for volatile */
typedef unsigned int spinlock_t;

static inline void spinlock_acquire(unsigned int *lock)
{
	unsigned int val;

	asm volatile(
	"1:	llock	%[val], [%[slock]]	\n"
	"	breq	%[val], %[LOCKED], 1b	\n"	/* spin while LOCKED */
	"	scond	%[LOCKED], [%[slock]]	\n"	/* acquire */
	"	bnz	1b			\n"
	"	dmb 3				\n"
	: [val]		"=&r"	(val)
	: [slock]	"r"	(lock),
	  [LOCKED]	"r"	(1)
	: "memory", "cc");
}

static inline void spinlock_release(unsigned int *lock)
{
	__asm__ __volatile__(
	"	dmb 3				\n"
	"	st	%[UNLOCKED], [%[slock]]	\n"
	:
	: [slock]	"r"	(lock),
	  [UNLOCKED]	"r"	(0)
	: "memory");
}

static inline void arc_enable_ints(void)
{
	asm volatile ("seti");
}

extern void arc_timer_setup(unsigned int);
extern void arc_register_isr(int irq, void (*fn)(int), int arg_to_isr);
extern void arc_ici_send(unsigned int cpu);

#endif
