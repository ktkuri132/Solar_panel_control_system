/*

VS Code is the best IDE

*/

extern "C"
{
#include "stm32f4xx.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "delay.h"
#include "usart.h"	
#include "key16.h"
}

#include "lib.h"
#include "cmath"
extern uint32_t ADC_data;
float angle;
TIM_OCInitTypeDef* TIMOCAdder;
extern int a;
int Pulse=0;
uint16_t hour;
uint16_t min;
uint16_t sec;
PID* pidadder;
int key_sign;
inline void ADC_DataOut()
{
	uint16_t a,b;
	a=*(uint32_t*)0x4001204C;
	delay_ms(1);
	b=*(uint32_t*)0x4001204C;
	if((a-b)<5&&(a-b)>-5)
	{
		ADC_data=(a+b)/2;
		angle=(136-(360.0/4096.0)*(float)ADC_data);
		//TIMOCAdder->TIM_Pulse=(uint32_t)((angle/360)*100);
		//TIM_OC1Init(TIM2,TIMOCAdder);
	}
	else ADC_DataOut();
}
inline void KEY_check()
{
	switch (key_sign)
	{
		case 14:
		{	
			pidadder->target_value=pidadder->target_value-1;
			
		}
		break;
		case 13:
		{	
			pidadder->target_value=pidadder->target_value+1;
			
		}
		break;
		default:break;
	}
	key_sign=0;
}

void order_1();
void order_2();

int main(void)
{
	delay_init(84);
	uart_init(10086);

	OLED_Init();
	ch451_init();
	GPIO led;
	led.config(GPIOF,GPIO_Pin_8|GPIO_Pin_10,GPIO_Mode_OUT);
	*led.OUTstate=0;
	TIM_config(TIM2,840-1,100-1,DISABLE,TIM2_IRQn);
	//TIM_config(TIM3,16800-1,50000-1,ENABLE,TIM3_IRQn);
	
	ADC_config(GPIOA,GPIO_Pin_1,ADC1,ADC_Channel_1);
	TIMOCAdder=PWM_config(TIM2,10);
	
	PID pid;
	pid.kp=0.4;
	pid.kd=17;
	pid.ki=0.01;

	pid.kpl=0.25;//0.5
	pid.kdl=8.2;//16
	pid.kil=0.008;//0.01

	pid.kph=0.41;
	pid.kdh=17;
	pid.kih=0.02;

	pid.integral=200;
	pid.target_value=20; 
	pidadder=&pid;
	
	
	ADC_SoftwareStartConv(ADC1);
	while (1)
	{
		ADC_DataOut();
		if(pid.target_value<=30)
		{
			PID_config(&pid,pid.kpl,pid.kdl,pid.kil);PFout(8)=1;
			
		}
		else if (pid.target_value>30&&pid.target_value<50)
		{
			PID_config(&pid,pid.kp,pid.kd,pid.ki);
		}
		else if(pid.target_value>=50)
		{
			PID_config(&pid,pid.kph,pid.kdh,pid.kih);
		}
		
		
	
		TIMOCAdder->TIM_Pulse=(uint32_t)pid.result;
		TIM_OC1Init(TIM2,TIMOCAdder);
		KEY_check();
		key_sign=ch451_read();
		while (ch451_read()!=0){}
		KEY_check();
		pid.current_value=labs((int32_t)angle);
		OLED_ShowChinese(0,0,"µ±Ç°½Ç¶È");
		OLED_Printf(64,0,OLED_8X16,":%4d",int32_t(angle));
		OLED_Printf(0,16,OLED_8X16,"PWM:%4d",TIMOCAdder->TIM_Pulse);
		OLED_ShowChinese(0,32,"Ö¸¶¨½Ç¶È");
		OLED_Printf(64,32,OLED_8X16,":%2d",(uint16_t)pid.target_value); 
		
		OLED_Printf(0,48,OLED_8X16,"key:%4d",key_sign);
		OLED_Update();
			
		//USART_SendData(USART1,TIMOCAdder->TIM_Pulse);
	}
}

extern "C"
{
	void TIM3_IRQHandler(void)
	{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½
		{
			
			TIM_Cmd(TIM3,DISABLE);
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//ï¿½ï¿½ï¿½ï¿½Ð¶Ï±ï¿½Ö¾Î?
		
	}
	void USART1_IRQHandler(void)
	{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)                                          
		{
			
			pidadder->target_value=USART_ReceiveData(USART1);
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

void order_1()
{
	USART_SendData(USART1,int(angle));
	while(PBin(9)==0){}
	Pulse=Pulse+5;
	if(Pulse>100)
		Pulse=50;
	USART_SendData(USART1,TIMOCAdder->TIM_Pulse);
}

void order_2()
{
	USART_SendData(USART1,int(angle));
	while(PBin(8)==0){}
	Pulse=Pulse-5;
	if(Pulse<0)
		Pulse=50;
	USART_SendData(USART1,TIMOCAdder->TIM_Pulse);
}


