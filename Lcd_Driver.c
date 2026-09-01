/************************************/
/*
@IEEE TEAM
*/
/***********************************/
#include "Lcd_Driver.h"

void LCD_CMD(u8 command)
{
	DIO_voidSetPinValue(control,RS,PIN_LOW);                   /* RESET REGISTER SELECT PIN */
	DIO_voidSetPinValue(control,EN,PIN_HIGH);                   /* SET ENABLE PIN */
	_delay_ms(1) ;                                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	
	#if (lcd_mode == 8 )                                /* CONFG IF YOUR MODE IS 8 BIT MODE */
	DIO_voidSetPortValue(DATA,command) ;                 /* SEND COMMAND ON DATA PORT */
    _delay_ms(1) ;                                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	
	#elif (lcd_mode == 4 )                              /* CONFG IF YOUR MODE IS 4 BIT MODE */
		DIO_voidSetPinValue(DATA,D7,GET_BIT(command,7)); /* SEND BIT 7 FROM COMMAND */ 
		DIO_voidSetPinValue(DATA,D6,GET_BIT(command,6)); /* SEND BIT 6 FROM COMMAND */ 
		DIO_voidSetPinValue(DATA,D5,GET_BIT(command,5)); /* SEND BIT 5 FROM COMMAND */ 
		DIO_voidSetPinValue(DATA,D4,GET_BIT(command,4)); /* SEND BIT 4 FROM COMMAND */ 
        DIO_voidSetPinValue(control,EN,PIN_LOW);               /* CLEAR ENABLE PIN */
		_delay_ms(1);                               /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
		DIO_voidSetPinValue(control,EN,PIN_HIGH);               /* SET ENABLE PIN */
    	DIO_voidSetPinValue(DATA,D7,GET_BIT(command,3)); /* SEND BIT 3 FROM COMMAND */ 
    	DIO_voidSetPinValue(DATA,D6,GET_BIT(command,2)); /* SEND BIT 2 FROM COMMAND */ 
    	DIO_voidSetPinValue(DATA,D5,GET_BIT(command,1)); /* SEND BIT 1 FROM COMMAND */ 
    	DIO_voidSetPinValue(DATA,D4,GET_BIT(command,0)); /* SEND BIT 0 FROM COMMAND */ 
	#endif
	
	DIO_voidSetPinValue(control,EN,PIN_LOW);                   /* REST EN */ 
	_delay_ms(30) ;                                 /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
}

void LCD_INIT(void)
{
    DIO_voidSetPinDirection(control,RS,PIN_OUTPUT); /* INIT CONTROL PINS AS OUTPUT */
    DIO_voidSetPinDirection(control,EN,PIN_OUTPUT); /* INIT CONTROL PINS AS OUTPUT */

	#if (lcd_mode == 8) 				  /* CONFG IF YOUR MODE IS 8 BIT MODE */
	DIO_voidSetPortDirection(DATA,PORT_OUTPUT);      /* INIT DATA PORT AS OUTPUT */
	_delay_ms(20);                    /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	LCD_CMD(RETURN_HOME);             /* COMMAND TO INIT LCD */
	LCD_CMD(LCD_8BIT_MODE) ;          /* SEND 8BIT MODE COMMAND FOR LCD */ 
	_delay_ms(1) ;                    /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */

	#elif (lcd_mode == 4)                 /* CONFG IF YOUR MODE IS 4 BIT MODE */
	DIO_voidSetPinDirection(DATA,D7,PIN_OUTPUT);    /* SETUP DATA PINS AS OUTPUT D7 */
	DIO_voidSetPinDirection(DATA,D6,PIN_OUTPUT);    /* SETUP DATA PINS AS OUTPUT D6 */
	DIO_voidSetPinDirection(DATA,D5,PIN_OUTPUT);    /* SETUP DATA PINS AS OUTPUT D5 */
	DIO_voidSetPinDirection(DATA,D4,PIN_OUTPUT);    /* SETUP DATA PINS AS OUTPUT D4 */
	_delay_ms(20);                    /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	LCD_CMD(RETURN_HOME);             /* COMMAND TO INIT LCD */
	LCD_CMD(LCD_4BIT_MODE);           /* COMMAND TO MAKE LCD CNFG AS 4BIT MODE */
	#endif
	
	LCD_CMD(0x0f);                    /* SEND DISPLAY ON COMMAND FOR LCD */ 
	LCD_CMD(LCD_CLR) ;                /* SEND CLEAR COMMAND FOR LCD */ 
	LCD_CMD(LCD_CURSOR_SHIFT_RIGHT);  /* COMMAND TO SHIFT CURSOR */
	_delay_ms(50) ;                   /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
}

