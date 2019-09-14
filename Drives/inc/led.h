#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include <stdbool.h>

#define LED_Pin_x       GPIO_Pin_1
#define LED_GPIO_x      GPIOA

void Init_LED(void);

#endif /* __LED_H */

