#include "usart.h"
#include <stdarg.h>

// ���ڶ˿ڳ�ʼ��
void USART_GPIO_Config(GPIO_TypeDef *GPIOx, uint16_t TX_Pin_x, uint16_t RX_Pin_x)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = TX_Pin_x;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = RX_Pin_x;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
}

// ���ڳ�ʼ��  USARTx ���ں�    BaudRate ������   isIRQ  �Ƿ��ж�    isRxDMA �Ƿ�DMA
void USART_Config(USART_TypeDef *USARTx, uint32_t BaudRate, bool isIRQ, bool isRxDMA)
{
  if (USARTx == USART1)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);  // ����1ʱ�� GPIOA ʱ��
    USART_GPIO_Config(GPIOA, GPIO_Pin_9, GPIO_Pin_10);
    if(isIRQ)
      NVIC_Configuration(USART1_IRQn);
    if(isRxDMA)
    {
      USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);   //ʹ�ܴ���  ���մ���DMA
      DMA_Config(DMA1_Channel5, (u32)&USART1->DR, (u32)RX1_Buffer, 1024);
      // DMA1ͨ��5,����Ϊ����1��DR�Ĵ���,�洢��ΪRX1_Buffer,����1024��
    }
  }
  else if (USARTx == USART2)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  // ����2 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA ʱ��
    USART_GPIO_Config(GPIOA, GPIO_Pin_2, GPIO_Pin_3);
    if(isIRQ)
      NVIC_Configuration(USART2_IRQn);
    if(isRxDMA)
    {
      USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);   //ʹ�ܴ���  ���մ���DMA
      DMA_Config(DMA1_Channel6, (u32)&USART2->DR, (u32)RX2_Buffer, 1024);
      // DMA1ͨ��5,����Ϊ����1��DR�Ĵ���,�洢��ΪRX2_Buffer,����1024��
    }
  }
  else if (USARTx == USART3)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  // ����3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // GPIOB ʱ��
    USART_GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Pin_11);
    if(isIRQ)
      NVIC_Configuration(USART3_IRQn);
    if(isRxDMA)
    {
      USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);   //ʹ�ܴ���  ���մ���DMA
      DMA_Config(DMA1_Channel3, (u32)&USART2->DR, (u32)RX3_Buffer, 1024);
      // DMA1ͨ��5,����Ϊ����1��DR�Ĵ���,�洢��ΪRX3_Buffer,����1024��
    }
  }
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = BaudRate;                                  // ������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // ����λ8λ
  USART_InitStructure.USART_Parity = USART_Parity_No;                             // nУ��λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 1λֹͣλ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // �շ�ģʽ

  USART_Init(USARTx, &USART_InitStructure); // ��ʼ�����ڲ���
  if(isIRQ)
  {
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);  //  ʹ�ܿ����ж�
    // USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);  //  ʹ�ܽ����ж�
  }
  USART_Cmd(USARTx, ENABLE); // ʹ�ܵ�ǰ����
}


/****************************************************************************
* ��    �ƣ�void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* ��    �ܣ���ʽ�������������
* ��ڲ�����USARTx:  ָ������
			Data��   ��������
			...:     ��������
* ���ڲ�������
* ˵    ������ʽ�������������
      "\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
			"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
* ���÷������� 
****************************************************************************/
void USART_OUT(USART_TypeDef *USARTx, uint8_t *Data, ...)
{
  const char *s;
  int d, arrLength=0;
  char buf[16];
  char * arr;
  va_list ap;
  va_start(ap, Data);

  while (*Data != 0)
  { //�ж��Ƿ񵽴��ַ���������
    if (*Data == 0x5c)
    { //'\'
      switch (*++Data)
      {
      case 'r': //�س���
        USART_SendData(USARTx, 0x0d);
        Data++;
        break;
      case 'n': //���з�
        USART_SendData(USARTx, 0x0a);
        Data++;
        break;

      default:
        Data++;
        break;
      }
    }
    else if (*Data == '%')
    { //
      switch (*++Data)
      {
      case 's': //�ַ���
        s = va_arg(ap, const char *);
        for (; *s; s++)
        {
          while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
          USART_SendData(USARTx, *s);
        }
        Data++;
        break;
      case 'd': //ʮ����
        d = va_arg(ap, int);
        itoa(d, buf, 10);
        for (s = buf; *s; s++)
        {
          while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
          USART_SendData(USARTx, *s);
        }
        Data++;
        break;
      case 'h': //16  ����      
        arr = va_arg(ap, char *);
        arrLength = chLength(arr);
        for(int i=0; i<arrLength; i++)
        {
          while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
          USART_SendData(USARTx, arr[i]);
        }
        Data++;
        break;
      default:
        Data++;
        break;
      }
    }
    else
    {
      while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
      USART_SendData(USARTx, *Data++);
    }
  }
  va_end(ap);
}

