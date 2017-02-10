OUTPUT_ARCH(arc)
ENTRY(_stext)
SECTIONS
{
    . = 0x80000000;
    _vec_tbl = .;
    .vector : { *(.vector) }

    . = ALIGN((1 << 13));
    _stext = .;
    .text : { . = ALIGN(8); *(.text.hot) *(.text) *(.ref.text) *(.text.unlikely) }
    _etext = .;

    . = ALIGN(((1 << 13))); .rodata : { *(.rodata) *(.rodata.*) } 

    . = ALIGN((1 << 13)); .data : { *(.data) }
    _edata = .;

    . = ALIGN(0); .bss : { *(.bss) *(.bss.*) } 
    _end = . ;

    . = ALIGN((1 << 13));
    .stab 0 : { *(.stab) } .stabstr 0 : { *(.stabstr) } .stab.excl 0 : { *(.stab.excl) } .stab.exclstr 0 : { *(.stab.exclstr) } .stab.index 0 : { *(.stab.index) } .stab.indexstr 0 : { *(.stab.indexstr) } .comment 0 : { *(.comment) }

    .arcextmap 0 : {
        *(.gnu.linkonce.arcextmap.*)
        *(.arcextmap.*)
     }
    .debug_aranges 0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_info 0 : { *(.debug_info) }
    .debug_abbrev 0 : { *(.debug_abbrev) }
    .debug_line 0 : { *(.debug_line) }
    .debug_str 0 : { *(.debug_str) }
    .debug_loc 0 : { *(.debug_loc) }
    .debug_macinfo 0 : { *(.debug_macinfo) }
}
