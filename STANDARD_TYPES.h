#ifndef STANDARD_TYPES_h
#define STANDARD_TYPES_h
// 8 bit types
typedef unsigned char u8;
typedef signed char s8;
// 16 bit types
typedef unsigned short int u16;
typedef signed short int s16;
// 32 bit types
typedef unsigned long int u32;
typedef signed long int s32;
// 64 bit types
typedef unsigned long long int u64;
typedef signed long long int s64;
// float numbers
typedef float f32;
typedef double d128;
typedef long double d64;
// null
#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif
// true & false
#ifndef False
#define False (0u)
#endif
#ifndef True
#define True (1u)
#endif
typedef unsigned char bool;
// high / low
typedef enum {
	LOW = (0u),
	HIGH = (1u)
} Logical_Value;
#endif