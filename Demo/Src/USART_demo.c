//
// Created by pc on 2025/11/30.
//

#include "USART_demo.h"
#include "USART1_Util.h"
#include "stm32f1xx_hal.h"
#include "USART2_Util.h"


void USART_demo01(void) {
    USART1_Init();

    USART1_SendByte('a');
    char  str[] = "Hello World ppp!\r\n";
    const char  str02[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\r', '\n'};

    USART1_Printf("Hello printf World! %s\r\n", str02);

    while (1) {
        USART1_SendString("Hello World!");
        HAL_Delay(1000);
        HAL_Delay(100);
    }

}


void USART_demo02(void) {
    USART2_Init();

    USART2_SendByte('a');
    char  str[] = "Hello World ppp!\r\n";
    const char  str02[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\r', '\n'};

    USART2_Printf("Hello printf World! %s\r\n",  str02);

    while (1) {
        USART2_SendString("Hello World!");
        HAL_Delay(1000);
        HAL_Delay(100);
    }

}

void USART_demo03(void) {
    USART1_Init();
    USART2_Init();

    USART2_SendByte('a');
    USART1_SendByte('a');
    char  str[] = "Hello World ppp!\r\n";
    const char  str02[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\r', '\n'};

    USART1_Printf("Hello printf World! %s\r\n", str02);
    USART2_Printf("Hello printf World! %s\r\n",  str02);

    while (1) {
        USART2_SendString("Hello World!");
        USART1_SendString("Hello World!");
        HAL_Delay(1000);
        HAL_Delay(100);
    }

}