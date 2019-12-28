//连接方式 ：请参考interface.h文件
//部份源码来源网络--请试用24小时后删除
#include "stm32f10x.h"
#include "interface.h"
#include "LCD1602.h"
#include "IRCtrol.h"
#include "motor.h"
#include "uart.h"
#include "UltrasonicCtrol.h"
#define EMPTY 1000
struct Position{
	int x;
	int y;
};
//全局变量定义
unsigned int speed_count=0;//占空比计数器 50次一周期
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms计数器，作为主函数的基本周期
unsigned char tick_1ms = 0;//1ms计数器，作为电机的基本计数器
unsigned char tick_200ms = 0;//刷新显示

char ctrl_comm = COMM_STOP;//控制指令
char ctrl_comm_last = COMM_STOP;//上一次的指令
unsigned char continue_time=0;
unsigned char bt_rec_flag=0;//蓝牙控制标志位
int Now_LED = 0;
int Last_LED = 0;
int MAIN_FLAG = 1;//主要路线ABCD
struct Position MainPosition; //Main路线上面的位置
struct Position NowPosition; //Main路线上面的位置
int object_cnt = 1;
int shelves; //这里代表货物应该放在的货架位置



int flag_of_putting = 0;
int moving_cnt = 0;

extern u8 USART_RX_BUF[8];
//循迹，通过判断三个光电对管的状态来控制小车运动






void SearchRun(void)
{
	//三路都检测到
	
	
	if(SEARCH_L_IO == BLACK_AREA && SEARCH_M_IO == WHITE_AREA && SEARCH_R_IO == BLACK_AREA)
	{
		ctrl_comm = COMM_UP;
		return;
	}
	
	
	
	if(SEARCH_L_IO == WHITE_AREA && SEARCH_M_IO == BLACK_AREA && SEARCH_R_IO == BLACK_AREA)//右
	{
		ctrl_comm = COMM_LEFT;
	}
	else if(SEARCH_L_IO == BLACK_AREA && SEARCH_M_IO == BLACK_AREA && SEARCH_R_IO == WHITE_AREA )//左
	{
		ctrl_comm = COMM_RIGHT;
	}
	else if(SEARCH_L_IO == WHITE_AREA && SEARCH_M_IO == WHITE_AREA && SEARCH_R_IO == WHITE_AREA)//中
	{
		ctrl_comm = COMM_UP;
	}
	
}


void SomeUnkonw()
{
		 
	 if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_200ms++;
			if(tick_200ms >= 40)
			{
				tick_200ms = 0;
				LEDToggle(LED_PIN);
			}
//			continue_time--;//200ms 无接收指令就停车
//			if(continue_time == 0)
//			{
//				continue_time = 1;
//				CarStop();
//			}
			//do something
			
	  }
}





//用不到壁障函数
//void BarrierProc(void)
//{
//	if((distance_cm < 15 && distance_cm > 10) || distance_cm > 25) 
//	{
//		ctrl_comm = COMM_STOP;//防震荡
//		return;
//	}
//	
//	if(distance_cm <= 10) ctrl_comm = COMM_DOWN;
//	
//	if(distance_cm >= 15) ctrl_comm = COMM_UP;
//}

//延时函数
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}






// 机械臂抓取函数，其中x，y，z参数待调整
void catch(int x,int y,int z)
	{
					TIM_SetCompare1(TIM5, x);
					Delayms(0x0FFFFF);
					TIM_SetCompare2(TIM5, y);
					Delayms(0x0FFFFF);
					TIM_SetCompare3(TIM5, z);
					Delayms(0x0FFFFF);
		
	}



void need_distance(int length)
{
	Distance();//计算距离
	while(distance_cm < (length+1) && distance_cm > length)
										 {	 
											 if(distance_cm < 20){
												 CarBack();
												 Delayms(60);
												 CarStop();
											 }else if(distance_cm > 23){
												 CarBack();
												 Delayms(60);
												 CarStop();
											 }
											 Distance();//计算距离
										 }		


}

	
	
	
	
	

