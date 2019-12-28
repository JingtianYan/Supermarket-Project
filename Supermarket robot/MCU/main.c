//���ӷ�ʽ ����ο�interface.h�ļ�
//����Դ����Դ����--������24Сʱ��ɾ��
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
//ȫ�ֱ�������
unsigned int speed_count=0;//ռ�ձȼ����� 50��һ����
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms����������Ϊ�������Ļ�������
unsigned char tick_1ms = 0;//1ms����������Ϊ����Ļ���������
unsigned char tick_200ms = 0;//ˢ����ʾ

char ctrl_comm = COMM_STOP;//����ָ��
char ctrl_comm_last = COMM_STOP;//��һ�ε�ָ��
unsigned char continue_time=0;
unsigned char bt_rec_flag=0;//�������Ʊ�־λ
int Now_LED = 0;
int Last_LED = 0;
int MAIN_FLAG = 1;//��Ҫ·��ABCD
struct Position MainPosition; //Main·�������λ��
struct Position NowPosition; //Main·�������λ��
int object_cnt = 1;
int shelves; //����������Ӧ�÷��ڵĻ���λ��



int flag_of_putting = 0;
int moving_cnt = 0;

extern u8 USART_RX_BUF[8];
//ѭ����ͨ���ж��������Թܵ�״̬������С���˶�






void SearchRun(void)
{
	//��·����⵽
	
	
	if(SEARCH_L_IO == BLACK_AREA && SEARCH_M_IO == WHITE_AREA && SEARCH_R_IO == BLACK_AREA)
	{
		ctrl_comm = COMM_UP;
		return;
	}
	
	
	
	if(SEARCH_L_IO == WHITE_AREA && SEARCH_M_IO == BLACK_AREA && SEARCH_R_IO == BLACK_AREA)//��
	{
		ctrl_comm = COMM_LEFT;
	}
	else if(SEARCH_L_IO == BLACK_AREA && SEARCH_M_IO == BLACK_AREA && SEARCH_R_IO == WHITE_AREA )//��
	{
		ctrl_comm = COMM_RIGHT;
	}
	else if(SEARCH_L_IO == WHITE_AREA && SEARCH_M_IO == WHITE_AREA && SEARCH_R_IO == WHITE_AREA)//��
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
//			continue_time--;//200ms �޽���ָ���ͣ��
//			if(continue_time == 0)
//			{
//				continue_time = 1;
//				CarStop();
//			}
			//do something
			
	  }
}





//�ò������Ϻ���
//void BarrierProc(void)
//{
//	if((distance_cm < 15 && distance_cm > 10) || distance_cm > 25) 
//	{
//		ctrl_comm = COMM_STOP;//����
//		return;
//	}
//	
//	if(distance_cm <= 10) ctrl_comm = COMM_DOWN;
//	
//	if(distance_cm >= 15) ctrl_comm = COMM_UP;
//}

//��ʱ����
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}






// ��е��ץȡ����������x��y��z����������
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
	Distance();//�������
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
											 Distance();//�������
										 }		


}

	
	
	
	
	

