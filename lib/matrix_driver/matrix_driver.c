#include "matrix_driver.h"

// The struct totally disabled the functionality of this thats why its not used here
// Nothing worked after we initialized it. 

// typedef struct key_descr_t
// {
// 	 volatile uint8_t* ddr;
// 	 volatile uint8_t* pin;
// 	 volatile uint8_t* port;
// 	 volatile uint8_t bit;
// } key_descr_t;

// volatile key_descr_t pins[]={
// 	{&DDRD, &PIND, &PORTD, PD7},
// 	{&DDRG, &PING, &PORTG, PG5},
// 	{&DDRB, &PINB, &PORTB, PB6},
// 	{&DDRH, &PINH, &PORTH, PH6},
// 	{&DDRE, &PINE, &PORTE, PE4},
// 	{&DDRL, &PINL, &PORTL, PL0},
// 	{&DDRH, &PINH, &PORTH, PH4},
// 	{&DDRL, &PINL, &PORTL, PL2}
// };	
		


// Used for returning chars in ascii code to caller
// x is the default output and is hopefuly ignored in the caller
const char characters[17] = { 
    'x',
    '1', '2', '3', 'A', 
    '4', '5', '6', 'B', 
    '7', '8', '9', 'C', 
    '*', '0', '#', 'D', 
};

/** 
 * Rows
 * T0   - PD7 - 1
 * OC0B - PG5 - 2
 * OC1B - PB6 - 3
 * OC2B - PH6 - 4
 * 
 * Columns 
 * OC3B - PE4 - 5
 * ICP4 - PL0 - 6
 * OC4B - PH4 - 7
 * T5   - PL2 - 8
 */

void init_matrix_keyboard(void) {

    // for(uint8_t i = 0; i<4; i++){
    //     *pins[i].ddr |= _BV(pins[i].bit);
    //     *pins[i].port |= _BV(pins[i].bit);
    // }

    // for(uint8_t i = 4 ; i< 8; i++){
    //     *pins[i].ddr &= ~(_BV(pins[i].bit));
    //     *pins[i].port |= _BV(pins[i].bit);
    // }

    {
        // Rows OUTPUT
        DDRD |= _BV(DDD7);
        DDRG |= _BV(DDG5);
        DDRB |= _BV(DDB6);
        DDRH |= _BV(DDH6);

        // Output HIGH
        PORTD |= _BV(PD7);
        PORTG |= _BV(PG5);
        PORTB |= _BV(PB6);
        PORTH |= _BV(PH6);
    }
	{
        // Columns INPUT
        DDRE &= ~(_BV(DDE4));
        DDRL &= ~(_BV(DDL0));
        DDRH &= ~(_BV(DDH4));
        DDRL &= ~(_BV(DDL2));

        // Input with pull up resistor
        PORTE |= _BV(PE4);
        PORTL |= _BV(PL0);
        PORTH |= _BV(PH4);
        PORTL |= _BV(PL2);
    }
}

// Finds the pressed key in the matrix. Max 1 key can be pressed.
// Returns a char converted to a number. x is 120 and 0 is 48
uint8_t getKey() {
    uint8_t value = 0;

    // iterate over the rows
    for(uint8_t i = 0; i<4; i++){


        // Turn off only the rw that we are looking at 

        // *(pins[i].port) &= ~(_BV(pins[i].bit));

        if(i == 0){
	        PORTD &= ~(_BV(PD7));
        }else if (i == 1){
            PORTG &= ~(_BV(PG5));
        }else if (i == 2){
            PORTB &= ~(_BV(PB6));
        }else if (i == 3){
            PORTH &= ~(_BV(PH6));
        }

        // iterate over the columns
        // for(uint8_t j = 4 ; j< 8; j++){
        //     if(~(*(pins[j].pin)) & _BV(pins[j].bit)){
        //         value = (i*4) + j + 1;
        //         break;
        //     }
        // }

        // Find which column is used
        if(~PINE & _BV(PE4)){
            value = (i*4) + 1;
        }else if(~PINL & _BV(PL0)){
            value = (i*4) + 2;
        }else if(~PINH & _BV(PH4)){
            value = (i*4) + 3;
        }else if(~PINL & _BV(PL2)){
            value = (i*4) + 4;
        }

        // *(pins[i].port) |= _BV(pins[i].bit);
        
        // Turn on the pin after using it to find the spot where the key is pressed
        if(i == 0){
	        PORTD |= _BV(PD7);
        }else if (i == 1){
            PORTG |= _BV(PG5);
        }else if (i == 2){
            PORTB |= _BV(PB6);
        }else if (i == 3){
            PORTH |= _BV(PH6);
        }
    }

    return characters[value];
}