int main(void)
{
	
	u16 USART_RX_STA=0;       //接收状态标记	  
  
  u16 len;	
	int goal;
//	struct Position MainPosition;
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	LCD1602Init();
	IRCtrolInit();
	TIM2_Init();
	//TIM5_PWM_Init();
	MotorInit();
	ServoInit();
	
	RedRayInit();
	//USART3Conf(9600);

	//开始的时候需要先暂停10秒
	//Delayms(3000);
	USART3Conf(19200);
	UltraSoundInit();
//	//测试串口函数
//	
//distance_cm = 0;
// Distance();//计算距离
//	LED_RESET;
//	Delayms(500);
//	LED_SET;
//	Delayms(500);
//  LED_RESET;
	
//	while(1){
//		
//	}
	
	CarGo();
	Delayms(20000);
	CarStop();
Delayms(10);
//	Delayms(500);
//	LED_SET;
//	Delayms(500);
//  LED_RESET;
//	
//	Delayms(500);
//	LED_SET;
//	Delayms(500);
//  LED_RESET;
//	
//	Delayms(500);
//	LED_SET;
//	Delayms(500);
//  LED_RESET;



//	
//		CarLeft();
//		Delayms(1);
//		CarStop();
//		Delayms(100);





	
// while(1)
// {	 
//	
//	 
//	 /*机械臂初始化*/
//	 
//	 
//	 
//			SomeUnkonw();
//			//SearchRun();
//			
//		 
//		 
//		 
//		 
//		 
//			  switch (MAIN_FLAG)
//			{
//						case 1:
//							if(object_cnt<=3){//表示开始有物品了,而且是在第一行
//								
//								PutChar(32);//这里发送32给电脑，如果电脑收到O就会拍照
//									
//									/*接收回来消息,就是要得到的目标位置*/
//									goal = USART_RX_BUF[0];
//									
//						
//									/*如果返回值很奇怪，说明是不要的*/
//								
//								
//								/*串口发送消息*/
//								/*32 这个是space的ASCII码*/
//								if(goal==5){
//									object_cnt++;
//									// do not catch the object
//								
//								}
//								else{
//									catch(111,111,111);
//								
//									
//								
//								
//								if(object_cnt == 1){//第一个物品
//									object_cnt ++;
//									/*前进x厘米*/
//									CarGo();
//									Delayms(17936);//一个物品需要走的距离
//									CarStop();
//									
//									need_distance(17936);//走到指定位置
//									
//								}
//								else if(object_cnt == 2){
//									object_cnt ++;
//									CarGo();
//									Delayms(17936);//一个物品需要走的距离
//									CarStop();
//									need_distance(17936);//走到指定位置
//								}
//								else if(object_cnt == 3){
//									object_cnt ++;
//									CarGo();
//									Delayms(17936);//一个物品需要走的距离
//									CarStop();
//									need_distance(17963);
//								}
//								
//								
//								
//								
//								
//								
//								
//								
//										
//										switch (goal){
//										case 1:
//											/*转弯*/
//											CarRight();
//											Delayms(17936);//这个参数需要调整
//											CarStop();
////////										
//											/*前行一小段，还有一个作用是保持小车直线前行*/
//											CarGo();
//											Delayms(17936);//这个参数需要调整
//											CarStop();
//										
//										  /*超声波第一次定位*/
//										need_distance(17963);
//										 //小车再一次右转
//										 CarRight();
//										 Delayms(17936);
//										
//										 need_distance(17963);
//											/*  前进检验空位**/
//										 //每次运动都移动相同时间 记个数 然后退回
//										  flag_of_putting = 0;
//										  moving_cnt = 0;
//									   	CarGo();//注意这里是back还是go
//									  	Delayms(17936);//来到第一格子
//										  CarStop();
//										  while(flag_of_putting==0){
//												DistanceA();//探测第一个空格
//												if(distance_cm< EMPTY){//距离小于多少 表示有东西了，然后再测另外一个
//													DistanceB();
//													if(distance_cm< EMPTY){//距离小于多少 表示有东西了,此时应该往前移动了
//														moving_cnt++;//说明这个格子不行，需要再往前
//														CarGo();
//														Delayms(17936);//这个数值保证每次移动一个格子，这里不数下面的黑格子，而是看货架的格子
//														CarStop();
//														
//													}else{//表示探测的位置可以放东西
//														flag_of_putting=1;
//													/*放东西函数*/
//													}
//													
//													
//												}else{//表示探测的位置可以放东西
//													flag_of_putting=1;
//													/*放东西函数*/
//												}
//												
//											}
//											flag_of_putting = 0;
//											
//											
//										 
//										 
//										 
//										
//											/*回到定位位置,当然是使用到了上次的moving_cnt*/
//										
//										  CarBack();
//											Delayms(moving_cnt*17936);
//											CarStop();
//											/*没有出错的话应该是到格子1前的位置,这个时候再利用超声波，保证是来到了第一个位置*/
//										need_distance(17963);
//											
//										 
//										  /*两次倒退*/
//										
//										 CarBackRight();//这里可能有点奇怪 为什么是右拐 因为右拐之后 车头方面探测距离
//										
//										 //每次转弯都要定位
//										  Distance();//计算距离
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//计算距离
//										 }
//										 
//										 CarBack();  //因为上次是Right 所以一定是Back
//										 Delayms(17936);
//										 //再次定位，马上就要回去了，准备转弯
//										 Distance();//计算距离
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//计算距离
//										 }
//										 
//										 CarBackRight();
//										 Delayms(17936);
//										 
//////										 //转完之后再走一小段来保证走到了直线上
//////										 CarBack();
//////										 Delayms(17936);
//////										 
//										 
//										 //上一步保证左右对齐，这一步保证前后对齐，之后准备走直线回去了
//										  Distance();//计算距离
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//计算距离
//										 }
//										 
//										 //根据object来判断回退多少
//										 CarBack();
//										 Delayms(17936);
//										 //如果是第三个 一定要改成B
//											break;
//										 
//										case 2:
//											CarGo();
//											Delayms(17936);//这个参数需要调整
//											CarStop();
//											Distance();//计算距离
//											while(distance_cm < 17936 && distance_cm > 17936)
//											{	 
//											 if(distance_cm < 17936){
//												 CarBack();//CarGo()
//												 Delayms(10);
//												 CarStop();
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//												 CarStop();
//											 }
//											 Distance();//计算距离
//										 }
//										
//										
////////										
//											/*前行一小段，还有一个作用是保持小车直线前行,使得达到第一个格子*/
//											CarGo();
//											Delayms(17936);//这个参数需要调整
//											CarStop();
//										
//										  /*  超声波第一次定?   ?*/
//										Distance();//计算距离
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();//CarGo()
//												 Delayms(10);
//												 CarStop();
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//												 CarStop();
//											 }
//											 Distance();//计算距离
//										 }

//											/*  前进检验空位**/
//										 //每次运动都移动相同时间 记个数 然后退回
//										  flag_of_putting = 0;
//										  moving_cnt = 0;
//									  
//										  while(flag_of_putting==0){
//												DistanceA();//探测第一个空格
//												if(distance_cm< EMPTY){//距离小于多少 表示有东西了，然后再测另外一个
//													DistanceB();
//													if(distance_cm< EMPTY){//距离小于多少 表示有东西了,此时应该往前移动了
//														moving_cnt++;//说明这个格子不行，需要再往前
//														CarBack();
//														Delayms(17936);//这个数值保证每次移动一个格子，这里不数下面的黑格子，而是看货架的格子
//														CarStop();
//														
//													}else{//表示探测的位置可以放东西
//														flag_of_putting=1;
//													/*放东西函数*/
//													}
//													
//													
//												}else{//表示探测的位置可以放东西
//													flag_of_putting=1;
//													/*放东西函数*/
//												}
//												
//											}
//											flag_of_putting = 0;
//											
//											
//										 
//										 
//										 
//										
//											/*回到定位位置,当然是使用到了上次的moving_cnt*/
//										
//										  CarBack();
//											Delayms(moving_cnt*17936);
//											CarStop();
//											/*到达初始位置*/
//										 need_distance(17936);
//											CarGo();
//											Delayms(17936);
//											need_distance(17936);
//											
//										 
//										  /*两次倒退*/
//										
//										 CarBackRight();//这里可能有点奇怪 为什么是右拐 因为右拐之后 车头方面探测距离
//										 Delayms(17936);
//										 CarStop();
//										 //每次转弯都要定位
//										  Distance();//计算距离
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//计算距离
//										 }
//										 
//										 
//										 
//										 
//										 
//										 
//										 CarBack();  //因为上次是Right 所以一定是Back
//										 Delayms(17936);
//										 
////										 //回到初始位置
//										 Distance();//计算距离
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//计算距离
//										 }
//										 //这里根据倒退的距离，object进行计算
//										 
//										 if(object_cnt==2){
//												CarBack();
//												Delayms(17936);
//												 need_distance(17963);
//										 }
//										 
//										  else if(object_cnt==3){
//												CarBack();
//												Delayms(17936);
//												need_distance(17963);
//										 }
//											
//										 else if(object_cnt==4){
//											 
//											 
//											 //这里要改变结果
////												CarBack();
////												Delayms(17936);
////											need_distance(17963);
//										 }
//										 

//											break;
//										case 3:
//											CarLeft();
//											CarGo();
//											Delayms(17963);
//											CarStop();
//											need_distance(17936);
//											CarRight();
//										//arrive at the place we need
//							
//											CarGo();
//											Delayms(17963);
//											CarStop();
//											need_distance(17963);
//										flag_of_putting = 0;
//										  moving_cnt = 0;
//									  
//										  while(flag_of_putting==0){
//												DistanceA();//探测第一个空格
//												if(distance_cm< EMPTY){//距离小于多少 表示有东西了，然后再测另外一个
//													DistanceB();
//													if(distance_cm< EMPTY){//距离小于多少 表示有东西了,此时应该往前移动了
//														moving_cnt++;//说明这个格子不行，需要再往前
//														CarBack();
//														Delayms(17936);//这个数值保证每次移动一个格子，这里不数下面的黑格子，而是看货架的格子
//														CarStop();
//														
//													}else{//表示探测的位置可以放东西
//														flag_of_putting=1;
//													/*放东西函数*/
//													}
//													
//													
//												}else{//表示探测的位置可以放东西
//													flag_of_putting=1;
//													/*放东西函数*/
//												}
//												
//											}
//											flag_of_putting = 0;
//											
//											
//											//然后就要回去了
//											
//										
//										
//										
//											break;

//									}
//									}
//									
//								
//								
//								
//								
//								
//								
//								
//								
//								
//								
//								
//								
//							}
//							break;
//						case 2:
//							
//							break;
//						case 3:
//							break;
//						case 4:
//							break;
//					}
//			 
//	 
//	 
//	 
//	 
//	 
//	 
//			
//			
//					
//					
//			
//				
//					
//					
//					
//					
//			}
//	 	
//			





























////调试

//before



		
 }
















































