
#include "arc.h"

// ARC port

spinlock_t tx_lock;

void interrupt_core(unsigned int core)
{
	arc_ici_send(core);
}

void enable_ints(void)
{
	arc_enable_ints();
}

void get_lock(void)
{
  spinlock_acquire(&tx_lock);
}

void release_lock(void)
{
  spinlock_release(&tx_lock);
}

void assert(void)
{
	arc_halt();
}

// tx test - prepared by Express Logic

#define TIMER_FIRE_INTERVAL	0xffff

volatile unsigned long timer_interrupt_count;
volatile unsigned long core_0_run_count;
volatile unsigned long core_0_interrupt_count;
volatile unsigned long core_1_run_count;
volatile unsigned long core_1_interrupt_count;
volatile unsigned long core_1_release_flag;

volatile unsigned long protection_owning_core = 0xFF;

void timer_interrupt_processing(int vector)
{
  timer_interrupt_count++;
}

void core_0_interrupt_handler(int vector)
{
  core_0_interrupt_count++;
  
//  if ((core_0_interrupt_count % 10) == 0)
//    interrupt_core(1);
}

void core_1_interrupt_handler(int vector)
{
  core_1_interrupt_count++;
}

void tx_primary_core_entry(void)
{
  arc_register_isr(IRQ_TIMER, &timer_interrupt_processing, NULL);
  arc_register_isr(IRQ_IPI, &core_0_interrupt_handler, NULL);

  /* set timer to count AFTER setting up ISR */
  arc_timer_setup(TIMER_FIRE_INTERVAL);

  enable_ints();
  
  /* release other core AFTER setting above otherwise we can deadlock. */
  core_1_release_flag = 1;

  while (1)
  {
    get_lock();
    
    if (protection_owning_core != 0xFF)
    {
      assert(); /* stick here - error! */
    }
    
    protection_owning_core = 0;

    interrupt_core(1);
    
    protection_owning_core = 0xFF;
    
    release_lock();
    
    core_0_run_count++;
  }
}

void tx_additional_core_entry(void)
{
  /* wait here for core 0 to get to tx_primary_core_entry */
  while (core_1_release_flag == 0)
  {
  }
  
  arc_register_isr(IRQ_IPI, &core_1_interrupt_handler, NULL);

  enable_ints();
  
  while (1)
  {
    get_lock();
    
    if (protection_owning_core != 0xFF)
    {
      assert(); /* stick here - error! */
    }
    
    protection_owning_core = 1;

    interrupt_core(0);

    protection_owning_core = 0xFF;
    
    release_lock();
    
    core_1_run_count++;
  }
}
