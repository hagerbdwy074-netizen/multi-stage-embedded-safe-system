#define F_CPU 8000000UL
#include <util/delay.h>
#include <string.h>

#include "STANDARD_TYPES.h"
#include "DIO_interface.h"
#include "Keypad.h"
#include "Lcd_Driver.h"

// ====== Pin definitions on PORTB ======
#define CTRL_PORT   PORTB_ID
#define IR_DETECT   PIN0
#define RED_LED     PIN1
#define BUZZER      PIN2
#define BUTTON      PIN3
#define ADMIN_PIN   PIN4
#define STEP1_LED   PIN5
#define STEP2_LED   PIN6
#define STEP3_LED   PIN7

// ====== State definitions ======
enum states {
	IDLE,
	IR_VERIFIED,
	READY_FOR_PASSWORD,
	MORSE_CODE,
	ALARM_MODE,
	SHUTDOWN_MODE,
	ADMIN_OVERRIDE,
	MISSING_COND,
	UNLOCKED
};

enum states currentState = IDLE;

// ====== Globals ======
u8 attempts = 0;
u8 morse_attempt = 0;
u8 missing_count = 0;
u8 steps_check = 0;
char correct_pas[8] = "1328";
char master_pass[8]  = "1010";

// ====== Custom Characters ======
u8 lock_closed[8] = {
	0x0E,  //  01110
	0x11,  //  10001
	0x11,  //  10001
	0x1F,  //  11111
	0x1B,  //  11011
	0x1B,  //  11011
	0x1F,  //  11111
	0x00   //  00000
};

u8 lock_opened[8] = {
	0x0E,  //  01110
	0x10,  //  10000
	0x10,  //  10000
	0x1F,  //  11111
	0x1B,  //  11011
	0x1B,  //  11011
	0x1F,  //  11111
	0x00   //  00000
};

// ====== Helper Functions ======
void LCD_Store_Custom_Chars() {
	u8 i;
	LCD_CMD(0x40);
	for(i = 0; i < 8; i++) {
		LCD_WRITE_CHAR(lock_closed[i]);
	}
	
	LCD_CMD(0x48);
	for(i = 0; i < 8; i++) {
		LCD_WRITE_CHAR(lock_opened[i]);
	}
	
	LCD_CMD(0x80);
}

void Update_Steps_LEDs(u8 step) {
	DIO_voidSetPinValue(CTRL_PORT, STEP1_LED, (step >= 1) ? PIN_HIGH : PIN_LOW);
	DIO_voidSetPinValue(CTRL_PORT, STEP2_LED, (step >= 2) ? PIN_HIGH : PIN_LOW);
	DIO_voidSetPinValue(CTRL_PORT, STEP3_LED, (step >= 3) ? PIN_HIGH : PIN_LOW);
}

u8 Check_IR() {
	if (DIO_u8GetPinValue(CTRL_PORT, IR_DETECT) == PIN_LOW) {
		_delay_ms(50);
		
		if (DIO_u8GetPinValue(CTRL_PORT, IR_DETECT) == PIN_LOW) {
			while (DIO_u8GetPinValue(CTRL_PORT, IR_DETECT) == PIN_LOW);
			return 1;
		}
	}
	return 0;
}

u8 Check_Morse() {
	u8 pattern[2]; 
	u8 count = 0;
	u16 hold = 0;
	u16 timer = 0;

	while (timer < 500) { 
		if (DIO_u8GetPinValue(CTRL_PORT, BUTTON) == PIN_HIGH) {
			hold = 0;
			
			
			while (DIO_u8GetPinValue(CTRL_PORT, BUTTON) == PIN_HIGH) {
				_delay_ms(10);
				hold++;
			}

			

			if (hold < 35) {
				pattern[count] = 0;
				} else {
				pattern[count] = 1;
			}
			count++;
			
		
			if (count == 2) {
				break;
			}
			
			_delay_ms(200); 
		}
		_delay_ms(10);
		timer++;
	}

	
	if (count == 2 && pattern[0] == 0 && pattern[1] == 1) {
		return 1;
	}
	return 0; 
}

