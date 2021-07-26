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