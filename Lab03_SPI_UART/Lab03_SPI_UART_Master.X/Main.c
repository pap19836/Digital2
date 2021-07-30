//|----------------------------------------------------------------------------|
//|--------------------------------MASTER--------------------------------------|
//|----------------------------------------------------------------------------|
// Archivo: main.c
// Dispositivo: PIC16F887
// Autor: Stefano Papadopolo
// Compilador: XC-8 (v2.32)
//
// Programa: SPI_UART
// Hardware: Virtual Terminal, 2 pots, 2 Pics
//
// Creado 29 de jul, 2021
// Última Actualización: 29 de jul, 2021

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
uint8_t analog0;

uint8_t analog1;

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
        __delay_ms(1);
        spiWrite(0b11111111);       //Dummy Data
        switch(RE0){
            case 0:
                analog0 =   spiRead();
                break;
            case 1:
                analog1 =   spiRead();
                break;
        }
        PORTB   =   analog0;
        PORTD   =   analog1;
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
    TRISB   =   0;
    TRISC   =   0b10010000;
    TRISD   =   0;
    TRISE   =   0b0001;

    //Interrupt config
    GIE     =   1;
    PEIE    =   1;
    ADIE    =   1;
    
    //ADC Config
    ADCON0bits.ADCS    =   0b01;    //Fosc/8
    ADCON0bits.CHS     =   0b0000;  //Select Channel 0
    ADON    =   1;                  //Enable ADC
    ADIF    =   0;
    
    UART_Init();
    
    //MSSP SPI Config
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
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

}


