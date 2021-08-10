//|----------------------------------------------------------------------------|
//|--------------------------------MASTER--------------------------------------|
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
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

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
uint16_t analog0;
uint16_t analog0_temp;

uint8_t counter=0;
uint8_t counter_temp;

uint8_t temperature_sensor;
uint8_t temperature_sensor_temp;


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
    Lcd_Write_String("  S1    S2    S3 ");

    uint8_t v0;
    uint8_t m0;
    uint8_t c0;
    uint8_t d0;
    uint8_t u0;
    
    uint8_t mc;
    uint8_t cc;
    uint8_t dc;
    uint8_t uc;
    

    uint8_t     mt;
    uint8_t     ct;
    uint8_t     dt;
    uint8_t     ut;
    
    
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0b00000001);
        analog0 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0b00000011);
        counter = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        
        
        I2C_Master_Start();
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0b10000001);
        temperature_sensor = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        if(analog0_temp !=  analog0 | counter_temp!=counter | temperature_sensor_temp  !=  temperature_sensor){
            divide(analog0, &m0, &c0, &d0, &u0);
            divide(counter, &mc, &cc, &dc, &uc);
            divide(temperature_sensor, &mt, &ct, &dt, &ut);
            analog0_temp =  analog0;
            counter_temp =  counter;
            temperature_sensor_temp =   temperature_sensor;
            //Write V1 on LCD
            Lcd_Cmd(0b11000000);
            Lcd_Write_String(" ");
            Lcd_Write_Char(c0+48);
            Lcd_Write_Char(d0+48);
            Lcd_Write_Char(u0+48);
            Lcd_Write_String("   ");
            Lcd_Write_Char(dc+48);
            Lcd_Write_Char(uc+48);
            Lcd_Write_String("   ");
            Lcd_Write_Char(ct+48);
            Lcd_Write_Char(dt+48);
            Lcd_Write_Char(ut+48);
        }
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
    TRISE   =   0;

    //Interrupt config
    GIE     =   1;
    PEIE    =   1;
    ADIE    =   1;
    
    
    Lcd_Init();
    
    I2C_Master_Init(100000);
    
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



