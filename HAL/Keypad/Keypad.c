#include "Keypad.h"


u8 Keypad[4][4] = { {'1','4','7','*'},
                    {'2','5','8','0'},
                    {'3','6','9','#'},
                    {'A','B','C','D'} };

void KEY_INIT() {
    for (u8 row = 0; row < ROWS; row++) {
        DIO_voidSetPinDirection(KEY_PORT, row, PIN_OUTPUT);
    }
    for (u8 col = 0; col < COLUMNS; col++) {
        DIO_voidSetPinDirection(KEY_PORT, col + 4, PIN_INPUT);
    }
    DIO_voidSetPortValue(KEY_PORT, 0xF0); // Pull-up resistors
}

void GPIO_WRITE_ROW(u8 row) {
    DIO_voidWriteLowNibble(KEY_PORT, 0x0F);
    DIO_voidSetPinValue(KEY_PORT, row, PIN_LOW);
}

u8 KEY_GETKEY() {
    for (u8 row = 0; row < ROWS; row++) {
        GPIO_WRITE_ROW(row);
        for (u8 col = 0; col < COLUMNS; col++) {
            if (DIO_u8GetPinValue(KEY_PORT, col + 4) == PIN_LOW) {
                _delay_ms(20); // Debounce
                if (DIO_u8GetPinValue(KEY_PORT, col + 4) == PIN_LOW) {
                    u8 key = Keypad[row][col];
                    while (DIO_u8GetPinValue(KEY_PORT, col + 4) == PIN_LOW);
                    return key;
                }
            }
        }
    }
    return 0;
}