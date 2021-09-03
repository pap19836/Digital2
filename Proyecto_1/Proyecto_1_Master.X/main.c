
// Archivo: main.c
// Dispositivo: PIC16F887
// Autor: Stefano Papadopolo
// Compilador: XC-8 (v2.32)
//
// Programa: Proyecto 1 Sistema de Bienvenida
// Hardware: LCD Display, UART, I2C, TSL2561 Light sensor, 3 PIC16f887, 1pot
// 2 ultrasonic sensors, 1 servo, 1 motoreductor, 1 L293D Motor Driver
//
// Creado 23 de agosto, 2021
// Ultima Actualizacion: 3 de septiembre, 2021

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ  4000000
#include <stdint.h>
#include <pic16f887.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include <stdio.h>
#include"Digital2_toolbox.h"

//|----------------------------------------------------------------------------|
//|-------------------------------VARIABLES------------------------------------|
//|----------------------------------------------------------------------------|
uint16_t light;             //light sensor variables
uint8_t light_low;
uint8_t light_high;
bool    light_flag;
uint8_t Adafruit_light;

uint8_t out_flag;           //I2C outdoor sensors
uint8_t lock;
bool keep_lock_off;
uint8_t door;
bool keep_door_open;

uint8_t in_sensor;          //I2C indoor sensor
uint8_t time;               //Time before door closes
bool close;                 //Activates closing protocol

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
    //Starting state of LCD
    Lcd_Write_String(" Lock Door Lights");
    Lcd_Cmd(0b11000000);
    Lcd_Write_String(" ON");
    Lcd_Cmd(0b11000100);
    Lcd_Write_String(" NO ");
    Lcd_Cmd(0b11001100);
    Lcd_Write_String("OFF");

    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0b01110010);   //Light sensor address write
        I2C_Master_Write(0b10000000);   //Enter CONTROL register
        I2C_Master_Write(0b00000011);   // power on
        I2C_Master_Stop();

        __delay_ms(403);
        I2C_Master_Start();
        I2C_Master_Write(0b01110010);   //Light Sensor Address Write
        I2C_Master_Write(0b10101100);   //CH0 low read
        I2C_Master_Stop();

        I2C_Master_Start();             //I2C read light sensor
        I2C_Master_Write(0b01110011);
        light_low   =   I2C_Master_Read(0);
        I2C_Master_Stop();

        I2C_Master_Start();
        I2C_Master_Write(0b01110010);   //Light Sensor Address Write
        I2C_Master_Write(0b10101100);   //CH0 high read
        I2C_Master_Stop();

        I2C_Master_Start();             //I2C read light sensor
        I2C_Master_Write(0b01110011);
        light_high   =   I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);

        light = (light_high<<8)| light_low;     //concatenate light bytes

        //I2C Recieve SlavePic 1
        I2C_Master_Start();
        I2C_Master_Write(0b00000001);
        out_flag = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        lock    =   out_flag & 0b00000001;
        door    =   out_flag & 0b00000010;

        //I2C Recieve SlavePic 2
        I2C_Master_Start();
        I2C_Master_Write(0b00000011);
        in_sensor = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);

        //SERIAL Send data
        UART_Write_Char(44);
        UART_Write_Char(keep_lock_off+48);
        UART_Write_Char(44);
        UART_Write_Char(keep_door_open+48);
        UART_Write_Char(44);
        UART_Write_Char(light_flag+48);
        UART_Write_Char(44);

        //Control lock
        if(lock != 0 && keep_lock_off == 0){
            CCPR1L  =   128;
            Lcd_Cmd(0b11000000);
            Lcd_Write_String("OFF");
            keep_lock_off = 1;
        }

        //Control Door
        if (door != 0 && keep_door_open == 0){
            Lcd_Cmd(0b11000101);
            Lcd_Write_String("OPEN");
            RD0 =   0;
            RD1 =   1;
            __delay_ms(150);
            RD1 =   0;
            keep_door_open = 1;
        }

        //Closing mechanism
        if(in_sensor!=0 && keep_lock_off && keep_door_open){
            TMR1    =   0;
            TMR1ON  =   1;
        }

        if(time>=4){// if 2s pass with in_sensor off
            time    =   0;      //Reset time
            TMR1ON  =   0;      // Turn TMR1 off
            close   =   1;      //Turn on closing flag
        }

        if(close == 1 && keep_door_open == 1 && keep_lock_off == 1){
            close = 0;
            keep_door_open = 0;
            keep_lock_off  = 0;
            Lcd_Cmd(0b11000101);
            Lcd_Write_String(" NO ");
            RD0=1;
            RD1=0;
            __delay_ms(150);
            RD0=0;
            __delay_ms(150);
            CCPR1L  =   32;
            Lcd_Cmd(0b11000000);
            Lcd_Write_String(" ON");
        }
        if(RCIF){
            Adafruit_light   =   RCREG;
        }

        //light Control
        if(Adafruit_light==0){      //Adafruit light control is "Auto" 
            if(light<500){          // Uses Sensor to turn it on or off
            Lcd_Cmd(0b11001100);
            Lcd_Write_String(" ON");
            light_flag = 1;
            RD2 =   1;
            }
            else{
            Lcd_Cmd(0b11001100);
            Lcd_Write_String(" OFF");
            light_flag = 0;
            RD2 =   0;
            }
        }
        if (Adafruit_light == 1){       //Adafruit light control is "OFF"
            Lcd_Cmd(0b11001100);
            Lcd_Write_String("OFF");
            light_flag = 0;
            RD2 =   0;
        }
        if (Adafruit_light == 2){       //Adafruit light control is "ON"
            Lcd_Cmd(0b11001100);
            Lcd_Write_String(" ON");
            light_flag = 1;
            RD2 =   1;
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
    TMR1IE  =   1;
    TMR1IF  =   0;


    //Timer1 Config
    T1CONbits.T1CKPS    =   3;      //Prescaler 1:8

    Lcd_Init();
    UART_Init();
    I2C_Master_Init(400000);

    //Configure PMW CCP1
    TRISCbits.TRISC2    =  1;//CCP2 are as inputs so they don't change in config
    PR2     =   249;    //PR2 for period of PMW
    CCP1M3  =   1;      //Activate PMW mode of CCP
    CCP1M2  =   1;
    CCPR1L  =   32;     //Start at duty cicle of 1/21ms

    TMR2IF  =   0;
    T2CON   =   3;          //turn on T2 Prescaler to 1:16
    T2CONbits.TMR2ON =  1;  //Turn on timer 2
    while(TMR2IF==0){
    }
    TRISCbits.TRISC2    =   0;

    //Port Inicialization
    PORTA   =   0;
    PORTB   =   0;
    PORTC   =   0;
    PORTD   =   0;
    PORTE   =   0;

    //Variable Inicialization
    light_high  =   0;
    light_low   =   0;
    light   =   0;
    Adafruit_light = 0;
    door    =   0;
    keep_door_open  =   0;
    lock    =   0;
    keep_lock_off   =   0;
    in_sensor   =   0;
    close       =   0;
}

//|----------------------------------------------------------------------------|
//|------------------------------INTERRUPTS------------------------------------|
//|----------------------------------------------------------------------------|
void __interrupt() isr(void){
    if(TMR1IF){
        TMR1    =   0;
        time++;
        TMR1IF  =   0;
    }
}