void LCD_WRITE_STRING(const u8 *data)
{
	 for( u8 i=0; data[i]!='\0' ; i++ )
	 {
	  DIO_voidSetPinValue(control,RS,PIN_HIGH);   /* RESET REGISTER SELECT PIN */
	  DIO_voidSetPinValue(control,EN,PIN_HIGH);   /* SET ENABLE PIN */
      _delay_ms(1) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	  
      #if (lcd_mode == 8 )				  /* CONFG IF YOUR MODE IS 8 BIT MODE */
      DIO_voidSetPortValue(DATA,data[i]) ; /* SEND COMMAND ON DATA PORT */
      _delay_ms(1) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
      
	  #elif (lcd_mode == 4 )			  /* CONFG IF YOUR MODE IS 4 BIT MODE */
      DIO_voidSetPinValue(DATA,D7,GET_BIT(data[i],7)); /* SEND BIT 7 FROM data */ 
      DIO_voidSetPinValue(DATA,D6,GET_BIT(data[i],6)); /* SEND BIT 6 FROM data */ 
      DIO_voidSetPinValue(DATA,D5,GET_BIT(data[i],5)); /* SEND BIT 5 FROM data */ 
      DIO_voidSetPinValue(DATA,D4,GET_BIT(data[i],4)); /* SEND BIT 4 FROM data */ 
      DIO_voidSetPinValue(control,EN,PIN_LOW);   /* CLEAR ENABLE PIN */
      _delay_ms(1);                   /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
      DIO_voidSetPinValue(control,EN,PIN_HIGH);   /* SET ENABLE PIN */
      DIO_voidSetPinValue(DATA,D7,GET_BIT(data[i],3)); /* SEND BIT 3 FROM data */ 
      DIO_voidSetPinValue(DATA,D6,GET_BIT(data[i],2)); /* SEND BIT 2 FROM data */ 
      DIO_voidSetPinValue(DATA,D5,GET_BIT(data[i],1)); /* SEND BIT 1 FROM data */ 
      DIO_voidSetPinValue(DATA,D4,GET_BIT(data[i],0)); /* SEND BIT 0 FROM data */ 
      #endif
	  
      DIO_voidSetPinValue(control,EN,PIN_LOW);               /* REST EN */
      _delay_ms(30) ;                             /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	 }
}

