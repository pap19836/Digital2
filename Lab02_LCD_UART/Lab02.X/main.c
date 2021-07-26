// Archivo: main.c
// Dispositivo: PIC16F887
// Autor: Stefano Papadopolo
// Compilador: XC-8 (v2.32)
//
// Programa: LCD & UART
// Hardware: LCD Display, 2 pot, Consola
//
// Creado 21 de jul, 2021
// Última Actualización: 21 de jul, 2021

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
uint16_t    v1;
uint16_t    v1_compare;
uint16_t    read_an1;
uint8_t     m1;
uint8_t     c1;
uint8_t     d1;
uint8_t     u1;

uint16_t    v2;
uint16_t    v2_compare;
uint16_t    read_an2;
uint8_t     m2;
uint8_t     c2;
uint8_t     d2;
uint8_t     u2;

uint8_t     counter;
uint8_t     counter_compare;
uint8_t     mc;
uint8_t     cc;
uint8_t     dc;
uint8_t     uc;

//|----------------------------------------------------------------------------|
//|------------------------------PROTOTYPES------------------------------------|
//|----------------------------------------------------------------------------|
void setup(void);
void divide(uint16_t value, uint8_t *mil, uint8_t *cent, uint8_t *dec, uint8_t *unit);

//|----------------------------------------------------------------------------|
//|---------------------------------CODE---------------------------------------|
//|----------------------------------------------------------------------------|

void    main(void){
    setup();
    Lcd_Write_String("  V1    V2    V3 ");

    
    
    while(1){
        GO  =   1;
        __delay_us(100);
        v1    =   (float)5000*(float)read_an1/(float)1024;
        v2    =   (float)5000*(float)read_an2/(float)1024;
        if(RCIF){
            if(RCREG==43){
                counter++;
            }
            if(RCREG==45){
                counter--;
            }   
        }
        
        if(v1!=v1_compare || v2!=v2_compare){
            v1_compare    =   v1;
            v2_compare    =   v2;
            divide(v1, &m1, &c1, &d1, &u1);
            divide(v2, &m2, &c2, &d2, &u2);
            //Write V1 on LCD
            Lcd_Cmd(0b11000000);
            Lcd_Write_Char(m1+48);
            Lcd_Write_String(".");
            Lcd_Write_Char(c1+48);
            Lcd_Write_Char(d1+48);
            Lcd_Write_String("V ");
            //Write V1 on Terminal
            for(uint8_t i; i<42; i++){
                UART_Write_Char(8);
            }
            UART_Write_Char(13);//Return terminal to beginning of line
            UART_Write("Voltage 1 = ");
            UART_Write_Char(m1+48);
            UART_Write(".");
            UART_Write_Char(c1+48);
            UART_Write_Char(d1+48);
            UART_Write_Char(u1+48);
            UART_Write(" V    ");
            //Write V2 on LCD
            Lcd_Write_Char(m2+48);
            Lcd_Write_String(".");
            Lcd_Write_Char(c2+48);
            Lcd_Write_Char(d2+48);
            Lcd_Write_String("V ");
            //Write V2 on terminal
            UART_Write("Voltage 2 = ");
            UART_Write_Char(m2+48);
            UART_Write(".");
            UART_Write_Char(c2+48);
            UART_Write_Char(d2+48);
            UART_Write_Char(u2+48);
            UART_Write(" V");
        }
        if(counter_compare!=counter){
            Lcd_Cmd(0b11001100);
            counter_compare =   counter;
            divide(counter,  &mc, &cc, &dc, &uc);
             //Write counter on LCD
            if(cc==0){
                Lcd_Write_String(" ");
            }
            else{
                Lcd_Write_Char(cc+48);
            }
            if(dc==0){
                Lcd_Write_String(" ");
            }
            else{
                Lcd_Write_Char(dc+48);
            }
            Lcd_Write_Char(uc+48);
        }
        
        
    }
}
//|----------------------------------------------------------------------------|
//|------------------------------FUNCTIONS-------------------------------------|
//|----------------------------------------------------------------------------|

void setup(void){
    //I/O Setup
    ANSEL   =   0b00000011;
    ANSELH  =   0;
    TRISA   =   0b00000011;
    TRISB   =   0;
    TRISC   =   0b10000000;
    TRISD   =   0;
    TRISE   =   0;

    //Interrupt config
    GIE     =   1;
    PEIE    =   1;
    ADIE    =   1;
    
    //ADC Config
    ADCON0bits.ADCS    =   0b01;    //Fosc/8
    ADCON0bits.CHS     =   0b0000;  //Select Channel 0
    ADON    =   1;                  //Enable ADC
    ADIF    =   0;
        
    Lcd_Init();
    
    UART_Init();
    
    //Port Inicialization
    PORTA   =   0;
    PORTB   =   0;
    PORTC   =   0;
    PORTD   =   0;
    PORTE   =   0;
    
    //Variable Inicialization
    read_an1    =   0;
    v1    =   0;
    read_an2    =   0;
    v2    =   0;
    
}

void divide(uint16_t value, uint8_t *mil, uint8_t *cent, uint8_t *dec, uint8_t *unit){
    *mil=value/1000;
    *cent=(value-1000*(int)*mil)/100;
    *dec=(value-1000*(int)*mil-100*(int)*cent)/10;
    *unit=value-1000*(int)*mil-100*(int)*cent-10*(int)*dec;
    
}       

//|----------------------------------------------------------------------------|
//|------------------------------INTERRUPTS------------------------------------|
//|----------------------------------------------------------------------------|
void __interrupt() isr(void){
    if(ADIF){
        if(ADCON0bits.CHS==0b0000){
            read_an1    =   readAnalog();
            ADCON0bits.CHS =   0b0001;
        }
        else if(ADCON0bits.CHS==0b0001){
            read_an2    =   readAnalog();
            ADCON0bits.CHS =   0b0000;
        }
        ADIF    =   0;
    }
}
