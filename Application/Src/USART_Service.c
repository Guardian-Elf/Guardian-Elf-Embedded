//
// Created by pc on 2025/12/6.
//

#include "../Inc/USART_Service.h"


void USART2_ReceiveCallback() {
    USART1_Printf("串口2中断:定位数据转结构体：\n%s\n");
    GP10_DataConvert((char * )USART2_buffer, &GP10_Data);
}

void USART3_ReceiveCallback() {
    USART1_Printf("串口3中断\n%s\n", USART3_buffer);
}


void USART_Service_Init() {
    USART_Init(USART1, 115200);
//    USART_Init(USART2, 9600);
    USART_Init(USART3, 115200);
}
