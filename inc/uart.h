#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

void init_uart(void);
void write_canal(unsigned short count);
void USART1_IRQHandler(void);
void DMA1_Channel2_3_IRQHandler(void);
unsigned long get_pc_tmr(void);
unsigned char* get_tx_ptr(void);
unsigned char* get_rx_ptr(void);
unsigned short get_rx_cnt(void);
void clear_rx_cnt(void);
#endif /* UART_H_INCLUDED */
