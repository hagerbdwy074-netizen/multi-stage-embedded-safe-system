#ifndef MCAL_MACROS_MACROS_H_
#define MCAL_MACROS_MACROS_H_

// determine the bit value
#define SET_BIT(REG,BIT) ((REG) |= (1<<(BIT)))

// clear the bit
#define CLEAR_BIT(REG,BIT) ((REG) &=~ (1<<(BIT)))

// inverse the bit status
#define TOGGLE_BIT(REG,BIT) ((REG) ^= (1<<(BIT)))

// rotating to the right
#define ROR(REG,NUM) (((REG) >> (NUM)) | ((REG) << (8-(NUM))))

// rotating to the left
#define ROL(REG,NUM) (((REG) << (NUM)) | ((REG) >> (8-(NUM))))

// checking
#define BIT_IS_SET(REG,BIT) ((REG) & (1<<(BIT)))

//
#define BIT_IS_CLEAR(REG,BIT) (!((REG) & (1<<BIT)))

// reading the value of the bit
#define GET_BIT(REG,BIT) (((REG) >> (BIT)) & 1)

#endif