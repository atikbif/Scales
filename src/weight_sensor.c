#include "weight_sensor.h"
#include "stm32f0xx_conf.h"

void weight_sensor_init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_9);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned char isWeightSensorReady(unsigned char canNum)
{
    if(canNum==1) return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)==Bit_RESET);
    if(canNum==2) return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==Bit_RESET);
    return 0;
}

void getWeight(unsigned long* w1, unsigned long* w2)
{
    unsigned long value1 = 0;
    unsigned long value2 = 0;
    unsigned char i;
    if(isWeightSensorReady(1)&&(isWeightSensorReady(2))) {
        for(i=0;i<24;i++) {
            GPIO_SetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_9);
            value1=value1<<1;
            value2=value2<<1;
            GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_9);

            if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)==Bit_RESET) value1++;
            if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==Bit_RESET) value2++;
        }
        for(i=0;i<1;i++) {
            GPIO_SetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_9);

            GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_9);

        }

        //value1=value1^0x800000;
        //value2=value2^0x800000;
    }
    *w1 = value1;
    *w2 = value2;
}
