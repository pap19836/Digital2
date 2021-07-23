#include "Digital2_toolbox.h"
#include <stdint.h>
#include <pic.h>
#define _XTAL_FREQ  4000000

uint16_t readAnalog(){
    uint16_t result = 0;
    uint16_t a;
    uint16_t b;
    a   =   ADRESL>>6;
    b   =   ADRESH<<2;
    result   = b|a;
    return result;
}

uint8_t EightBitAnalog(){       //ONLY USE WITH RIGHT JUSTIFIED ADC
    uint8_t a = ADRESH;
    return a;
}
//|----------------------------------------------------------------------------|
//|----------------------------------LCD---------------------------------------|
//|----------------------------------------------------------------------------|

void Lcd_Cmd(uint8_t a) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    PORTD   =   a;
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}


void Lcd_Init(void) {
    PORTD   =   0;
    __delay_ms(20);
    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_us(160);
    Lcd_Cmd(0x03);
    /////////////////////////////////////////////////////
    PORTD   =   0b00111100;     //interface Lenght
    PORTD   =   0x10;           //Turn off Display
    PORTD   =   0x01;           //Clear Display
    PORTD   =   0x07;           //cursor Move direction
    PORTD   =   0x0F;           //Turn on Displa/cursor
    
    __delay_ms(100);
}

void Lcd_Write_Char(char a) {
    PORTD   =   a;
    RS = 1; // => RS = 1    
    EN = 1;
    __delay_us(100);
    EN = 0;
    __delay_ms(10);
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}