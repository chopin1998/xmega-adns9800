#include "main.h"
#include "io.h"


void io_init(void)
{
    LED_PORT.DIRSET = LED_SYS | LED_DIAG;
    LED_PORT.OUTSET = LED_SYS | LED_DIAG;

    // PORTC.DIR = 0x80;
    // PORTCFG.CLKEVOUT |= PORTCFG_CLKOUT_PC7_gc;

    // PORTC.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm;
}

