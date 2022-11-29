#pragma once

#ifndef UNIT_TEST
    #include <avr/io.h>
    #include <avr/interrupt.h>
#else
    #include "../../extras/forced_include.h"
    #include "../../extras/include/avr/io.h"
    #include "../../extras/include/avr/interrupt.h"
#endif

/**
 * Uses ports:
 * 
 * PortA 0-7
 * 
 */

void init_solar_panel_application();
void refresh_solar_panel_application();
