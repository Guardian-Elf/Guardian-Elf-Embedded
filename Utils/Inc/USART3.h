//
// Created by pc on 2025/12/5.
//

#ifndef STM32F103ZET6_USART3_UTIL_H
#define STM32F103ZET6_USART3_UTIL_H

#include "stm32f103xb.h"

// 接收缓冲区大小
#define USART3_R_SIZE 300
// 发送缓冲区大小
#define USART3_T_SIZE 300

// 初始化函数
void USART3_Init(void);

// 发送函数
void USART3_SendByte(uint8_t byte);
void USART3_SendString(char *str);
void USART3_SendStringWithLen(char *str, int len);
int USART3_Printf(const char *format, ...);

// 接收相关函数
void USART3_StartReceive(void);
void USART3_ClearBuffer(void);

// 中断函数
void USART3_IRQHandler(void);

// 回调函数（需用户实现）
void USART3_ReceiveCallback(void);

// 外部变量声明
extern uint8_t USART3_buffer[USART3_R_SIZE];
extern uint32_t USART3_size;

#endif //STM32F103ZET6_USART3_UTIL_H
