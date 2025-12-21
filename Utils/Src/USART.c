//
// Created by pc on 2025/11/30.
//

#include <stdarg.h>
#include <stdio.h>
#include "USART.h"

// USART1 接收缓冲区
UART_HandleTypeDef huart1;
uint8_t USART1_buffer[USART_R_SIZE];
uint32_t USART1_size = 0;


// USART2 句柄和缓冲区
UART_HandleTypeDef huart2;
uint8_t USART2_buffer[USART_R_SIZE];
uint32_t USART2_size = 0;

// USART3 句柄和缓冲区
UART_HandleTypeDef huart3;
uint8_t USART3_buffer[USART_R_SIZE];
uint32_t USART3_size = 0;

void USART_Init(USART_TypeDef *USARTx, uint32_t BaudRate) {


    // 初始化配置
    IRQn_Type IRQn;
    UART_HandleTypeDef *huart;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_TypeDef *GPIOx;
    uint32_t TX_Pin, RX_Pin;
    uint8_t *USART_buffer;

    if (USARTx == USART1) {
        huart = &huart1;
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();
        GPIOx = GPIOA;
        TX_Pin = GPIO_PIN_9;
        RX_Pin = GPIO_PIN_10;
        IRQn = USART1_IRQn;
        USART_buffer = USART1_buffer;
    } else if (USARTx == USART2) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART2_CLK_ENABLE();
        GPIOx = GPIOA;
        TX_Pin = GPIO_PIN_2;
        RX_Pin = GPIO_PIN_3;
        huart = &huart2;
        IRQn = USART2_IRQn;
        USART_buffer = USART2_buffer;
    } else if (USARTx == USART3) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_USART3_CLK_ENABLE();
        GPIOx = GPIOB;
        TX_Pin = GPIO_PIN_10;
        RX_Pin = GPIO_PIN_11;
        huart = &huart3;
        IRQn = USART3_IRQn;
        USART_buffer = USART3_buffer;
    } else {
        return;
    }

    // 配置USART1_TX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = TX_Pin;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);



    // 配置为USART1_RX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Pin = RX_Pin;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

    // 3. USART1参数配置
    huart->Instance = USARTx;
    huart->Init.BaudRate = BaudRate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控制
    huart->Init.OverSampling = UART_OVERSAMPLING_16; // 16倍采样

    // 初始化USART1
    HAL_UART_Init(huart);

    // 4. 使能接收中断
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);

    // 5. 配置NVIC
    HAL_NVIC_SetPriority(IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(IRQn);

    // 启动中断接收
    HAL_UARTEx_ReceiveToIdle_IT(huart, USART_buffer, USART_R_SIZE);

}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}

void USART3_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart3);
}

int USART_Printf(UART_HandleTypeDef *huart, const char *format, va_list args) {
    char buffer[USART_T_SIZE] = {'\0'};
    int len = vsnprintf(buffer, sizeof(buffer), format, args);

    if (len > 0) {
        HAL_UART_Transmit(huart, (uint8_t *)buffer, len, HAL_MAX_DELAY);

    }

    return len;

}

int USART1_Printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = USART_Printf(&huart1, format, args);
    va_end(args);
    return len;
}

int USART2_Printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = USART_Printf(&huart2, format, args);
    va_end(args);
    return len;
}

int USART3_Printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = USART_Printf(&huart3, format, args);
    va_end(args);
    return len;
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) {
        USART1_size = Size;
        USART1_ReceiveCallback();
        HAL_UARTEx_ReceiveToIdle_IT(huart, USART1_buffer, USART_R_SIZE);

    } else if (huart->Instance == USART2) {
        USART2_size = Size;
        USART2_ReceiveCallback();
        HAL_UARTEx_ReceiveToIdle_IT(huart, USART2_buffer, USART_R_SIZE);

    } else if (huart->Instance == USART3) {
        USART3_size = Size;
        USART3_ReceiveCallback();
        HAL_UARTEx_ReceiveToIdle_IT(huart, USART3_buffer, USART_R_SIZE);

    }


}

__attribute__((weak)) void USART1_ReceiveCallback(void) {
    // 弱定义，用户可在其他文件中重新实现
    USART1_Printf("df1: %s\r\n", USART1_buffer);
}

__attribute__((weak)) void USART2_ReceiveCallback(void) {
    // 弱定义，用户可在其他文件中重新实现
    USART2_Printf("df2: %s\r\n", USART2_buffer);
}

__attribute__((weak)) void USART3_ReceiveCallback(void) {
    // 弱定义，用户可在其他文件中重新实现
    USART3_Printf("df3: %s\r\n", USART3_buffer);
}