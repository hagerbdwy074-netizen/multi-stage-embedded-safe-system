/*
* <<<<<<<  DIO_program.c  >>>>>>> *
* Author      : Hagar mhmd "Dibra"
* Created     : 12/6/2024
* Layer       : MCAL
* SWC         : DIO 
*/
#include "STANDARD_TYPES.h"
#include "MACROS.h"  
#include "DIO_interface.h"
#include "DIO_private.h"

/* For atomic operations */
#include <avr/interrupt.h>

/* ============================================
 *      BASIC PIN OPERATIONS - LEVEL 1
 * ============================================ */

void DIO_voidSetPinDirection(u8 portId, u8 pinId, u8 direction) {
    if (direction == PIN_OUTPUT) {
        switch (portId) {
            case PORTA_ID: SET_BIT(DDRA_REG, pinId); break;
            case PORTB_ID: SET_BIT(DDRB_REG, pinId); break;
            case PORTC_ID: SET_BIT(DDRC_REG, pinId); break;
            case PORTD_ID: SET_BIT(DDRD_REG, pinId); break;
        }
    } else if (direction == PIN_INPUT) {
        switch (portId) {
            case PORTA_ID: CLEAR_BIT(DDRA_REG, pinId); break;
            case PORTB_ID: CLEAR_BIT(DDRB_REG, pinId); break;
            case PORTC_ID: CLEAR_BIT(DDRC_REG, pinId); break;
            case PORTD_ID: CLEAR_BIT(DDRD_REG, pinId); break;
        }
    }
}

void DIO_voidSetPinValue(u8 portId, u8 pinId, u8 value) {
    if (value == PIN_HIGH) {
        switch (portId) {
            case PORTA_ID: SET_BIT(PORTA_REG, pinId); break;
            case PORTB_ID: SET_BIT(PORTB_REG, pinId); break;
            case PORTC_ID: SET_BIT(PORTC_REG, pinId); break;
            case PORTD_ID: SET_BIT(PORTD_REG, pinId); break;
        }
    } else if (value == PIN_LOW) {
        switch (portId) {
            case PORTA_ID: CLEAR_BIT(PORTA_REG, pinId); break;
            case PORTB_ID: CLEAR_BIT(PORTB_REG, pinId); break;
            case PORTC_ID: CLEAR_BIT(PORTC_REG, pinId); break;
            case PORTD_ID: CLEAR_BIT(PORTD_REG, pinId); break;
        }
    }
}

u8 DIO_u8GetPinValue(u8 portId, u8 pinId) {
    u8 value = 0;
    switch (portId) {
        case PORTA_ID: value = GET_BIT(PINA_REG, pinId); break;
        case PORTB_ID: value = GET_BIT(PINB_REG, pinId); break;
        case PORTC_ID: value = GET_BIT(PINC_REG, pinId); break;
        case PORTD_ID: value = GET_BIT(PIND_REG, pinId); break;
    }
    return value;
}

void DIO_voidTogglePin(u8 portId, u8 pinId) {
    switch (portId) {
        case PORTA_ID: TOGGLE_BIT(PORTA_REG, pinId); break;
        case PORTB_ID: TOGGLE_BIT(PORTB_REG, pinId); break;
        case PORTC_ID: TOGGLE_BIT(PORTC_REG, pinId); break;
        case PORTD_ID: TOGGLE_BIT(PORTD_REG, pinId); break;
    }
}

void DIO_voidEnablePullUp(u8 portId, u8 pinId) {
    /* Pull-up is enabled by setting PORT register when pin is input */
    switch (portId) {
        case PORTA_ID: SET_BIT(PORTA_REG, pinId); break;
        case PORTB_ID: SET_BIT(PORTB_REG, pinId); break;
        case PORTC_ID: SET_BIT(PORTC_REG, pinId); break;
        case PORTD_ID: SET_BIT(PORTD_REG, pinId); break;
    }
}

