#pragma once

#ifndef UNIT_TEST
    #include <avr/io.h>
#else
    #include "../../extras/forced_include.h"
    #include "../../extras/include/avr/io.h"
#endif

/**
 * Uses ports:
 * 
 * PortC 0-5
 * 
 */

void init_keys();
uint8_t get_key_status(uint8_t key_no); // key_no [1..6], return number if it is pressed
uint8_t read_keys();