/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  
 
	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							    			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
  int i, d;
  int flag = 0;
  char *ptr = string;
  /* This implementation only works for decimal numbers. */
  if (radix != 10)
  {
    *ptr = 0;
    return string;
  }
  if (!value)
  {
    *ptr++ = 0x30;
    *ptr = 0;
    return string;
  }
  /* if this is a negative value insert the minus sign. */
  if (value < 0)
  {
    *ptr++ = '-';
    /* Make the value positive. */
    value *= -1;
  }
  for (i = 10000; i > 0; i /= 10)
  {
    d = value / i;
    if (d || flag)
    {
      *ptr++ = (char)(d + 0x30);
      value -= (d * i);
      flag = 1;
    }
  }
  /* Null terminate the string. */
  *ptr = 0;
  return string;
} /* NCL_Itoa */


void NVIC_Configuration(uint8_t USARTx_IRQn)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    

	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;			     	//���ô���1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     	//��ռ���ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

// ��������
// uint8_t ch[] = { 0xFF, 0xFB, 0xAA };
// USART_OUT_Arr(USART1, ch);
void USART_OUT_Arr(USART_TypeDef* USARTx, uint8_t* Data)
{
  int arrLength = chLength((char *)Data);
  for(int i=0; i<arrLength; i++) {
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
    USART_SendData(USARTx, Data[i]);
  }
}

// �Զ���printf��UART1���
// my_printf(USART2, "%d\t", b[i]);
int vsnprintf (char * s, size_t n, const char * format, va_list arg );
void my_printf(USART_TypeDef* USARTx, const char *fmt, ...)
{  
  char buf[80], *p;
  va_list ap;  
  va_start(ap, fmt);  
  vsnprintf(buf, sizeof(buf), fmt, ap);
  for (p = buf; *p; ++p)
  {
    USART_SendData(USARTx, *p);
    while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
  }
  va_end(ap);
}

// ���ڽ����жϣ�ÿ�յ�һ�����ݿ��Դ���һ��   �����ж�  �Ƚ�����ɺ��ж�
void USART1_IRQHandler(void)      //����1 �жϷ������
{
  // int buffSize = 1024;
  // if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  // �����ж�(���յ������ݱ�����0x0d 0x0a��β)
  // {
  //   USART_ClearITPendingBit(USART1, USART_IT_IDLE); // ����жϱ�־
  //   USART_ReceiveData(USART1);
	// 	GPIO_ResetBits(LED_GPIO_x, LED_Pin_x);

	// 	DMA_Enable(DMA1_Channel5, buffSize);  //��ʼһ��DMA���䣡
	// }

  int buffSize = 1024;
  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  // �������  �����ж� (���յ������ݱ�����0x0d 0x0a��β)
  {
		uint8_t USART_RX_Data = USART_ReceiveData(USART1);
    int Usart1_Rec_Cnt = buffSize - DMA_GetCurrDataCounter(DMA1_Channel5); // ����ӱ�֡���ݳ���
		
		my_printf(USART2, (const char *)RX1_Buffer);  // ����2���Դ���1����

		RX1_Flat = true;
		DMA_Enable(DMA1_Channel5, buffSize);  //��ʼһ��DMA���䣡
		USART_ClearITPendingBit(USART1, USART_IT_IDLE); // ����жϱ�־
  }

	if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)//�����ж� 
	{ 
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//��ֹ�����������жϣ� 
	}
}

void USART2_IRQHandler(void) // ����2�ж�
{
  int buffSize = 1024;
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  // �������  �����ж� (���յ������ݱ�����0x0d 0x0a��β)
  {
		uint8_t USART_RX_Data = USART_ReceiveData(USART2);
    int Usart2_Rec_Cnt = buffSize - DMA_GetCurrDataCounter(DMA1_Channel6); // ����ӱ�֡���ݳ���
		
		my_printf(USART1, (const char *)RX2_Buffer);  // ����1���Դ���2����

		RX2_Flat = true;
		DMA_Enable(DMA1_Channel6, buffSize);  //��ʼһ��DMA���䣡
		USART_ClearITPendingBit(USART2, USART_IT_IDLE); // ����жϱ�־
  }

  if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)//�����ж� 
	{
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);//��ֹ�����������жϣ� 
	}
}

void USART3_IRQHandler(void) // ����2�ж�
{
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  // �����ж�(���յ������ݱ�����0x0d 0x0a��β)
  {
    USART_ClearITPendingBit(USART3, USART_IT_IDLE); // ����жϱ�־
    USART_ReceiveData(USART3);

    DMA_Enable(DMA1_Channel3, 1024);  //��ʼһ��DMA���䣡
  }

  if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)//�����ж� 
	{
		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);//��ֹ�����������жϣ� 
	}	
}





// http://www.eeworld.com.cn/mcu/article_2018060139474.html


