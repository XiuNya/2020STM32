#ifndef __RNG_H
#define __RNG_H
#include "sys.h"

u8 RNG_Init(void);
u32 RNG_Get_RandomNum(void);
int RNG_Get_RandomRange(int min,int max);

#endif
