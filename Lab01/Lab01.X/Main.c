// Archivo: main.c
// Dispositivo: PIC16F887
// Autor: Stefano Papadopolo
// Compilador: XC-8 (v2.32)
//
// Programa: Contador, ADC con alarma referente al contador
// Hardware: 8 leds, 1 pot, 2 7seg
//
// Creado 15 jul, 2021
// Última Actualización: 15 jul, 2021

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ  4000000
#include<xc.h>
#include<pic.h>
#include<stdint.h>
#include"Digital2_toolbox.h"

//|----------------------------------------------------------------------------|
//|-------------------------------VARIABLES------------------------------------|
//|----------------------------------------------------------------------------|
uint8_t analog_voltage;
uint8_t display[17];
uint8_t low_hex;
uint8_t high_hex;
uint8_t flag_7seg;

//|----------------------------------------------------------------------------|
//|------------------------------PROTOTYPES------------------------------------|
//|----------------------------------------------------------------------------|
void setup(void);
void __interrupt() isr(void);
void reset_timer0(void);

//|----------------------------------------------------------------------------|
//|---------------------------------CODE---------------------------------------|
//|----------------------------------------------------------------------------|

void    main(void){
    setup();
    while(1){
        GO = 1; //Start ADC conversion
        __delay_us(100);
        low_hex =   analog_voltage&0b00001111;
        high_hex    =   analog_voltage>>4;
        
        switch(flag_7seg){
            case 0:
                RE0 =   1;
                RE1 =   0;
                PORTD   =   display[high_hex];
                break;
            case 1:
                RE0 =   0;
                RE1 =   1;
                PORTD   =   display[low_hex];
                break;
        }
        if(analog_voltage>PORTC){
            RE2 =   1;
        }
        else{
            RE2 =   0;
        }

    }
    
}
//|----------------------------------------------------------------------------|
//|------------------------------FUNCTIONS-------------------------------------|
//|----------------------------------------------------------------------------|

void setup(){
    //I/O Setup
    ANSEL   =   0b00000001;
    ANSELH  =   0;
    TRISA   =   0b00000001;
    TRISB   =   0b00000011;
    TRISC   =   0;
    TRISD   =   0;
    TRISE   =   0;
    
    //Config PORTB
    OPTION_REGbits.nRBPU    =   0;
    WPUB    =   0b00000011;
    IOCB    =   0b00000011;
    
    //ADC Config
    ADCON0bits.ADCS    =   0b01;    //ADC at 8MHz
    ADCON0bits.CHS    =   0b000;    //Select only channel 0
    ADCON0bits.ADON    =   1;       //ADC enable
    
    
    //Set Timer0
    INTCONbits.T0IF     =   0;
    TMR0                =   8;
    OPTION_REGbits.T0CS =   0;
    OPTION_REGbits.PSA  =   0;
    OPTION_REGbits.PS0  =   1;
    OPTION_REGbits.PS1  =   0;
    OPTION_REGbits.PS2  =   0;

    //Interrupt config
    GIE     =   1;
    RBIE    =   1;  //IOCB interrupt
    PEIE    =   1;
    ADIE    =   1;  //ADC interrupt
    T0IE    =   1;  //TMR0 interrupt
    
    //7seg display options
    display[0]  = 0b00111111;
    display[1]  = 0b00000110;
    display[2]  = 0b01011011;
    display[3]  = 0b01001111;
    display[4]  = 0b01100110;
    display[5]  = 0b01101101;
    display[6]  = 0b01111101;
    display[7]  = 0b00000111;
    display[8]  = 0b01111111;
    display[9]  = 0b01100111;
    display[10] = 0b01110111;
    display[11] = 0b01111100;
    display[12] = 0b00111001;
    display[13] = 0b01011110;
    display[14] = 0b01111001;
    display[15] = 0b01110001;
    display[16] = 0b00000000;
    
    //Port Inicialization
    PORTA   =   0;
    PORTB   =   0;
    PORTC   =   128;
    PORTD   =   0;
    PORTE   =   2;
    
    //Variable Inicialization
    flag_7seg   =   0;
}

void reset_timer0(void){
    INTCONbits.T0IF     =   0;
    TMR0                =   8;
    OPTION_REGbits.T0CS =   0;
    OPTION_REGbits.PS   =   0;
    OPTION_REGbits.PS0  =   0;
    OPTION_REGbits.PS1  =   0;
    OPTION_REGbits.PS2  =   0;
}

//|----------------------------------------------------------------------------|
//|------------------------------INTERRUPTS------------------------------------|
//|----------------------------------------------------------------------------|
void __interrupt() isr(void){
    if(RBIF){
        if(!RB0){
            PORTC++;
        }
        if(!RB1){
            PORTC--;
        }
        RBIF    =   0;
    }
    if (ADIE){
        analog_voltage  =   EightBitAnalog();
        ADIF    =   0;
    }
    if  (T0IF==1){              // Multiplexer rotate from 0 to 1
        flag_7seg ++;
        if  (flag_7seg==2){
            flag_7seg=0;
        }
        reset_timer0();
    }

}