#!/bin/sh

export NSIM_MULTICORE=1

# for arc_uart directly hooked up to one core
# UART_OPT="-prop=nsim_mem-dev=uart0,base=0xc0fc1000,irq=24"

# for arc_uart via ARConnect IDU hooked up to both cores
UART_OPT="-prop=nsim_mem-dev=uart0,base=0xc0fc1000,irq=0,use_connect=1"

OPTS="-cmpd=soc -nsim -av2hs -core2 -profile -Xrtc -Xtimer0 -Xtimer1 -Xlib -Xll64 -Xatomic -Xunaligned -Xmacd -mmuv4 -prop=mmu_pagesize=8192 -icache=16384,64,4,o -dcache=16384,64,2,o -prop=nsim_isa_num_actionpoints=4 -prop=nsim_isa_aps_feature=1 -prop=nsim_connect=2 -prop=nsim_connect_ici=2 -OKN img.elf"

#uart opt only needed to be given once
mdb -pset=1 -psetname=core0 -prop=ident=0x00000050 $OPTS $UART_OPT
mdb -pset=2 -psetname=core1 -prop=ident=0x00000150 $OPTS
mdb -multifiles=core0,core1 -cmpd=soc -OKN