void DIO_voidDisablePullUp(u8 portId, u8 pinId) {
    switch (portId) {
        case PORTA_ID: CLEAR_BIT(PORTA_REG, pinId); break;
        case PORTB_ID: CLEAR_BIT(PORTB_REG, pinId); break;
        case PORTC_ID: CLEAR_BIT(PORTC_REG, pinId); break;
        case PORTD_ID: CLEAR_BIT(PORTD_REG, pinId); break;
    }
}

/* ============================================
 *      FULL PORT OPERATIONS - LEVEL 2
 * ============================================ */

void DIO_voidSetPortDirection(u8 portId, u8 direction) {
    switch (portId) {
        case PORTA_ID: DDRA_REG = direction; break;
        case PORTB_ID: DDRB_REG = direction; break;
        case PORTC_ID: DDRC_REG = direction; break;
        case PORTD_ID: DDRD_REG = direction; break;
    }
}

void DIO_voidSetPortValue(u8 portId, u8 value) {
    switch (portId) {
        case PORTA_ID: PORTA_REG = value; break;
        case PORTB_ID: PORTB_REG = value; break;
        case PORTC_ID: PORTC_REG = value; break;
        case PORTD_ID: PORTD_REG = value; break;
    }
}

u8 DIO_u8GetPortValue(u8 portId) {
    u8 value = 0;
    switch (portId) {
        case PORTA_ID: value = PINA_REG; break;
        case PORTB_ID: value = PINB_REG; break;
        case PORTC_ID: value = PINC_REG; break;
        case PORTD_ID: value = PIND_REG; break;
    }
    return value;
}

void DIO_voidTogglePort(u8 portId) {
    switch (portId) {
        case PORTA_ID: PORTA_REG ^= 0xFF; break;
        case PORTB_ID: PORTB_REG ^= 0xFF; break;
        case PORTC_ID: PORTC_REG ^= 0xFF; break;
        case PORTD_ID: PORTD_REG ^= 0xFF; break;
    }
}

/* ============================================
 *      NIBBLE OPERATIONS - LEVEL 3
 * ============================================ */

void DIO_voidSetNibbleDirection(u8 portId, u8 nibble, u8 direction) {
    direction &= 0x0F; /* Ensure only 4 bits */
    
    if (nibble == LOWER_NIBBLE) {
        switch (portId) {
            case PORTA_ID: 
                DDRA_REG = (DDRA_REG & 0xF0) | direction;
                break;
            case PORTB_ID: 
                DDRB_REG = (DDRB_REG & 0xF0) | direction;
                break;
            case PORTC_ID: 
                DDRC_REG = (DDRC_REG & 0xF0) | direction;
                break;
            case PORTD_ID: 
                DDRD_REG = (DDRD_REG & 0xF0) | direction;
                break;
        }
    } else if (nibble == UPPER_NIBBLE) {
        direction <<= 4; /* Shift to upper nibble */
        switch (portId) {
            case PORTA_ID: 
                DDRA_REG = (DDRA_REG & 0x0F) | direction;
                break;
            case PORTB_ID: 
                DDRB_REG = (DDRB_REG & 0x0F) | direction;
                break;
            case PORTC_ID: 
                DDRC_REG = (DDRC_REG & 0x0F) | direction;
                break;
            case PORTD_ID: 
                DDRD_REG = (DDRD_REG & 0x0F) | direction;
                break;
        }
    }
}

