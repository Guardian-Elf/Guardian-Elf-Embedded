//
// Created by pc on 2025/11/30.
//

#ifndef STM32F103ZET6_USART_UTIL_H
#define STM32F103ZET6_USART_UTIL_H

#include "stm32f1xx_hal.h"

// 接受缓冲区
#define USART_R_SIZE 500
// 发送缓冲区
#define USART_T_SIZE 500

extern UART_HandleTypeDef huart1;
extern uint8_t USART1_buffer[USART_R_SIZE];
extern uint32_t USART1_size;


extern UART_HandleTypeDef huart2;
extern uint8_t USART2_buffer[USART_R_SIZE];
extern uint32_t USART2_size;



extern UART_HandleTypeDef huart3;
extern uint8_t USART3_buffer[USART_R_SIZE];
extern uint32_t USART3_size;

// 初始化
void USART_Init(USART_TypeDef *USARTx, uint32_t BaudRate);

int USART1_Printf(const char *format, ...);
int USART2_Printf(const char *format, ...);
int USART3_Printf(const char *format, ...);

// 回调
void USART1_ReceiveCallback(void);
void USART2_ReceiveCallback(void);
void USART3_ReceiveCallback(void);

#endif //STM32F103ZET6_USART_UTIL_H
