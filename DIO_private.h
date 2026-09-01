/*
* <<<<<<<  DIO_program.c  >>>>>>> *
* Author      : Hagar mhmd "Dibra"
* Created     : 12/6/2024
* Layer       : MCAL
* SWC         : DIO 
*/

#ifndef DIO_PRIVATE_H
#define DIO_PRIVATE_H

#include "STANDARD_TYPES.h"

/* ============================================
 *          PORTA REGISTER ADDRESSES
 * ============================================ */
#define PORTA_REG   *((volatile u8*)0x3B)  /* Port A Data Register */
#define DDRA_REG    *((volatile u8*)0x3A)  /* Port A Data Direction Register */
#define PINA_REG    *((volatile u8*)0x39)  /* Port A Input Pins Address */

/* ============================================
 *          PORTB REGISTER ADDRESSES
 * ============================================ */
#define PORTB_REG   *((volatile u8*)0x38)  /* Port B Data Register */
#define DDRB_REG    *((volatile u8*)0x37)  /* Port B Data Direction Register */
#define PINB_REG    *((volatile u8*)0x36)  /* Port B Input Pins Address */

/* ============================================
 *          PORTC REGISTER ADDRESSES
 * ============================================ */
#define PORTC_REG   *((volatile u8*)0x35)  /* Port C Data Register */
#define DDRC_REG    *((volatile u8*)0x34)  /* Port C Data Direction Register */
#define PINC_REG    *((volatile u8*)0x33)  /* Port C Input Pins Address */

/* ============================================
 *          PORTD REGISTER ADDRESSES
 * ============================================ */
#define PORTD_REG   *((volatile u8*)0x32)  /* Port D Data Register */
#define DDRD_REG    *((volatile u8*)0x31)  /* Port D Data Direction Register */
#define PIND_REG    *((volatile u8*)0x30)  /* Port D Input Pins Address */

/* ============================================
 *        INTERNAL HELPER MACROS (PRIVATE)
 * ============================================ */

/*
 * Macro: NIBBLE_MASK_LOW
 * Description: Mask for lower nibble (bits 0-3)
 */
#define NIBBLE_MASK_LOW     0x0F

/*
 * Macro: NIBBLE_MASK_HIGH
 * Description: Mask for upper nibble (bits 4-7)
 */
#define NIBBLE_MASK_HIGH    0xF0

/*
 * Macro: NIBBLE_SHIFT
 * Description: Number of bits to shift for upper nibble
 */
#define NIBBLE_SHIFT        4

/*
 * Macro: PORT_MASK_ALL
 * Description: Mask for all 8 bits in a port
 */
#define PORT_MASK_ALL       0xFF

/*
 * Macro: PIN_COUNT_PER_PORT
 * Description: Number of pins in each port
 */
#define PIN_COUNT_PER_PORT  8

/*
 * Macro: VALID_PIN_ID
 * Description: Check if pin ID is valid (0-7)
 */
#define VALID_PIN_ID(pin)   ((pin) < PIN_COUNT_PER_PORT)

/*
 * Macro: VALID_PORT_ID
 * Description: Check if port ID is valid (0-3)
 */
#define VALID_PORT_ID(port) ((port) <= PORTD_ID)

/* ============================================
 *      REGISTER ACCESS HELPER MACROS
 * ============================================ */

/*
 * These macros provide a cleaner way to access registers
 * in switch-case statements
 */

/* Get DDR register pointer based on port ID */
#define GET_DDR_REG(portId) \
    ((portId) == PORTA_ID ? &DDRA_REG : \
     (portId) == PORTB_ID ? &DDRB_REG : \
     (portId) == PORTC_ID ? &DDRC_REG : \
     (portId) == PORTD_ID ? &DDRD_REG : NULL_PTR)

/* Get PORT register pointer based on port ID */
#define GET_PORT_REG(portId) \
    ((portId) == PORTA_ID ? &PORTA_REG : \
     (portId) == PORTB_ID ? &PORTB_REG : \
     (portId) == PORTC_ID ? &PORTC_REG : \
     (portId) == PORTD_ID ? &PORTD_REG : NULL_PTR)

/* Get PIN register pointer based on port ID */
#define GET_PIN_REG(portId) \
    ((portId) == PORTA_ID ? &PINA_REG : \
     (portId) == PORTB_ID ? &PINB_REG : \
     (portId) == PORTC_ID ? &PINC_REG : \
     (portId) == PORTD_ID ? &PIND_REG : NULL_PTR)

/* ============================================
 *         ERROR CHECKING (OPTIONAL)
 * ============================================ */

/*
 * Enable/disable runtime error checking
 * Set to 1 to enable parameter validation (slower but safer)
 * Set to 0 to disable (faster but no safety checks)
 */
#define DIO_ERROR_CHECKING_ENABLED  0

#if DIO_ERROR_CHECKING_ENABLED
    /* Error codes */
    #define DIO_OK              0
    #define DIO_ERROR_INVALID_PORT  1
    #define DIO_ERROR_INVALID_PIN   2
    #define DIO_ERROR_NULL_PTR      3
    
    /* Error handling function (to be implemented if needed) */
    void DIO_voidReportError(u8 errorCode);
#endif

/* ============================================
 *      PERFORMANCE OPTIMIZATION MACROS
 * ============================================ */

#ifdef __GNUC__
    #define DIO_INLINE __attribute__((always_inline)) inline
#else
    #define DIO_INLINE inline
#endif


/* ============================================
 *          REGISTER BIT POSITIONS
 * ============================================ */

/* Individual pin bit positions (for clarity) */
#define PIN0_BIT    0
#define PIN1_BIT    1
#define PIN2_BIT    2
#define PIN3_BIT    3
#define PIN4_BIT    4
#define PIN5_BIT    5
#define PIN6_BIT    6
#define PIN7_BIT    7

#endif /* DIO_PRIVATE_H */