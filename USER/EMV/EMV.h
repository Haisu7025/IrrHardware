
#ifndef __EMV_H
#define __EMV_H

#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"

void EMV_init(void);
void EMV_open(void);
void EMV_close(void);

#endif
