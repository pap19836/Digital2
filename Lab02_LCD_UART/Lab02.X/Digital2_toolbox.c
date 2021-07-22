#include "Digital2_toolbox.h"
#include <stdint.h>
#include <pic.h>

uint16_t readAnalog(){
    uint16_t result = 0;
    uint8_t a;
    uint16_t b;
    a   =   ADRESL;
    b   =   ADRESH;
    result   = (a<<2)|(b>>6);
    return result;
}

uint8_t EightBitAnalog(){       //ONLY USE WITH RIGHT JUSTIFIED ADC
    uint8_t a = ADRESH;
    return a;
}