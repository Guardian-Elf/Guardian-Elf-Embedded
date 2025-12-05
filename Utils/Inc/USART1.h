//
// Created by pc on 2025/11/30.
//

#ifndef STM32F103ZET6_USART_UTIL_H
#define STM32F103ZET6_USART_UTIL_H

#include "stm32f103xb.h"

// 接受缓冲区
#define USART1_R_SIZE 300
// 发送缓冲区
#define USART1_T_SIZE 300


// 初始化
void USART1_Init(void);

// 发送一个字符
void USART1_SendByte(uint8_t byte);


// 发送字符串
void USART1_SendString(char *str);

void USART1_SendStringWithLen(char *str, int len);

int USART1_Printf(const char *format, ...);

// 中断函数
void USART1_IRQHandler(void);

// 回调
void USART1_ReceiveCallback(void);

// 接收缓冲区相关（根据需求添加）
extern uint8_t USART1_buffer[USART1_R_SIZE];
extern uint32_t USART1_size;


#endif //STM32F103ZET6_USART_UTIL_H
