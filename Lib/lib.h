
#ifndef __cpp_test__
#define __cpp_test__
extern "C"
{
#include "stm32f4xx.h"
#include "delay.h"
#include "stm32f4xx_adc.h"
#include "stdlib.h"
//#include "OLED.h"
#include "math.h"
}
//uint32_t APB1Periph_map(TIM_TypeDef* TIMx);
void IT_config(TIM_TypeDef *TIMx,FunctionalState NewState);
void TIM_config(TIM_TypeDef* TIMx,uint16_t Prescaler,uint32_t Period,FunctionalState NewState=DISABLE,uint8_t TIMx_IRQn=TIM3_IRQn);
void ADC_config(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x,ADC_TypeDef* ADCx,uint8_t ADC_Channel_x);
void DAC_config(uint32_t DAC_Channel_x,uint32_t GPIO_Pin_x);
void DMA_config(DMA_Stream_TypeDef* DMAy_Streamx,uint32_t DMA_Channel_x);
TIM_OCInitTypeDef* PWM_config(TIM_TypeDef* TIMx,uint32_t CCR);
void led测试();
class GPIO
{
    private:
        uint32_t AHB1Periph_map(GPIO_TypeDef* GPIOx)
        {
            uint32_t RCC_AHB1Periph_GPIOx;
            if(GPIOx==GPIOA)
            {
                RCC_AHB1Periph_GPIOx=RCC_AHB1Periph_GPIOA;
            }
            else if(GPIOx==GPIOB)
            {
                RCC_AHB1Periph_GPIOx=RCC_AHB1Periph_GPIOB;
            }
            else if(GPIOx==GPIOC)
            {
                RCC_AHB1Periph_GPIOx=RCC_AHB1Periph_GPIOC;
            }
            else if(GPIOx==GPIOD)
            {
                RCC_AHB1Periph_GPIOx=RCC_AHB1Periph_GPIOD;
            }
            else if(GPIOx==GPIOE)
            {
                RCC_AHB1Periph_GPIOx=RCC_AHB1Periph_GPIOE;
            }
            else if(GPIOx==GPIOF)
            {
                RCC_AHB1Periph_GPIOx=RCC_AHB1Periph_GPIOF;
            }
            else if(GPIOx==GPIOG)
            {
                RCC_AHB1Periph_GPIOx=RCC_AHB1Periph_GPIOG;
            }
            return RCC_AHB1Periph_GPIOx;
        }
        GPIOSpeed_TypeDef GPIO_Speed_x=GPIO_Speed_100MHz;
        GPIOMode_TypeDef GPIO_Mode_x=GPIO_Mode_IN;
        GPIOOType_TypeDef GPIO_OType_x=GPIO_OType_PP;
        GPIO_TypeDef* GPIOx;
    public:
        GPIO_InitTypeDef* Gsadder;
        volatile unsigned long* OUTstate;
        volatile unsigned long* INstate;

    public:
        //适用于全通用/输出
        void config(GPIO_TypeDef*GPIOx,uint32_t GPIO_Pin_x,GPIOMode_TypeDef GPIO_Mode_x,GPIOOType_TypeDef GPIO_OType_x=GPIO_OType_PP,GPIOSpeed_TypeDef GPIO_Speed_x=GPIO_Speed_100MHz,GPIOPuPd_TypeDef GPIO_PuPd_x=GPIO_PuPd_UP)
        {
            //uint32_t GPIOx_ODR=(uint32_t)GPIOx+20;
            //uint8_t pin=logf((float)GPIO_Pin_x)/logf(2);
            //uint32_t pin_ODR_Adder=((GPIOx_ODR & 0xF0000000)+0x2000000+((GPIOx_ODR &0xFFFFF)<<5)+(pin<<2));
            OUTstate=((volatile unsigned long  *)((((uint32_t)GPIOx+20) & 0xF0000000)+0x2000000+((((uint32_t)GPIOx+20) &0xFFFFF)<<5)+((uint8_t)(logf((float)GPIO_Pin_x)/logf(2))<<2)));
            INstate=((volatile unsigned long  *)((((uint32_t)GPIOx+16) & 0xF0000000)+0x2000000+((((uint32_t)GPIOx+16) &0xFFFFF)<<5)+((uint8_t)(logf((float)GPIO_Pin_x)/logf(2))<<2)));
            RCC_AHB1PeriphClockCmd(AHB1Periph_map(GPIOx),ENABLE);
            GPIO_InitTypeDef GPIO_InitStruct;
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin=GPIO_Pin_x;
            GPIO_InitStruct.GPIO_Mode=GPIO_Mode_x;
            GPIO_InitStruct.GPIO_OType=GPIO_OType_x;
            GPIO_InitStruct.GPIO_Speed=GPIO_Speed_x;
            GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_x;
            GPIO_Init(GPIOx,&GPIO_InitStruct);
            Gsadder=new GPIO_InitTypeDef;
        }
        
        inline void put(volatile unsigned long a)
        {
            *OUTstate=a;
        }
};

typedef struct
{
    float kp;
    float kd;
    float ki;
    
    float kpl;
    float kdl;
    float kil;

    float kph;
    float kdh;
    float kih;;
    
    float target_value;
    float current_value;
    float error=target_value-current_value;
    float old_error;
    float acc_error;
    float integral;
    float different;
    float result;
    float judge_result;

}PID;

float PID_config(PID*pid,float kp,float kd,float ki);



       


#endif

