#include "../include/hal/button.h"
#include <esp32-hal-gpio.h>




SmartSecurity_Esp::Button::Button(int pin){
    
    pinMode(pin, INPUT_PULLUP);
    this->_pin = pin;

}
bool SmartSecurity_Esp::Button::isPressed(){

    return !digitalRead(this->_pin);
}
bool SmartSecurity_Esp::Button::button_state(void* func, void* output){   
    if(isPressed()){
        ((void(*)(void*))func)(output);

    }
    return false;
}