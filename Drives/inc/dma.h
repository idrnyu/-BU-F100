#ifndef __USART_DMA_H
#define __USART_DMA_H

#include "stm32f10x.h"
#include <stdbool.h>

void DMA_Enable(DMA_Channel_TypeDef* DMA_CHx, u16 cndtr);
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr);

#endif /* __USART_DMA_H */

