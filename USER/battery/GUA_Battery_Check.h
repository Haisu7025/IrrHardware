#ifndef _GUA_BATTERY_CHECK_H_
#define _GUA_BATTERY_CHECK_H_

/*********************宏定义************************/
#ifndef GUA_U8
typedef unsigned char GUA_U8;
#endif

#ifndef GUA_8
typedef signed char GUA_8;
#endif

#ifndef GUA_U16
typedef unsigned short GUA_U16;
#endif

#ifndef GUA_16
typedef signed short GUA_16;
#endif

#ifndef GUA_U32
typedef unsigned long GUA_U32;
#endif

#ifndef GUA_32
typedef signed long GUA_32;
#endif

#ifndef GUA_U64
typedef unsigned long long GUA_U64;
#endif

#ifndef GUA_64
typedef signed long long GUA_64;
#endif

/*********************外部函数声明************************/
GUA_U16 GUA_Battery_Check_Read(void);
void GUA_Battery_Check_Init(void);

#endif