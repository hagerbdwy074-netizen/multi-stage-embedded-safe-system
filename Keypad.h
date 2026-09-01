#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <util/delay.h>
#include "STANDARD_TYPES.h"
#include "DIO_interface.h"

#define ROWS        4
#define COLUMNS     4
#define KEY_PORT    PORTC_ID 

void KEY_INIT();
u8 KEY_GETKEY();

#endif