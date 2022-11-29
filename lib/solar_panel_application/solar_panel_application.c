#include "./solar_panel_application.h"
#include "../segments_driver/segments_driver.h"
#include "../matrix_driver/matrix_driver.h"
#include "../temperature_driver/temperature_driver.h"
#include "../led_driver/led_driver.h"
#include "../servo_driver/servo_driver.h"
#include "../key_driver/key_driver.h"

int16_t current_temperature = 0;
int16_t low_temperature = 22;
int16_t high_temperature = 27;

// Modes:
// 0 - current temperature,
// 1 - low temperature,
// 2 - high temperature,
// 3 - low temperature enter mode,
// 4 - high temperature enter mode,
uint8_t mode = 0; 

// Modes:
// 0 - pump off
// 1 - pump on
uint8_t pumpMode = 0;

// Used to store the value when it is being entered by the user
// Calculator style input
// 20 is the default value. Because its out of reach
int8_t userInput1 = 20;
int8_t userInput2 = 20;
int8_t userInput3 = 20;
int8_t userInput4 = 20;

// Used to handle the buttons jumping between pressed and not pressed a lot. Button jitter
uint16_t previousKey = 0;

// The button has to be pressed for at least 30 cycles of the same key to activate something in the logic.
uint16_t sameKeyCount = 0;

void init_solar_panel_application(){
    init_display();
    init_matrix_keyboard();
    init_temperature_sensor();
    init_leds();
    // init_keys(); // The keys interfered somehow with the matrix so we made the keys in the matrix instead.
    init_servo();
}

void refresh_solar_panel_application(){
    char key = getKey();
    
    // Read the key before any of the events to change the mode if needed

    if(key != 'x'){

        // Leads to different modes depending on which mode you are in

        // Key has to be pressed for 30 cycles
        if(key == '1' && mode != 1 && mode != 2 && sameKeyCount == 30){
            // t low mode
            mode = 1;
            sameKeyCount = 0;
        }else if (key == '2' && mode != 1 && mode != 2 && sameKeyCount == 30){
            // t high mode
            mode = 2;
            sameKeyCount = 0;
        }else if (key == '3' && mode != 1 && mode != 2 && sameKeyCount == 30){
            // Current temperature mode
            mode = 0;
            sameKeyCount = 0;
        }

        // This was breaking the matrix in unpredictable ways for some reason. Even though the ports are completely different.
        // if(get_button(1) && mode != 1 && mode != 2 ){
        //     // t low mode
        //     mode = 1;
        //     sameKeyCount = 0;
        // }else if (get_button(2) && mode != 1 && mode != 2 ){
        //     // t high mode
        //     mode = 2;
        //     sameKeyCount = 0;
        // }else if (get_button(3) && mode != 1 && mode != 2 ){
        //     // Current temperature mode
        //     mode = 0;
        //     sameKeyCount = 0;
        // }
    }


    if(mode == 0){
        handle_mode_one(key);
        

        // There are leds that indicate the current mode that the user is in.
        // These are corrected every time the user is in the mode
        // Only the specific leds are changed. The pump led stays in the same mode that it is.
        set_led(1, 0);
        set_led(2, 0);
        set_led(3, 0);
        set_led(4, 0);

        set_led(0, 1);
    }else if(mode == 1 || mode == 2){
        handle_mode_two_three(key);

        set_led(0, 0);
        set_led(3, 0);
        set_led(4, 0);

        set_led(1, 1);
        set_led(2, 1);
        
    }else if(mode == 3){
        handle_mode_four(key);
        set_led(0, 0);
        set_led(1, 0);
        set_led(2, 0);
        set_led(4, 0);

        set_led(3, 1);
    }else if(mode == 4){
        handle_mode_five(key);
        set_led(0, 0);
        set_led(1, 0);
        set_led(2, 0);
        set_led(3, 0);

        set_led(4, 1);
    }

    // CHeck if key changes and increment key counter, to help with key jitter once keys are pressed.
    if (previousKey == key){
        if(sameKeyCount < 30){
            sameKeyCount++;
        }
    }else{
        sameKeyCount = 0;
        previousKey = key;
    }

    // Do operations based on the temperature of the air
    current_temperature = get_temperature();

    // Turn on the pump and valve(servo) if the temperature is equal to or above high temperature setting
    if(current_temperature >= high_temperature && !pumpMode){
        pumpMode = 1;
        set_led(7, 1);
        move_CW();

    // Turn of the pump and valve if temperature drops to or below low temperature settings
    }else if(current_temperature <= low_temperature && pumpMode){
        pumpMode = 0;
        set_led(7, 0);
        move_CCW();
    }
}

