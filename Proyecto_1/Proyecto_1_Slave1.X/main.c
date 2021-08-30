//|----------------------------------------------------------------------------|
//|--------------------------------SLAVE 1-------------------------------------|
//|----------------------------------------------------------------------------|
// Archivo: main.c
// Dispositivo: PIC16F887
// Autor: Stefano Papadopolo
// Compilador: XC-8 (v2.32)
//
// Programa: Proyecto 1
// Hardware: Sensores Ultrasonicos, 3 PICS, Sensor de intensidad de luz LS2651,
// AdafruitIO, Terminal Serial
// Creado 29 de agosto, 2021
// Ultima Actualizacion: 29 de agosto, 2021

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
// Definici髇 e importaci髇 de librer韆s
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include"Digital2_toolbox.h"
//*****************************************************************************
// Definici髇 de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
uint8_t z;
uint8_t flags;       //[0] = lock, [1] = Door
int V;
int D;
//*****************************************************************************
// Definici髇 de funciones para que se puedan colocar despu閟 del main de lo 
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

        
        TMR1H = 0;
        TMR1L = 0;
        
        RB0 = 1;
        __delay_us(10);
        RB0 = 0;

        
        while(!RB1);              //Waiting for Echo goes HIGH
        TMR1ON = 1;               //Timer Starts
        while(RB1);               //Waiting for Echo goes LOW
        TMR1ON = 0;               //Timer Stops
        
        V = (TMR1L | (TMR1H<<8));
        D = V/58;
       
        if(D >= 20){
            flags   =   flags   &  0b00000010;
        }
        if(D < 20){
            flags   =   flags | 0b00000001;
        }
        
        if (RB2 ==   0){
            flags   =   flags   &   0b00000001;
            RA0 =   0;
        }
        if (RB2 ==  1){
            flags   =   flags   |   0b00000010;
            RA0 =   1;
        }
        
        PORTD   =   flags;
        
       __delay_ms(50);
    }
    return;
}
//*****************************************************************************
// Funci髇 de Inicializaci髇
//*****************************************************************************
void setup(void){
 //I/O Setup
    ANSEL   =   0;
    ANSELH  =   0;
    TRISA   =   0;
    TRISB   =   0b00000110;
    TRISC   =   0;
    TRISD   =   0;
    TRISE   =   0;
    

    //Interrupt config
    GIE     =   1;
    
    I2C_Slave_Init(0b00000000);

    //Port Inicialization
    PORTA   =   0;
    PORTB   =   0;
    PORTC   =   0;
    PORTD   =   0;
    PORTE   =   0;
    
    //Variable Inicialization
    flags    =   0;

    
}
void __interrupt() isr(void){

    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
           //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci贸n recepci贸n/transmisi贸n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci贸n se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepci贸n
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = flags;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

