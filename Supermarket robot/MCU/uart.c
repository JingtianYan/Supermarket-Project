#include "uart.h"
#include "interface.h"
u8 USART_RX_BUF[8];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;       //接收状态标记
//UART function
//UART3 TxD GPIOB10   RxD GPIOB11
void USART3Conf(u32 baudRate)
{
	NVIC_InitTypeDef 	NVIC_InitStruct;//定义一个设置中断的结构体	
	USART_InitTypeDef USART_InitSturct;//定义串口1的初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;//定义串口对应管脚的结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);//打开串口管脚时钟
	//USART3_Tx_Pin Configure 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//输出引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
	GPIO_Init(GPIOB , &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3_Rx_Pin Configure
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO模式悬浮输入
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//输入引脚
  GPIO_Init(GPIOB, &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//波特率19200
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//数据宽度8位
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitSturct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送与接收
	USART_Init(USART3 , &USART_InitSturct);//将初始化好的结构体装入寄存器	
	//USART1_INT Configure
	USART_ITConfig(USART3 , USART_IT_RXNE , ENABLE);//使能接收中断
//	USART_ITConfig(USART3 , USART_IT_TXE , ENABLE);
	USART_Cmd(USART3 , ENABLE);//打开串口
	USART_ClearFlag(USART3 , USART_FLAG_TC);//解决第一个数据发送失败的问题
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//打开该中断
	NVIC_Init(&NVIC_InitStruct);
	
}


//这里把所有的串口1 换成串口3**********************************************************8
void USART3_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
		
			if(Res == 'a'){
				LED_SET;
				Delayms(5000);
				LED_RESET;
				Delayms(5000);
				PutStr("OK");
			}
			
			
			
			
			
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(8-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   	



			
     }else{
			 
			 LED_SET;
				Delayms(1000);
				LED_RESET;
				Delayms(1000);
		 }

 
		 
} 
	










void PutChar(u8 Data)
{
	USART_SendData(USART3 , Data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);//等待发送完毕
}
void PutStr(char *str)//发送一个字符串
{
	while(*str != '\0')
	{
		USART_SendData(USART3 , *str++);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);//等待发送完毕
	}
}

void PutNChar(u8 *buf , u16 size)
{
  u8 i;
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); //防止第一字节丢失
	for(i=0;i<size;i++)
	{
		 USART_SendData(USART3 , buf[i]);
		 while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);//等待发送完毕
	}
}







////////////////////////////////////////////////////////
static void TIM5_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM3CLK 为 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

//   /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
//   GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  配置TIM3输出的PWM信号的模式，如周期、极性、占空比
  * @param  无
  * @retval 无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIM5_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
 	u16 CCR1_Val = 500;        
 	u16 CCR2_Val = 1000;
 	u16 CCR3_Val = 1500;
//  	u16 CCR4_Val = 125;

/* ----------------------------------------------------------------------- 
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR+1)* 100% = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR+1)* 100% = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR+1)* 100% = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR+1)* 100% = 12.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 20000;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);	  //使能通道2
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
  TIM_OC3Init(TIM5, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);


  /* TIM3 enable counter */
  TIM_Cmd(TIM5, ENABLE);                   //使能定时器3	
}

/**
  * @brief  TIM3 输出PWM信号初始化，只要调用这个函数
  *         TIM3的四个通道就会有PWM信号输出
  * @param  无
  * @retval 无
  */
void TIM5_PWM_Init(void)
{
	TIM5_GPIO_Config();
	TIM5_Mode_Config();	
}






