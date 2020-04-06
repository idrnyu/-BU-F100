#include "usart.h"
#include <stdarg.h>

// 串口端口初始化
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

// 串口初始化  USARTx 串口号    BaudRate 波特率   isIRQ  是否中断    isRxDMA 是否DMA
void USART_Config(USART_TypeDef *USARTx, uint32_t BaudRate, bool isIRQ, bool isRxDMA)
{
  if (USARTx == USART1)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);  // 串口1时钟 GPIOA 时钟
    USART_GPIO_Config(GPIOA, GPIO_Pin_9, GPIO_Pin_10);
    if(isIRQ)
      NVIC_Configuration(USART1_IRQn);
    if(isRxDMA)
    {
      USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);   //使能串口  接收传输DMA
      DMA_Config(DMA1_Channel5, (u32)&USART1->DR, (u32)RX1_Buffer, 1024);
      // DMA1通道5,外设为串口1的DR寄存器,存储器为RX1_Buffer,长度1024，
    }
  }
  else if (USARTx == USART2)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  // 串口2 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA 时钟
    USART_GPIO_Config(GPIOA, GPIO_Pin_2, GPIO_Pin_3);
    if(isIRQ)
      NVIC_Configuration(USART2_IRQn);
    if(isRxDMA)
    {
      USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);   //使能串口  接收传输DMA
      DMA_Config(DMA1_Channel6, (u32)&USART2->DR, (u32)RX2_Buffer, 1024);
      // DMA1通道5,外设为串口1的DR寄存器,存储器为RX2_Buffer,长度1024，
    }
  }
  else if (USARTx == USART3)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  // 串口3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // GPIOB 时钟
    USART_GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Pin_11);
    if(isIRQ)
      NVIC_Configuration(USART3_IRQn);
    if(isRxDMA)
    {
      USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);   //使能串口  接收传输DMA
      DMA_Config(DMA1_Channel3, (u32)&USART2->DR, (u32)RX3_Buffer, 1024);
      // DMA1通道5,外设为串口1的DR寄存器,存储器为RX3_Buffer,长度1024，
    }
  }
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = BaudRate;                                  // 波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // 数据位8位
  USART_InitStructure.USART_Parity = USART_Parity_No;                             // n校验位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 1位停止位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 收发模式

  USART_Init(USARTx, &USART_InitStructure); // 初始化串口参数
  if(isIRQ)
  {
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);  //  使能空闲中断
    // USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);  //  使能接收中断
  }
  USART_Cmd(USARTx, ENABLE); // 使能当前串口
}


/****************************************************************************
* 名    称：void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* 功    能：格式化串口输出函数
* 入口参数：USARTx:  指定串口
			Data：   发送数组
			...:     不定参数
* 出口参数：无
* 说    明：格式化串口输出函数
      "\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
			"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
* 调用方法：无 
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
  { //判断是否到达字符串结束符
    if (*Data == 0x5c)
    { //'\'
      switch (*++Data)
      {
      case 'r': //回车符
        USART_SendData(USARTx, 0x0d);
        Data++;
        break;
      case 'n': //换行符
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
      case 's': //字符串
        s = va_arg(ap, const char *);
        for (; *s; s++)
        {
          while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
          USART_SendData(USARTx, *s);
        }
        Data++;
        break;
      case 'd': //十进制
        d = va_arg(ap, int);
        itoa(d, buf, 10);
        for (s = buf; *s; s++)
        {
          while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
          USART_SendData(USARTx, *s);
        }
        Data++;
        break;
      case 'h': //16  数组      
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
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  
 
	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
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

	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;			     	//设置串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     	//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能
	NVIC_Init(&NVIC_InitStructure);
}

// 发送数组
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

// 自定义printf到UART1输出
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

// 串口接收中断，每收到一个数据可以处理一次   空闲中断  等接收完成后中断
void USART1_IRQHandler(void)      //串口1 中断服务程序
{
  // int buffSize = 1024;
  // if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
  // {
  //   USART_ClearITPendingBit(USART1, USART_IT_IDLE); // 清除中断标志
  //   USART_ReceiveData(USART1);
	// 	GPIO_ResetBits(LED_GPIO_x, LED_Pin_x);

	// 	DMA_Enable(DMA1_Channel5, buffSize);  //开始一次DMA传输！
	// }

  int buffSize = 1024;
  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  // 接收完成  空闲中断 (接收到的数据必须是0x0d 0x0a结尾)
  {
		uint8_t USART_RX_Data = USART_ReceiveData(USART1);
    int Usart1_Rec_Cnt = buffSize - DMA_GetCurrDataCounter(DMA1_Channel5); // 算出接本帧数据长度
		
		my_printf(USART2, (const char *)RX1_Buffer);  // 串口2回显串口1数据

		RX1_Flat = true;
		DMA_Enable(DMA1_Channel5, buffSize);  //开始一次DMA传输！
		USART_ClearITPendingBit(USART1, USART_IT_IDLE); // 清除中断标志
  }

	if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)//发送中断 
	{ 
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//禁止发缓冲器空中断， 
	}
}

void USART2_IRQHandler(void) // 串口2中断
{
  int buffSize = 1024;
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  // 接收完成  空闲中断 (接收到的数据必须是0x0d 0x0a结尾)
  {
		uint8_t USART_RX_Data = USART_ReceiveData(USART2);
    int Usart2_Rec_Cnt = buffSize - DMA_GetCurrDataCounter(DMA1_Channel6); // 算出接本帧数据长度
		
		my_printf(USART1, (const char *)RX2_Buffer);  // 串口1回显串口2数据

		RX2_Flat = true;
		DMA_Enable(DMA1_Channel6, buffSize);  //开始一次DMA传输！
		USART_ClearITPendingBit(USART2, USART_IT_IDLE); // 清除中断标志
  }

  if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)//发送中断 
	{
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);//禁止发缓冲器空中断， 
	}
}

void USART3_IRQHandler(void) // 串口2中断
{
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
  {
    USART_ClearITPendingBit(USART3, USART_IT_IDLE); // 清除中断标志
    USART_ReceiveData(USART3);

    DMA_Enable(DMA1_Channel3, 1024);  //开始一次DMA传输！
  }

  if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)//发送中断 
	{
		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);//禁止发缓冲器空中断， 
	}	
}





// http://www.eeworld.com.cn/mcu/article_2018060139474.html


