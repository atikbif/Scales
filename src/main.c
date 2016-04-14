/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f0xx_conf.h"

#include "crc.h"
#include "modbus.h"
#include "uart.h"
#include "settings.h"
#include "weight_sensor.h"

volatile unsigned short gl_tmr = 0;
request req;
unsigned long w1=0,w2=0;
unsigned short full_weight = 0;
unsigned short weight=0;
unsigned short bufWeight[8];
unsigned long sumWeight;
unsigned char buf_cnt=0;
unsigned short adc = 0;

void led_init(void);
void toggle_led(void);
void led_on(void);
void led_off(void);
void delay_ms(unsigned short value);
static void powerOn(void);  // индикация работоспособности устройства

int main(void)
{
    unsigned short tmp;
    unsigned char* rx_buf;
    unsigned char* tx_buf;
    unsigned short measure_cnt=0;

    FLASH_Unlock();

    rx_buf = get_rx_ptr();
    tx_buf = get_tx_ptr();
    req.tx_buf = tx_buf; req.rx_buf = rx_buf;

    if (SysTick_Config(SystemCoreClock / 1000)) {/* Capture error */ while (1);}

    led_init();
    init_uart();
    init_settings();
    weight_sensor_init();

    powerOn();

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_64); // IWDG counter clock: 40KHz(LSI) / 64  (1.6 ms)
    IWDG_SetReload(150); //Set counter reload value
    IWDG_ReloadCounter();
    IWDG_Enable();

    for(tmp=0;tmp<8;tmp++) {bufWeight[tmp]=0;}

    while(1)
    {
        if((get_rx_cnt())&&(get_pc_tmr()>10))
		{
		    toggle_led();
		    if((rx_buf[0]==get_net_address() || (rx_buf[0]==0))&&(GetCRC16(rx_buf,get_rx_cnt())==0))
			{
			    set_cur_net_address(rx_buf[0]);
				// разбор команд
				switch(rx_buf[1])
				{
				    case 0x00:break;
				    case 0x03:
						req.addr=((unsigned short)rx_buf[2]<<8) | rx_buf[3];
						req.cnt=((unsigned short)rx_buf[4]<<8) | rx_buf[5];
						tmp = read_holdregs(&req);
						write_canal(tmp);
						break;
                    case 0x06:
						req.addr=((unsigned short)rx_buf[2]<<8) | rx_buf[3];
						req.cnt=((unsigned short)rx_buf[4]<<8) | rx_buf[5];
						tmp = write_single_reg(&req);
						write_canal(tmp);
						break;
					case 0x10:
						req.addr=((unsigned short)rx_buf[2]<<8) | rx_buf[3];
						req.cnt=((unsigned short)rx_buf[4]<<8) | rx_buf[5];
						tmp=write_multi_regs(&req);
						write_canal(tmp);
						break;
				}
			}
			clear_rx_cnt();
		}
		delay_ms(5);
		IWDG_ReloadCounter();
		measure_cnt++;
		if(measure_cnt>=20) {
    		measure_cnt = 0;
    		if(isWeightSensorReady(1)&&isWeightSensorReady(2))  {
                getWeight(&w1,&w2);
                adc = (w1>>8) + (w2>>8);
                full_weight = ((unsigned long)((w1>>8) + (w2>>8)))*get_coeff()/10000;
                if(full_weight>=get_platform_weight()) bufWeight[buf_cnt] = full_weight - get_platform_weight();
                else bufWeight[buf_cnt] = 0;
                buf_cnt++;
                if(buf_cnt>=8) {
                    sumWeight = 0;
                    for(tmp=0;tmp<8;tmp++) {sumWeight+=bufWeight[tmp];bufWeight[tmp]=0;}
                    weight = sumWeight>>3;
                    buf_cnt = 0;
                }
    		}
		}
    }
}

void led_init(void)
{
    /*RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_9);*/
}

void toggle_led(void)
{
    //GPIOA->ODR ^= GPIO_Pin_9;
}

void led_on(void)
{
    //GPIO_ResetBits(GPIOA, GPIO_Pin_9);
}

void led_off(void)
{
    //GPIO_SetBits(GPIOA, GPIO_Pin_9);
}

void delay_ms(unsigned short value)
{
    unsigned short wait_tmr = gl_tmr + value;
    while(gl_tmr!=wait_tmr);
}

void powerOn(void)
{
    unsigned char tmp = 0;
    for(tmp=0;tmp<5;tmp++) {
        led_off();
        delay_ms(200);
        led_on();
        delay_ms(50);
    }
    led_off();
}
