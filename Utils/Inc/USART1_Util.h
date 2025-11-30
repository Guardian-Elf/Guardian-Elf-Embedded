//
// Created by pc on 2025/11/30.
//

#ifndef STM32F103ZET6_USART_UTIL_H
#define STM32F103ZET6_USART_UTIL_H
#include "stm32f103xe.h"


// 初始化
void USART1_Init(void);

// 发送一个字符
void USART1_SendByte(uint8_t byte);


// 发送字符串
void USART1_SendString(char *str);

int USART1_Printf(const char *format, ...);

// 中断函数
void USART1_IRQHandler(void);

// 接收缓冲区相关（根据需求添加）
extern uint8_t USART1_buffer[100];
extern uint8_t USART1_size;


#endif //STM32F103ZET6_USART_UTIL_H