// This is where handling of all the modes comes in

// Mode one shows current temperature
void handle_mode_one(char key){
    set_number_4u((uint16_t)get_temperature());
}

// This edits the high and low values based on user input
void handle_mode_two_three(char key){
    // User enters number, the shown numbers are updated
    if(
        key != '*' && key != '#' && key != 'x' && key != 'A' && key != 'B' && key != 'C' && key != 'D' &&  
        sameKeyCount == 30  ){
        // inputs display as this on segments: 1, 2, 3, 4

        // THe numbers appear in calculator style
        // Entered from left to right while aligned to the right
        // THis handles the logic of such way of inputing
        if (userInput4 == 20){
            userInput4 = ((int)key) - 48; // 0 in ascii is 48, so substracting 48 gives actual integers
        }else if(userInput3 == 20){
            userInput3 = userInput4;
            userInput4 = ((int)key) - 48;
        }else if(userInput2 == 20){
            userInput2 = userInput3;
            userInput3 = userInput4;
            userInput4 = ((int)key) - 48;
        }else if(userInput1 == 20){
            userInput1 = userInput2;
            userInput2 = userInput3;
            userInput3 = userInput4;
            userInput4 = ((int)key) - 48;
        }

        
        uint16_t number_to_show = 0;

        // Combine the numbers digit by digit
        if(userInput1 != 20)
            number_to_show += userInput1*1000;
        if(userInput2 != 20)
            number_to_show += userInput2*100;
        if(userInput3 != 20)
            number_to_show += userInput3*10;
        if(userInput4 != 20)
            number_to_show += userInput4;

        // Show the entered number
        set_number_4u(number_to_show);
        // Resets the count for the key after key is read
        sameKeyCount = 0;
    }else if(previousKey != key && key != 'x' ){
        
        if (key == '*'){
            // save the number
            uint16_t number_to_show = 0;

            // Combine the numbers digit by digit
            if(userInput1 != 20)
                number_to_show += userInput1*1000;         
            if(userInput2 != 20)
                number_to_show += userInput2*100;        
            if(userInput3 != 20)
                number_to_show += userInput3*10;
            if(userInput4 != 20)
                number_to_show += userInput4;

            // Choose where to save depending on the mode
            if(mode == 1){
                low_temperature = number_to_show;
                mode = 3;
            }else if (mode == 2){
                high_temperature = number_to_show;
                mode = 4;
            }
            sameKeyCount = 0;

            // Reset the keys 
            userInput1 = 20;
            userInput2 = 20;
            userInput3 = 20;
            userInput4 = 20;

        }else if (key == '#'){
            // cancel. Discard the numbers the user inputed
            userInput1 = 20;
            userInput2 = 20;
            userInput3 = 20;
            userInput4 = 20;
            if(mode == 1){
                mode = 3;
            }else if(mode == 2){
                mode = 4;
            }
            sameKeyCount = 0;
        }
    }else{
        // If user is not pressing anything show the numbers the user inputed
        uint16_t number_to_show = 0;

        // Combine the numbers digit by digit
        if(userInput1 != 20)
            number_to_show += userInput1*1000;
        if(userInput2 != 20)
            number_to_show += userInput2*100;
        if(userInput3 != 20)
            number_to_show += userInput3*10;
        if(userInput4 != 20)
            number_to_show += userInput4;

        set_number_4u(number_to_show);
    }
}

// Show the low temperature 
void handle_mode_four(char key){
    set_number_4u(low_temperature);
}

// Show the high temperature 
void handle_mode_five(char key){
    set_number_4u(high_temperature);
}