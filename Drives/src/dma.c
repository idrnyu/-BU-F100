#include "dma.h"

// DMA����  @DMA_CHx = DMAͨ��    @cpar = DMA�������ַ  @cmar = DMA�ڴ����ַ  @cndtr = DMAͨ����DMA����Ĵ�С
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);   // ʹ��DMA����
  DMA_DeInit(DMA_CHx);  // ��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;   // DMA�������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = cmar;       // DMA�ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // ���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
  DMA_InitStructure.DMA_BufferSize = cndtr;          // DMAͨ����DMA����Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // �����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    // ���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;      // ����������ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // DMAͨ�� xӵ�������ȼ� 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA_CHx, &DMA_InitStructure); // ����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Rx_DMA ����ʶ�ļĴ���
  DMA_Enable(DMA_CHx, cndtr);
}

void DMA_Enable(DMA_Channel_TypeDef* DMA_CHx, u16 cndtr)
{
  DMA_Cmd(DMA_CHx, DISABLE);   // �ر�USARTx TX DMA1 ��ָʾ��ͨ�� 
  DMA_SetCurrDataCounter(DMA_CHx, cndtr);  // �������û����С,ָ������0
  DMA_Cmd(DMA_CHx, ENABLE);    // ʹ��USARTx_ TX DMA1 ��ָʾ��ͨ��
}


