//
// Created by pc on 2025/11/30.
//

#include "USART_demo.h"
#include "USART1.h"
#include "stm32f1xx_hal.h"
#include "USART2.h"
#include "GP10.h"
#include "USART3.h"

//void USART1_ReceiveCallback() {
//    USART1_Printf((char *) USART1_buffer);
//}

void USART2_ReceiveCallback() {
//    USART1_Printf((char *) USART2_buffer);
    USART2_Printf((char *) USART2_buffer);
//    GP10Data data;
//    GP10_DataConvert((char *) USART2_buffer, &data);
//    char buf[1024];
//    Gp10_Data2Json(&data, buf, sizeof(buf), true);
//    USART1_Printf(buf);
//    Gp10_Data2ChineseInfo(&data, buf, sizeof(buf));
//    USART1_Printf(buf);
//    Gp10_Data2SimpleChinese(&data, buf, sizeof(buf));
//    USART1_Printf(buf);


}

void USART_demo01(void) {
    USART1_Init();

    USART1_SendByte('a');
    char str[] = "Hello World ppp!\r\n";
    const char str02[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                          't', 'u', 'v', 'w', 'x', 'y', 'z', '\r', '\n'};

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
    char str[] = "Hello World ppp!\r\n";
    const char str02[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                          't', 'u', 'v', 'w', 'x', 'y', 'z', '\r', '\n'};

    USART2_Printf("Hello printf World! %s\r\n", str02);

    while (1) {
        USART2_SendString("Hello World!");
        HAL_Delay(1000);
        HAL_Delay(100);
    }

}

void USART_demo03(void) {
    USART3_Init();

    USART3_SendByte('a');
    char str[] = "Hello World ppp!\r\n";
    const char str02[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                          't', 'u', 'v', 'w', 'x', 'y', 'z', '\r', '\n'};

    USART3_Printf("Hello printf World! %s\r\n", str02);

    while (1) {
        USART3_SendString("Hello World!");
        HAL_Delay(1000);
        HAL_Delay(100);
    }

}

void USART_demo04(void) {
    USART1_Init();
    USART2_Init();
    USART1_Printf("Hello printf World! %s\r\n");


    while (1) {

        HAL_Delay(1000);
    }

}