void DIO_voidSetNibbleValue(u8 portId, u8 nibble, u8 value) {
    value &= 0x0F; /* Ensure only 4 bits */
    
    if (nibble == LOWER_NIBBLE) {
        switch (portId) {
            case PORTA_ID: 
                PORTA_REG = (PORTA_REG & 0xF0) | value;
                break;
            case PORTB_ID: 
                PORTB_REG = (PORTB_REG & 0xF0) | value;
                break;
            case PORTC_ID: 
                PORTC_REG = (PORTC_REG & 0xF0) | value;
                break;
            case PORTD_ID: 
                PORTD_REG = (PORTD_REG & 0xF0) | value;
                break;
        }
    } else if (nibble == UPPER_NIBBLE) {
        value <<= 4;
        switch (portId) {
            case PORTA_ID: 
                PORTA_REG = (PORTA_REG & 0x0F) | value;
                break;
            case PORTB_ID: 
                PORTB_REG = (PORTB_REG & 0x0F) | value;
                break;
            case PORTC_ID: 
                PORTC_REG = (PORTC_REG & 0x0F) | value;
                break;
            case PORTD_ID: 
                PORTD_REG = (PORTD_REG & 0x0F) | value;
                break;
        }
    }
}

u8 DIO_u8GetNibbleValue(u8 portId, u8 nibble) {
    u8 value = 0;
    
    switch (portId) {
        case PORTA_ID: value = PINA_REG; break;
        case PORTB_ID: value = PINB_REG; break;
        case PORTC_ID: value = PINC_REG; break;
        case PORTD_ID: value = PIND_REG; break;
    }
    
    if (nibble == LOWER_NIBBLE) {
        value &= 0x0F;
    } else if (nibble == UPPER_NIBBLE) {
        value = (value >> 4) & 0x0F;
    }
    
    return value;
}

void DIO_voidToggleNibble(u8 portId, u8 nibble) {
    u8 mask = (nibble == LOWER_NIBBLE) ? 0x0F : 0xF0;
    
    switch (portId) {
        case PORTA_ID: PORTA_REG ^= mask; break;
        case PORTB_ID: PORTB_REG ^= mask; break;
        case PORTC_ID: PORTC_REG ^= mask; break;
        case PORTD_ID: PORTD_REG ^= mask; break;
    }
}

/* ============================================
 *     MULTI-PIN OPERATIONS - LEVEL 4
 * ============================================ */

void DIO_voidWriteHighNibble(u8 portId, u8 value) {
    DIO_voidSetNibbleValue(portId, UPPER_NIBBLE, value);
}

void DIO_voidWriteLowNibble(u8 portId, u8 value) {
    DIO_voidSetNibbleValue(portId, LOWER_NIBBLE, value);
}

/* ============================================
 *     BIT FIELD OPERATIONS - LEVEL 5
 * ============================================ */

void DIO_voidSetPinRange(u8 portId, u8 startPin, u8 endPin, u8 value) {
    u8 i;
    for (i = startPin; i <= endPin && i < 8; i++) {
        DIO_voidSetPinValue(portId, i, value);
    }
}

void DIO_voidTogglePinRange(u8 portId, u8 startPin, u8 endPin) {
    u8 i;
    for (i = startPin; i <= endPin && i < 8; i++) {
        DIO_voidTogglePin(portId, i);
    }
}

/* ============================================
 *       ATOMIC OPERATIONS - LEVEL 6
 * ============================================ */

void DIO_voidAtomicSetPin(u8 portId, u8 pinId, u8 value) {
    cli(); /* Disable interrupts */
    DIO_voidSetPinValue(portId, pinId, value);
    sei(); /* Enable interrupts */
}

void DIO_voidAtomicTogglePin(u8 portId, u8 pinId) {
    cli();
    DIO_voidTogglePin(portId, pinId);
    sei();
}

/* ============================================
 *        UTILITY FUNCTIONS - LEVEL 7
 * ============================================ */

u8 DIO_u8ReadPortWithMask(u8 portId, u8 mask) {
    u8 portValue = DIO_u8GetPortValue(portId);
    return (portValue & mask);
}

void DIO_voidWritePortWithMask(u8 portId, u8 value, u8 mask) {
    u8 currentValue = DIO_u8GetPortValue(portId);
    u8 newValue = (currentValue & ~mask) | (value & mask);
    DIO_voidSetPortValue(portId, newValue);
}