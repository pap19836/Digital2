/* 
 * File:   Digital2_toolbox.h
 * Author: stefa
 *
 * Created on 15 de julio de 2021, 10:44 PM
 */
#include <stdint.h>
#include <pic.h>

#ifndef DIGITAL2_TOOLBOX_H
#define	DIGITAL2_TOOLBOX_H

//|----------------------------------------------------------------------------|
//|----------------------------------ADC---------------------------------------|
//|----------------------------------------------------------------------------|
uint16_t readAnalog();
uint8_t EightBitAnalog();

//|----------------------------------------------------------------------------|
//|----------------------------------LCD---------------------------------------|
//|----------------------------------------------------------------------------|
//USAR HD44780 en PORTD y PORTE
#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif

#ifndef RW
#define RW RE2
#endif

void    Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Cmd(uint8_t a);
void cursor_Pos(uint8_t a);

#endif	/* DIGITAL2_TOOLBOX_H */

//|----------------------------------------------------------------------------|
//|---------------------------------UART---------------------------------------|
//|----------------------------------------------------------------------------|
void UART_Init(void);
void UART_Write(unsigned char* word);
void UART_Write_Char(uint8_t character);

//|----------------------------------------------------------------------------|
//|----------------------------------SPI---------------------------------------|
//|----------------------------------------------------------------------------|
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __SPI_H
#define	__SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
typedef enum 
{
    SPI_MASTER_OSC_DIV4  = 0b00100000,
    SPI_MASTER_OSC_DIV16 = 0b00100001,
    SPI_MASTER_OSC_DIV64 = 0b00100010,
    SPI_MASTER_TMR2      = 0b00100011,
    SPI_SLAVE_SS_EN      = 0b00100100,
    SPI_SLAVE_SS_DIS     = 0b00100101
}Spi_Type;

typedef enum
{
    SPI_DATA_SAMPLE_MIDDLE   = 0b00000000,
    SPI_DATA_SAMPLE_END      = 0b10000000
}Spi_Data_Sample;

typedef enum
{
    SPI_CLOCK_IDLE_HIGH  = 0b00010000,
    SPI_CLOCK_IDLE_LOW   = 0b00000000        
}Spi_Clock_Idle;

typedef enum
{
    SPI_IDLE_2_ACTIVE    = 0b00000000,
    SPI_ACTIVE_2_IDLE    = 0b01000000
}Spi_Transmit_Edge;


void spiInit(Spi_Type, Spi_Data_Sample, Spi_Clock_Idle, Spi_Transmit_Edge);
void spiWrite(char);
unsigned spiDataReady();
char spiRead();

#endif	/* SPI_H */
//|----------------------------------------------------------------------------|
//|------------------------------Miscelaneous----------------------------------|
//|----------------------------------------------------------------------------|
void divide(uint16_t value, uint8_t *mil, uint8_t *cent, uint8_t *dec, uint8_t *unit);