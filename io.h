#ifndef _IO_H
#define _IO_H

#include "main.h"


#define LED_PORT PORTA
    #define LED_SYS  PIN0_bm
    #define LED_DIAG PIN1_bm


void io_init(void);

#endif
