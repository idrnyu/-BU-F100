#include "dma.h"

// DMA配置  @DMA_CHx = DMA通道    @cpar = DMA外设基地址  @cmar = DMA内存基地址  @cndtr = DMA通道的DMA缓存的大小
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);   // 使能DMA传输
  DMA_DeInit(DMA_CHx);  // 将DMA的通道x寄存器重设为缺省值
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;   // DMA外设基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = cmar;       // DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 数据传输方向，从内存读取发送到外设
  DMA_InitStructure.DMA_BufferSize = cndtr;          // DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    // 数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;      // 工作在正常模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // DMA通道 x拥有中优先级 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA_CHx, &DMA_InitStructure); // 根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Rx_DMA 所标识的寄存器
  DMA_Enable(DMA_CHx, cndtr);
}

void DMA_Enable(DMA_Channel_TypeDef* DMA_CHx, u16 cndtr)
{
  DMA_Cmd(DMA_CHx, DISABLE);   // 关闭USARTx TX DMA1 所指示的通道 
  DMA_SetCurrDataCounter(DMA_CHx, cndtr);  // 从新设置缓冲大小,指向数组0
  DMA_Cmd(DMA_CHx, ENABLE);    // 使能USARTx_ TX DMA1 所指示的通道
}