u8 Check_Password(char* correct) {
	char entered[5];
	u8 i = 0;
	u8 key;

	while (i < 4) {
		key = KEY_GETKEY();
		if (key != 0) {
			entered[i] = key;
			LCD_SET_CURSOR(1, i);
			LCD_WRITE_CHAR('*');
			i++;
			_delay_ms(200);
		}
	}
	entered[4] = '\0';

	if (strcmp(entered, correct) == 0) return 1;
	return 0;
}

// ====== STATES ======

void State_Idle() {
	LCD_SET_CURSOR(0, 0);
	LCD_WRITE_STRING((u8*)"System Idle     ");
	LCD_SET_CURSOR(1, 0);
	LCD_WRITE_STRING((u8*)"                ");

	steps_check = 0;
	Update_Steps_LEDs(steps_check);
	attempts = 0;

	if (Check_IR()) {
		steps_check = 1;
		Update_Steps_LEDs(steps_check);
		currentState = IR_VERIFIED;
		return;
	}

	if (KEY_GETKEY() != 0) {
		currentState = MISSING_COND;
		return;
	}
}

void State_IR_Verified() {
	u16 timeout = 0;
	LCD_CLEAR();
	LCD_WRITE_STRING((u8*)"IR Verified!");
	LCD_SET_CURSOR(1, 0);
	LCD_WRITE_STRING((u8*)"Enter Password");

	while (timeout < 600) {
		if (KEY_GETKEY() != 0) {
			currentState = READY_FOR_PASSWORD;
			return;
		}
		_delay_ms(10);
		timeout++;
	}
	currentState = IDLE;
}

void State_Ready_Password() {
	LCD_CLEAR();
	LCD_WRITE_CHAR(0); 
	LCD_WRITE_STRING((u8*)" Password:");

	if (Check_Password(correct_pas)) {
		attempts = 0;
		steps_check = 2;
		Update_Steps_LEDs(steps_check);
		currentState = MORSE_CODE;
		} else {
		attempts++;
		LCD_CLEAR();
		LCD_WRITE_STRING((u8*)"Access Denied!");
		_delay_ms(1000);
		if (attempts >= 2) {
			currentState = ALARM_MODE;
			} else {
			currentState = IDLE;
		}
	}
}

void State_Morse_Verified() {
	u16 timeout = 0;
	LCD_CLEAR();
	LCD_WRITE_STRING((u8*)"Password OK!");
	LCD_SET_CURSOR(1, 0);
	LCD_WRITE_STRING((u8*)"Enter Morse...");

	while (timeout < 600) {
		if (DIO_u8GetPinValue(CTRL_PORT, BUTTON) == PIN_HIGH) {
			if (Check_Morse()) {
				steps_check = 3;
				Update_Steps_LEDs(steps_check);
				DIO_voidSetPinValue(CTRL_PORT, BUZZER, PIN_HIGH);
				_delay_ms(1000);
				DIO_voidSetPinValue(CTRL_PORT, BUZZER, PIN_LOW);
				
				currentState = UNLOCKED;
				return;
				} else {
				morse_attempt++;
				LCD_CLEAR();
				LCD_WRITE_STRING((u8*)"Wrong Code!");
				_delay_ms(1000);
				if (morse_attempt >= 2) {
					currentState = ALARM_MODE;
					return;
				}
				break;
			}
		}
		_delay_ms(10);
		timeout++;
	}
	currentState = IDLE;
}

void State_Unlocked() {
	u16 auto_lock = 0;
	LCD_CLEAR();
	LCD_WRITE_CHAR(1); 
	LCD_WRITE_STRING((u8*)" UNLOCKED!");
	LCD_SET_CURSOR(1, 0);
	LCD_WRITE_STRING((u8*)"Press * to lock");

	while (1) {
		if (KEY_GETKEY() == '*') {
			currentState = IDLE;
			return;
		}
		_delay_ms(10);
		auto_lock++;
		if (auto_lock >= 1200) {
			currentState = IDLE;
			return;
		}
	}
}