void LCD_WRITE_CHAR(const u8 data)
{	 
	DIO_voidSetPinValue(control,RS,PIN_HIGH);   /* RESET REGISTER SELECT PIN */
	DIO_voidSetPinValue(control,EN,PIN_HIGH);   /* SET ENABLE PIN */
	_delay_ms(1) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	
	#if (lcd_mode == 8 )
		DIO_voidSetPortValue(DATA,data) ;    /* SEND COMMAND ON DATA PORT */
		_delay_ms(1) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	
	#elif (lcd_mode == 4 )
		DIO_voidSetPinValue(DATA,D7,GET_BIT(data,7)); /* SEND BIT 7 FROM data */
		DIO_voidSetPinValue(DATA,D6,GET_BIT(data,6)); /* SEND BIT 6 FROM data */
		DIO_voidSetPinValue(DATA,D5,GET_BIT(data,5)); /* SEND BIT 5 FROM data */
		DIO_voidSetPinValue(DATA,D4,GET_BIT(data,4)); /* SEND BIT 4 FROM data */
		DIO_voidSetPinValue(control,EN,PIN_LOW);            /* CLEAR ENABLE PIN */
		_delay_ms(1); 							 /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
		DIO_voidSetPinValue(control,EN,PIN_HIGH);            /* SET ENABLE PIN */
		DIO_voidSetPinValue(DATA,D7,GET_BIT(data,3)); /* SEND BIT 3 FROM data */
		DIO_voidSetPinValue(DATA,D6,GET_BIT(data,2)); /* SEND BIT 2 FROM data */
		DIO_voidSetPinValue(DATA,D5,GET_BIT(data,1)); /* SEND BIT 1 FROM data */
		DIO_voidSetPinValue(DATA,D4,GET_BIT(data,0)); /* SEND BIT 0 FROM data */
	#endif
	
	DIO_voidSetPinValue(control,EN,PIN_LOW);    /* REST EN */
	_delay_ms(30) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
}

void LCD_WRITE_NUM(s32 num) 
{
	char data[11] ; 
	ltoa(num,data,10); /* Changed to ltoa to support 32bit safely */
	for( u8 i=0; data[i]!='\0' ; i++ )
	{
		 DIO_voidSetPinValue(control,RS,PIN_HIGH);   /* RESET REGISTER SELECT PIN */
		 DIO_voidSetPinValue(control,EN,PIN_HIGH);   /* SET ENABLE PIN */
		 _delay_ms(1) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
		 
		 #if (lcd_mode == 8 )
		 DIO_voidSetPortValue(DATA,data[i]) ; /* SEND COMMAND ON DATA PORT */
		 _delay_ms(1) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
		 
		 #elif (lcd_mode == 4 )
		 DIO_voidSetPinValue(DATA,D7,GET_BIT(data[i],7));
		 DIO_voidSetPinValue(DATA,D6,GET_BIT(data[i],6));
		 DIO_voidSetPinValue(DATA,D5,GET_BIT(data[i],5));
		 DIO_voidSetPinValue(DATA,D4,GET_BIT(data[i],4));
		 DIO_voidSetPinValue(control,EN,PIN_LOW);   /* CLEAR ENABLE PIN */
		 _delay_ms(1);
		 DIO_voidSetPinValue(control,EN,PIN_HIGH);   /* SET ENABLE PIN */
		 DIO_voidSetPinValue(DATA,D7,GET_BIT(data[i],3));
		 DIO_voidSetPinValue(DATA,D6,GET_BIT(data[i],2));
		 DIO_voidSetPinValue(DATA,D5,GET_BIT(data[i],1));
		 DIO_voidSetPinValue(DATA,D4,GET_BIT(data[i],0));
		 #endif
		 
		 DIO_voidSetPinValue(control,EN,PIN_LOW);   /* REST EN */
		 _delay_ms(30) ;                  /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	}
}

void LCD_CLEAR(void)
{
	LCD_CMD(LCD_CLR);                /* CLEAR LCD */
	_delay_ms(10);                   /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
	LCD_CMD(LCD_CURSOR_FIRST_LINE);  /* SEND COMMAND TO START FROM THE FIRST LINE AGAIN */
    _delay_ms(10);                   /* SMALL DELAY WAIT FOR LCD TO PROCESS DATA */
}

void LCD_SET_CURSOR(u8 ROW , u8 COLOM)
{
	if (ROW == 0)
	{
		LCD_CMD(COLOM | LCD_CURSOR_FIRST_LINE) ;
	}
	else if (ROW == 1)
	{
		LCD_CMD(COLOM | LCD_CURSOR_SEC_LINE) ;
	}
	LCD_CMD(LCD_DISPLAY_ON_CURSOR_OFF) ;
}