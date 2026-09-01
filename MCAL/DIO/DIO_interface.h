/*
* <<<<<<<  DIO_program.c  >>>>>>> *
* Author      : Hagar mhmd "Dibra"
* Created     : 12/6/2024
* Layer       : MCAL
* SWC         : DIO 
*/
#ifndef DIO_INTERFACE_H
#define DIO_INTERFACE_H

#include "STANDARD_TYPES.h"

/* ============================================
 *          VERSION INFORMATION
 * ============================================ */
#define DIO_DRIVER_VERSION_MAJOR    2
#define DIO_DRIVER_VERSION_MINOR    0
#define DIO_DRIVER_VERSION_PATCH    0

/* ============================================
 *            PORT IDENTIFIERS
 * ============================================ */
#define PORTA_ID    0  /* Port A identifier */
#define PORTB_ID    1  /* Port B identifier */
#define PORTC_ID    2  /* Port C identifier */
#define PORTD_ID    3  /* Port D identifier */

/* ============================================
 *         PIN DIRECTION DEFINITIONS
 * ============================================ */
#define PIN_INPUT   0  /* Configure pin as input */
#define PIN_OUTPUT  1  /* Configure pin as output */

/* ============================================
 *          PIN VALUE DEFINITIONS
 * ============================================ */
#define PIN_LOW     0  /* Logic LOW (0V) */
#define PIN_HIGH    1  /* Logic HIGH (5V/3.3V) */

/* ============================================
 *        PORT DIRECTION DEFINITIONS
 * ============================================ */
#define PORT_INPUT      0x00  /* All pins as input (0b00000000) */
#define PORT_OUTPUT     0xFF  /* All pins as output (0b11111111) */

/* ============================================
 *         PORT VALUE DEFINITIONS
 * ============================================ */
#define PORT_LOW        0x00  /* All pins LOW */
#define PORT_HIGH       0xFF  /* All pins HIGH */

/* ============================================
 *       PULL-UP RESISTOR CONTROL
 * ============================================ */
#define PULL_UP_DISABLE 0  /* Disable internal pull-up */
#define PULL_UP_ENABLE  1  /* Enable internal pull-up */

/* ============================================
 *           PIN NUMBER DEFINITIONS
 * ============================================ */
#define PIN0    0  /* Pin 0 */
#define PIN1    1  /* Pin 1 */
#define PIN2    2  /* Pin 2 */
#define PIN3    3  /* Pin 3 */
#define PIN4    4  /* Pin 4 */
#define PIN5    5  /* Pin 5 */
#define PIN6    6  /* Pin 6 */
#define PIN7    7  /* Pin 7 */

/* ============================================
 *          NIBBLE DEFINITIONS
 * ============================================ */
#define LOWER_NIBBLE    0  /* Lower 4 bits (pins 0-3) */
#define UPPER_NIBBLE    1  /* Upper 4 bits (pins 4-7) */

/* ============================================
 *        COMMONLY USED BIT PATTERNS
 * ============================================ */
#define EVEN_PINS       0b01010101  /* Pins 0,2,4,6 */
#define ODD_PINS        0b10101010  /* Pins 1,3,5,7 */
#define FIRST_HALF      0b00001111  /* Pins 0-3 */
#define SECOND_HALF     0b11110000  /* Pins 4-7 */

/* ============================================
 *     BASIC PIN OPERATIONS (LEVEL 1)
 * ============================================ */


void DIO_voidSetPinDirection(u8 portId, u8 pinId, u8 direction);


void DIO_voidSetPinValue(u8 portId, u8 pinId, u8 value);


u8 DIO_u8GetPinValue(u8 portId, u8 pinId);


void DIO_voidTogglePin(u8 portId, u8 pinId);


void DIO_voidEnablePullUp(u8 portId, u8 pinId);


void DIO_voidDisablePullUp(u8 portId, u8 pinId);

/* ============================================
 *    FULL PORT OPERATIONS (LEVEL 2)
 * ============================================ */

void DIO_voidSetPortDirection(u8 portId, u8 direction);

void DIO_voidSetPortValue(u8 portId, u8 value);


u8 DIO_u8GetPortValue(u8 portId);


void DIO_voidTogglePort(u8 portId);

/* ============================================
 *    NIBBLE OPERATIONS (LEVEL 3 - ADVANCED)
 * ============================================ */


void DIO_voidSetNibbleDirection(u8 portId, u8 nibble, u8 direction);


void DIO_voidSetNibbleValue(u8 portId, u8 nibble, u8 value);


u8 DIO_u8GetNibbleValue(u8 portId, u8 nibble);


void DIO_voidToggleNibble(u8 portId, u8 nibble);

/* ============================================
 *   CONVENIENCE FUNCTIONS (LEVEL 4)
 * ============================================ */


void DIO_voidWriteHighNibble(u8 portId, u8 value);


void DIO_voidWriteLowNibble(u8 portId, u8 value);

/* ============================================
 *     BIT FIELD OPERATIONS (LEVEL 5 - PRO)
 * ============================================ */


void DIO_voidSetPinRange(u8 portId, u8 startPin, u8 endPin, u8 value);


void DIO_voidTogglePinRange(u8 portId, u8 startPin, u8 endPin);

/* ============================================
 *       ATOMIC OPERATIONS (LEVEL 6 - PRO)
 * ============================================ */


void DIO_voidAtomicSetPin(u8 portId, u8 pinId, u8 value);


void DIO_voidAtomicTogglePin(u8 portId, u8 pinId);

/* ============================================
 *      UTILITY FUNCTIONS (LEVEL 7)
 * ============================================ */


u8 DIO_u8ReadPortWithMask(u8 portId, u8 mask);


void DIO_voidWritePortWithMask(u8 portId, u8 value, u8 mask);


#endif 