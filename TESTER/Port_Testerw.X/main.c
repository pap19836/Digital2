
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definici�n e importaci�n de librer�as
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include <stdio.h>
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************

//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void __interrupt() isr(void);

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void setup(void){
 //I/O Setup
    ANSEL   =   0;
    ANSELH  =   0;
    TRISA   =   0;
    TRISB   =   0;
    TRISC   =   0;
    TRISD   =   0;
    TRISE   =   0;

    //Port Inicialization
    PORTA   =   0xFF;
    PORTB   =   0xFF;
    PORTC   =   0xFF;
    PORTD   =   0xFF;
    PORTE   =   0xFF;

}
void __interrupt() isr(void){

}