void State_Alarm() {
	s8 i;
	LCD_CLEAR();
	LCD_WRITE_STRING((u8*)"!! ALARM !!");
	attempts = 0;
	morse_attempt = 0;

	for (i = 20; i > 0; i--) {
		LCD_SET_CURSOR(1, 0);
		LCD_WRITE_STRING((u8*)"Time left: ");
		LCD_WRITE_NUM(i);
		LCD_WRITE_STRING((u8*)"s  ");

		DIO_voidSetPinValue(CTRL_PORT, RED_LED, PIN_HIGH);
		DIO_voidSetPinValue(CTRL_PORT, BUZZER, PIN_HIGH);
		_delay_ms(500);
		DIO_voidSetPinValue(CTRL_PORT, BUZZER, PIN_LOW);
		_delay_ms(500);

		if (Check_IR()) {
			if (Check_Morse()) {
				DIO_voidSetPinValue(CTRL_PORT, RED_LED, PIN_LOW);
				DIO_voidSetPinValue(CTRL_PORT, BUZZER, PIN_LOW);
				currentState = READY_FOR_PASSWORD;
				return;
			}
		}
	}

	currentState = SHUTDOWN_MODE;
}

void State_Shutdown() {
	LCD_CLEAR();
	LCD_WRITE_STRING((u8*)"SHUTDOWN!");
	LCD_SET_CURSOR(1, 0);
	LCD_WRITE_STRING((u8*)"Use master key");

	DIO_voidSetPinValue(CTRL_PORT, RED_LED, PIN_HIGH);
	DIO_voidSetPinValue(CTRL_PORT, BUZZER, PIN_HIGH);

	while (DIO_u8GetPinValue(CTRL_PORT, ADMIN_PIN) == PIN_LOW);

	DIO_voidSetPinValue(CTRL_PORT, RED_LED, PIN_LOW);
	DIO_voidSetPinValue(CTRL_PORT, BUZZER, PIN_LOW);
	currentState = ADMIN_OVERRIDE;
}

void State_Admin_Override() {
	LCD_CLEAR();
	
	LCD_WRITE_STRING((u8*)" Admin Password:"); 

	if (Check_Password(master_pass)) {
		LCD_CLEAR();
		LCD_WRITE_STRING((u8*)"Access Granted");
		_delay_ms(1000);
		currentState = IDLE;
		} else {
		LCD_CLEAR();
		LCD_WRITE_STRING((u8*)"Wrong Code!");
		_delay_ms(1000);
		currentState = SHUTDOWN_MODE;
	}
}

void State_Missing() {
	LCD_CLEAR();
	LCD_WRITE_STRING((u8*)"Access Denied!");
	_delay_ms(1000);
	missing_count++;
	if (missing_count >= 2) {
		missing_count = 0;
		currentState = ALARM_MODE;
		} else {
		currentState = IDLE;
	}
}

// ====== MAIN ======
int main() {
	// PORTB (Control)
	DIO_voidSetPinDirection(CTRL_PORT, IR_DETECT, PIN_INPUT);
	DIO_voidSetPinDirection(CTRL_PORT, BUTTON,    PIN_INPUT);
	DIO_voidSetPinDirection(CTRL_PORT, ADMIN_PIN, PIN_INPUT);
	
	DIO_voidSetPinDirection(CTRL_PORT, RED_LED,   PIN_OUTPUT);
	DIO_voidSetPinDirection(CTRL_PORT, BUZZER,    PIN_OUTPUT);
	DIO_voidSetPinDirection(CTRL_PORT, STEP1_LED, PIN_OUTPUT);
	DIO_voidSetPinDirection(CTRL_PORT, STEP2_LED, PIN_OUTPUT);
	DIO_voidSetPinDirection(CTRL_PORT, STEP3_LED, PIN_OUTPUT);

	LCD_INIT();
	LCD_Store_Custom_Chars();
	KEY_INIT();

	LCD_CLEAR();
	LCD_WRITE_STRING((u8*)"Safe System ");
	LCD_SET_CURSOR(1, 0);
	LCD_WRITE_STRING((u8*)"Starting...");
	_delay_ms(1000);

	while (1) {
		switch (currentState) {
			case IDLE:               State_Idle();            break;
			case IR_VERIFIED:        State_IR_Verified();     break;
			case READY_FOR_PASSWORD: State_Ready_Password();  break;
			case MORSE_CODE:         State_Morse_Verified();  break;
			case UNLOCKED:           State_Unlocked();        break;
			case ALARM_MODE:         State_Alarm();           break;
			case SHUTDOWN_MODE:      State_Shutdown();        break;
			case ADMIN_OVERRIDE:     State_Admin_Override();  break;
			case MISSING_COND:       State_Missing();         break;
		}
	}
	return 0;
}