#pragma once



#ifndef UNIT_TEST
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>
#else
    #include "../../extras/forced_include.h"
    #include "../../extras/include/avr/io.h"
    #include "../../extras/include/avr/interrupt.h"
#endif

/**
 * Uses ports:
 * 
 * PortG 5,
 * PortB 6
 * PortH 6, 4
 * PortE 4
 * PortL 0,2,3
 * 
 */

void init_matrix_keyboard();
uint8_t getKey();