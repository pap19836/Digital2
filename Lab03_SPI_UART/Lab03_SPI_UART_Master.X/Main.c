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
uint8_t analog0temp;

uint8_t analog1;
uint8_t analog1temp;

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
    uint16_t    v1;
    uint8_t     m1;
    uint8_t     c1;
    uint8_t     d1;
    uint8_t     u1;

    uint16_t    v0;
    uint8_t     m0;
    uint8_t     c0;
    uint8_t     d0;
    uint8_t     u0;
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

        __delay_ms(1);
        if(analog0!=analog0temp || analog1!=analog1temp){
            analog0temp =   analog0;
            analog1temp =   analog1;
            v0    =   (float)5000*(float)analog0/(float)255;
            v1    =   (float)5000*(float)analog1/(float)255;
            divide(v0, &m0, &c0, &d0, &u0);
            divide(v1, &m1, &c1, &d1, &u1);
            UART_Write_Char(m0+48);
            UART_Write(".");
            UART_Write_Char(c0+48);
            UART_Write_Char(d0+48);
            UART_Write_Char(u0+48);
            UART_Write(" V,");
            
            UART_Write_Char(m1+48);
            UART_Write(".");
            UART_Write_Char(c1+48);
            UART_Write_Char(d1+48);
            UART_Write_Char(u1+48);
            UART_Write(" V");
            
        }
        
        if(RCIF){
            counter =   RCREG;
        }
        PORTB   =   counter;
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


