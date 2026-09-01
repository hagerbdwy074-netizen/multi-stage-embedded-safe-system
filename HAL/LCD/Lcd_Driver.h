#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include "STANDARD_TYPES.h"
#include "DIO_interface.h"
#include "MACROS.h"
#include <util/delay.h>

#define  lcd_mode  4

/* DEFINE CONTROL PINS */
#define control PORTA_ID
#define RS   PIN2  /* اتغيرت عشان متعملش تداخل مع الداتا */
#define EN   PIN3  /* اتغيرت عشان متعملش تداخل مع الداتا */

/* DEFINE DATA PINS (4-BIT MODE) */
#define DATA PORTA_ID
#define D7   PIN7
#define D6   PIN6
#define D5   PIN5
#define D4   PIN4

/* Commands */
#define LCD_CLR  0X01
#define RETURN_HOME 0X02
#define LCD_CURSOR_SHIFT_RIGHT 0X06
#define LCD_DISPLAY_ON_CURSOR_OFF 0x0C
#define LCD_DISPLAY_OFF_CURSOR_OFF 0X08
#define LCD_DISPLAY_ON_CURSOR_ON 0X0E
#define LCD_DISPLAY_ON_CURSOR_ON_BLINKING 0X0F
#define LCD_4BIT_MODE 0x28 
#define LCD_8BIT_MODE 0x38
#define LCD_CURSOR_FIRST_LINE 0X80
#define LCD_CURSOR_SEC_LINE 0XC0

void LCD_CMD(u8 command);
void LCD_INIT(void);
void LCD_WRITE_STRING(const u8 *data);
void LCD_WRITE_CHAR(const u8 data);
void LCD_SET_CURSOR(u8 ROW , u8 COLOM);
void LCD_WRITE_NUM(s32 num);
void LCD_CLEAR(void);

#endif