//
// Created by pc on 2025/12/5.
//

#include <stdarg.h>
#include <stdio.h>
#include "USART3.h"

// USART3 接收缓冲区
uint8_t USART3_buffer[USART3_R_SIZE] = {0};
uint32_t USART3_size = 0;

/**
  * @brief  USART3初始化函数
  * @note   使用默认引脚 PB10(TX), PB11(RX)
  *         波特率: 115200
  */
void USART3_Init(void) {
    // 1. 时钟使能
    // GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // USART3时钟（APB1总线）
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // 2. 引脚配置 - PB10(TX), PB11(RX)

    // 配置PB10为复用推挽输出 (USART3_TX)
    GPIOB->CRH &= ~GPIO_CRH_CNF10;  // 清除配置位
    GPIOB->CRH |= GPIO_CRH_CNF10_1; // 复用推挽输出
    GPIOB->CRH |= GPIO_CRH_MODE10;  // 输出模式，最大速度50MHz

    // 配置PB11为浮空输入 (USART3_RX)
    GPIOB->CRH &= ~GPIO_CRH_MODE11; // 输入模式
    GPIOB->CRH &= ~GPIO_CRH_CNF11;  // 清除配置位
    GPIOB->CRH |= GPIO_CRH_CNF11_0; // 浮空输入

    // 3. USART3配置
    // 波特率 115200 (APB1时钟36MHz)
    // 计算: 36000000 / (16 * 115200) ≈ 19.53125
    // BRR = 19.53125 * 16 = 312.5 ≈ 0x138
    USART3->BRR = 0x138;

    // 使能USART3，发送和接收
    USART3->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

    // 数据格式：8位数据位，无校验，1位停止位
    USART3->CR1 &= ~USART_CR1_M;    // 8位数据
    USART3->CR1 &= ~USART_CR1_PCE;  // 无校验
    USART3->CR2 &= ~USART_CR2_STOP; // 1位停止位

    // 4. 中断配置
    // 使能接收中断和空闲中断
    USART3->CR1 |= USART_CR1_IDLEIE | USART_CR1_RXNEIE;

    // 设置中断优先级并启用
    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, 3);  // 与USART1/2相同的优先级
    NVIC_EnableIRQ(USART3_IRQn);
}

/**
  * @brief  发送单个字节
  * @param  byte: 要发送的字节
  */
void USART3_SendByte(uint8_t byte) {
    // 等待发送缓冲区为空
    while ((USART3->SR & USART_SR_TXE) == 0);

    // 写入数据寄存器
    USART3->DR = byte;
}

/**
  * @brief  发送字符串
  * @param  str: 要发送的字符串（以'\0'结尾）
  */
void USART3_SendString(char *str) {
    while (*str != '\0') {
        USART3_SendByte(*str);
        str++;
    }
}

/**
  * @brief  发送指定长度的字符串
  * @param  str: 字符串指针
  * @param  len: 发送长度
  */
void USART3_SendStringWithLen(char *str, int len) {
    while (len--) {
        USART3_SendByte(*str);
        str++;
    }
}

/**
  * @brief  格式化输出函数
  * @param  format: 格式化字符串
  * @return 实际发送的字符数
  */
int USART3_Printf(const char *format, ...) {
    char buffer[USART3_T_SIZE] = {'\0'};
    va_list args;
    int len;

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len > 0) {
        // 发送字符串
        char *p = buffer;
        while (*p) {
            USART3_SendByte(*p++);
        }
    }

    return len;
}

/**
  * @brief  启动接收（如果需要手动启动）
  */
void USART3_StartReceive(void) {
    // 如果使用DMA或需要手动启动接收，可以在这里实现
    // 当前使用中断方式，无需特别启动
}

/**
  * @brief  清除接收缓冲区
  */
void USART3_ClearBuffer(void) {
    USART3_size = 0;
    for (int i = 0; i < USART3_R_SIZE; i++) {
        USART3_buffer[i] = 0;
    }
}

/**
  * @brief  USART3中断服务函数
  */
void USART3_IRQHandler(void) {
    // 处理接收中断（接收到数据）
    if (USART3->SR & USART_SR_RXNE) {
        if (USART3_size < USART3_R_SIZE - 1) {
            USART3_buffer[USART3_size++] = USART3->DR;
        }
    }
        // 处理空闲中断（接收线路空闲）
    else if (USART3->SR & USART_SR_IDLE) {
        // 读取DR寄存器以清除IDLE标志
        volatile uint32_t temp = USART3->DR;
        (void)temp;  // 避免未使用变量警告

        // 添加字符串结束符
        if (USART3_size < USART3_R_SIZE) {
            USART3_buffer[USART3_size] = '\0';
        }

        // 调用回调函数
        USART3_ReceiveCallback();

         USART3_size = 0;
    }
}

/**
  * @brief  接收回调函数（弱定义）
  * @note   用户需要在其他文件中重写此函数以实现具体功能
  */
__attribute__((weak)) void USART3_ReceiveCallback(void) {
    // 弱定义，用户可在其他文件中重新实现
    USART3_Printf("默认实现打印数据: %s\r\n", USART3_buffer);
}
