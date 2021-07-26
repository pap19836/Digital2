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
    Lcd_Cmd(0x030);
    __delay_ms(5);
    Lcd_Cmd(0x030);
    __delay_us(160);
    Lcd_Cmd(0x030);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0b00111000);        //Set Interface Length
    Lcd_Cmd(0x10);              //turn off the Display
    Lcd_Cmd(0x01);              //Clear the Display
    Lcd_Cmd(0b00000110);        //Set Cursor Move Direction
    Lcd_Cmd(0x10);              //Shit Dislpay/Move Cursor
    Lcd_Cmd(0b00001100);        //cursor, blink, Dislplay
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
    for (i = 0; a[i] != '\0'; i++){
        Lcd_Write_Char(a[i]);
    }
}

//|----------------------------------------------------------------------------|
//|---------------------------------UART---------------------------------------|
//|----------------------------------------------------------------------------|

void UART_Init(void){
    //Config Transmitter
    TXSTAbits.TXEN  =   1;      //habilitar tranmision
    TXSTAbits.SYNC  =   0;      //asincrono
    RCSTAbits.SPEN  =   1;      //enable serial port
    TXSTAbits.BRGH  =   1;      //high baud rate
    BRG16   =   0;
    SPBRGH  =   0;              //Calibrate SPBRGH:SPBRG for desired BaudRate
    SPBRG   =   25;
    
        //Config Reciever
    RCSTAbits.CREN  =   1;      //Habilitar reciever
}
    
void UART_Write(unsigned char* word){   
    while (*word != 0){                 //Loop until NULL
        TXREG = (*word);                //Send current array value pointed
        while(!TXSTAbits.TRMT);         //Make sure TSR is full (value sent)
        word++;                         //Go to next value in the array
    }
}

void UART_Write_Char(uint8_t character){
    TXREG   =   character;
    while   (!TXSTAbits.TRMT);
}