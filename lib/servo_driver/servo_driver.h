#pragma once

#ifndef UNIT_TEST
    #include <avr/io.h>
    #include <util/delay.h>
#else
    #include "../../extras/forced_include.h"
    #include "../../extras/include/avr/io.h"
#endif

/**
 * Uses ports:
 * 
 * PortL 4
 * 
 */

void init_servo();
void move_CCW();
void move_CW();
