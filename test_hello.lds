ENTRY(nomain)

SECTIONS
{
    . = 0x08048000 + SIZEOF_HEADERS;

    tinytext  : { *(.text) *(.data) test_hello.o(.rodata) }

    /DISCARD/ : { *(.comment) }
}