int main(void)
{
	
	u16 USART_RX_STA=0;       //����״̬���	  
  
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

	//��ʼ��ʱ����Ҫ����ͣ10��
	//Delayms(3000);
	USART3Conf(19200);
	UltraSoundInit();
//	//���Դ��ں���
//	
//distance_cm = 0;
// Distance();//�������
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
//	 /*��е�۳�ʼ��*/
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
//							if(object_cnt<=3){//��ʾ��ʼ����Ʒ��,�������ڵ�һ��
//								
//								PutChar(32);//���﷢��32�����ԣ���������յ�O�ͻ�����
//									
//									/*���ջ�����Ϣ,����Ҫ�õ���Ŀ��λ��*/
//									goal = USART_RX_BUF[0];
//									
//						
//									/*�������ֵ����֣�˵���ǲ�Ҫ��*/
//								
//								
//								/*���ڷ�����Ϣ*/
//								/*32 �����space��ASCII��*/
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
//								if(object_cnt == 1){//��һ����Ʒ
//									object_cnt ++;
//									/*ǰ��x����*/
//									CarGo();
//									Delayms(17936);//һ����Ʒ��Ҫ�ߵľ���
//									CarStop();
//									
//									need_distance(17936);//�ߵ�ָ��λ��
//									
//								}
//								else if(object_cnt == 2){
//									object_cnt ++;
//									CarGo();
//									Delayms(17936);//һ����Ʒ��Ҫ�ߵľ���
//									CarStop();
//									need_distance(17936);//�ߵ�ָ��λ��
//								}
//								else if(object_cnt == 3){
//									object_cnt ++;
//									CarGo();
//									Delayms(17936);//һ����Ʒ��Ҫ�ߵľ���
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
//											/*ת��*/
//											CarRight();
//											Delayms(17936);//���������Ҫ����
//											CarStop();
////////										
//											/*ǰ��һС�Σ�����һ�������Ǳ���С��ֱ��ǰ��*/
//											CarGo();
//											Delayms(17936);//���������Ҫ����
//											CarStop();
//										
//										  /*��������һ�ζ�λ*/
//										need_distance(17963);
//										 //С����һ����ת
//										 CarRight();
//										 Delayms(17936);
//										
//										 need_distance(17963);
//											/*  ǰ�������λ**/
//										 //ÿ���˶����ƶ���ͬʱ�� �Ǹ��� Ȼ���˻�
//										  flag_of_putting = 0;
//										  moving_cnt = 0;
//									   	CarGo();//ע��������back����go
//									  	Delayms(17936);//������һ����
//										  CarStop();
//										  while(flag_of_putting==0){
//												DistanceA();//̽���һ���ո�
//												if(distance_cm< EMPTY){//����С�ڶ��� ��ʾ�ж����ˣ�Ȼ���ٲ�����һ��
//													DistanceB();
//													if(distance_cm< EMPTY){//����С�ڶ��� ��ʾ�ж�����,��ʱӦ����ǰ�ƶ���
//														moving_cnt++;//˵��������Ӳ��У���Ҫ����ǰ
//														CarGo();
//														Delayms(17936);//�����ֵ��֤ÿ���ƶ�һ�����ӣ����ﲻ������ĺڸ��ӣ����ǿ����ܵĸ���
//														CarStop();
//														
//													}else{//��ʾ̽���λ�ÿ��ԷŶ���
//														flag_of_putting=1;
//													/*�Ŷ�������*/
//													}
//													
//													
//												}else{//��ʾ̽���λ�ÿ��ԷŶ���
//													flag_of_putting=1;
//													/*�Ŷ�������*/
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
//											/*�ص���λλ��,��Ȼ��ʹ�õ����ϴε�moving_cnt*/
//										
//										  CarBack();
//											Delayms(moving_cnt*17936);
//											CarStop();
//											/*û�г���Ļ�Ӧ���ǵ�����1ǰ��λ��,���ʱ�������ó���������֤�������˵�һ��λ��*/
//										need_distance(17963);
//											
//										 
//										  /*���ε���*/
//										
//										 CarBackRight();//��������е���� Ϊʲô���ҹ� ��Ϊ�ҹ�֮�� ��ͷ����̽�����
//										
//										 //ÿ��ת�䶼Ҫ��λ
//										  Distance();//�������
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//�������
//										 }
//										 
//										 CarBack();  //��Ϊ�ϴ���Right ����һ����Back
//										 Delayms(17936);
//										 //�ٴζ�λ�����Ͼ�Ҫ��ȥ�ˣ�׼��ת��
//										 Distance();//�������
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//�������
//										 }
//										 
//										 CarBackRight();
//										 Delayms(17936);
//										 
//////										 //ת��֮������һС������֤�ߵ���ֱ����
//////										 CarBack();
//////										 Delayms(17936);
//////										 
//										 
//										 //��һ����֤���Ҷ��룬��һ����֤ǰ����룬֮��׼����ֱ�߻�ȥ��
//										  Distance();//�������
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//�������
//										 }
//										 
//										 //����object���жϻ��˶���
//										 CarBack();
//										 Delayms(17936);
//										 //����ǵ����� һ��Ҫ�ĳ�B
//											break;
//										 
//										case 2:
//											CarGo();
//											Delayms(17936);//���������Ҫ����
//											CarStop();
//											Distance();//�������
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
//											 Distance();//�������
//										 }
//										
//										
////////										
//											/*ǰ��һС�Σ�����һ�������Ǳ���С��ֱ��ǰ��,ʹ�ôﵽ��һ������*/
//											CarGo();
//											Delayms(17936);//���������Ҫ����
//											CarStop();
//										
//										  /*  ��������һ�ζ�?   ?*/
//										Distance();//�������
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
//											 Distance();//�������
//										 }

//											/*  ǰ�������λ**/
//										 //ÿ���˶����ƶ���ͬʱ�� �Ǹ��� Ȼ���˻�
//										  flag_of_putting = 0;
//										  moving_cnt = 0;
//									  
//										  while(flag_of_putting==0){
//												DistanceA();//̽���һ���ո�
//												if(distance_cm< EMPTY){//����С�ڶ��� ��ʾ�ж����ˣ�Ȼ���ٲ�����һ��
//													DistanceB();
//													if(distance_cm< EMPTY){//����С�ڶ��� ��ʾ�ж�����,��ʱӦ����ǰ�ƶ���
//														moving_cnt++;//˵��������Ӳ��У���Ҫ����ǰ
//														CarBack();
//														Delayms(17936);//�����ֵ��֤ÿ���ƶ�һ�����ӣ����ﲻ������ĺڸ��ӣ����ǿ����ܵĸ���
//														CarStop();
//														
//													}else{//��ʾ̽���λ�ÿ��ԷŶ���
//														flag_of_putting=1;
//													/*�Ŷ�������*/
//													}
//													
//													
//												}else{//��ʾ̽���λ�ÿ��ԷŶ���
//													flag_of_putting=1;
//													/*�Ŷ�������*/
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
//											/*�ص���λλ��,��Ȼ��ʹ�õ����ϴε�moving_cnt*/
//										
//										  CarBack();
//											Delayms(moving_cnt*17936);
//											CarStop();
//											/*�����ʼλ��*/
//										 need_distance(17936);
//											CarGo();
//											Delayms(17936);
//											need_distance(17936);
//											
//										 
//										  /*���ε���*/
//										
//										 CarBackRight();//��������е���� Ϊʲô���ҹ� ��Ϊ�ҹ�֮�� ��ͷ����̽�����
//										 Delayms(17936);
//										 CarStop();
//										 //ÿ��ת�䶼Ҫ��λ
//										  Distance();//�������
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//�������
//										 }
//										 
//										 
//										 
//										 
//										 
//										 
//										 CarBack();  //��Ϊ�ϴ���Right ����һ����Back
//										 Delayms(17936);
//										 
////										 //�ص���ʼλ��
//										 Distance();//�������
//										 while(distance_cm < 17936 && distance_cm > 17936)
//										 {	 
//											 if(distance_cm < 17936){
//												 CarBack();
//												 Delayms(10);
//											 }else if(distance_cm > 17936){
//												 CarBack();
//												 Delayms(10);
//											 }
//											 Distance();//�������
//										 }
//										 //������ݵ��˵ľ��룬object���м���
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
//											 //����Ҫ�ı���
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
//												DistanceA();//̽���һ���ո�
//												if(distance_cm< EMPTY){//����С�ڶ��� ��ʾ�ж����ˣ�Ȼ���ٲ�����һ��
//													DistanceB();
//													if(distance_cm< EMPTY){//����С�ڶ��� ��ʾ�ж�����,��ʱӦ����ǰ�ƶ���
//														moving_cnt++;//˵��������Ӳ��У���Ҫ����ǰ
//														CarBack();
//														Delayms(17936);//�����ֵ��֤ÿ���ƶ�һ�����ӣ����ﲻ������ĺڸ��ӣ����ǿ����ܵĸ���
//														CarStop();
//														
//													}else{//��ʾ̽���λ�ÿ��ԷŶ���
//														flag_of_putting=1;
//													/*�Ŷ�������*/
//													}
//													
//													
//												}else{//��ʾ̽���λ�ÿ��ԷŶ���
//													flag_of_putting=1;
//													/*�Ŷ�������*/
//												}
//												
//											}
//											flag_of_putting = 0;
//											
//											
//											//Ȼ���Ҫ��ȥ��
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





























////����

//before



		
 }
















































