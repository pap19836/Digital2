//|----------------------------------------------------------------------------|
//|--------------------------------SLAVE1--------------------------------------|
//|----------------------------------------------------------------------------|
// Archivo: main.c
// Dispositivo: PIC16F887
// Autor: Stefano Papadopolo
// Compilador: XC-8 (v2.32)
//
// Programa: I2C
// Hardware: 3 PIC16f887, 2 pots, 4LEDs, LCD
//
// Creado 05 de ago, 2021
// Última Actualización: 05 de ago, 2021

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
#include<string.h>
#include"Digital2_toolbox.h"

//|----------------------------------------------------------------------------|
//|-------------------------------VARIABLES------------------------------------|
//|----------------------------------------------------------------------------|
uint8_t counter;

//|----------------------------------------------------------------------------|
//|------------------------------PROTOTYPES------------------------------------|
//|----------------------------------------------------------------------------|
void setup(void);
void __interrupt() isr(void);

//|----------------------------------------------------------------------------|
//|---------------------------------CODE---------------------------------------|
//|----------------------------------------------------------------------------|

void    main(void){
    setup();
    while(1){
        PORTD   =   counter&0x0F;       //mask counter
        
    }
}
//|----------------------------------------------------------------------------|
//|------------------------------FUNCTIONS-------------------------------------|
//|----------------------------------------------------------------------------|

void setup(void){
    //I/O Setup
    ANSEL   =   0;
    ANSELH  =   0;
    TRISA   =   0;
    TRISB   =   0b00000011;
    TRISC   =   0;
    TRISD   =   0;
    TRISE   =   0;
    
    //PORTB config
    OPTION_REGbits.nRBPU    =   0;
    WPUB    =   0b00000011;
    IOCB    =   0b00000011;

    //Interrupt config
    GIE     =   1;
    RBIE    =   1;
    RBIF    =   0;
    
   //Port Inicialization
    PORTA   =   0;
    PORTB   =   0;
    PORTC   =   0;
    PORTD   =   0;
    PORTE   =   0;
    
    //Variable Inicialization
    
}

       

//|----------------------------------------------------------------------------|
//|------------------------------INTERRUPTS------------------------------------|
//|----------------------------------------------------------------------------|
void __interrupt() isr(void){
    if(RBIF){
        if(RB0==0 && RB1==1){
            counter++;
        }
        if(RB0==1 && RB1==0){
            counter--;
        }
        RBIF    =   0;
        
    }

}

