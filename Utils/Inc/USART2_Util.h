//
// Created by pc on 2025/11/30.
//

#ifndef STM32F103ZET6_USART2_UTIL_H
#define STM32F103ZET6_USART2_UTIL_H

#include "stm32f103xe.h"

// 接收缓冲区
#define USART2_R_SIZE 300
// 发送缓冲区
#define USART2_T_SIZE 300
// 初始
void USART2_Init(void);

// 发送一个字符
void USART2_SendByte(uint8_t byte);

// 发送字符串
void USART2_SendString(char *str);

// 格式化输出
int USART2_Printf(const char *format, ...);

// 中断函数
void USART2_IRQHandler(void);

// 回调
void USART2_ReceiveCallback();

// 接收缓冲区相关（根据需求添加）
extern uint8_t USART2_buffer[USART2_R_SIZE];
extern uint32_t USART2_size;

#endif //STM32F103ZET6_USART2_UTIL_H