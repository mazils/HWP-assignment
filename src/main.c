#ifndef __AVR_ATmega2560__
    #define __AVR_ATmega2560__
#endif

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../lib/solar_panel_application/solar_panel_application.h"

int main(void){
    // Initialize basic things
    sei();
    
    // initialize application library
    init_solar_panel_application();

    // Event loop
    while (1){   
        _delay_ms(20);
        refresh_solar_panel_application();
    }
    return 0;
}