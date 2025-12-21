//
// Created by pc on 2025/12/6.
//

#include "../Inc/USART_Service.h"


void USART2_ReceiveCallback() {
    USART1_Printf("串口2中断(原始定位数据)：\n%s\n", USART2_buffer);
    GP10Data data;
    GP10_DataConvert((char *) USART2_buffer, &data);
    GP10_DataConvert((char *) USART2_buffer, &GP10_Data);
    char buf[1024];
    Gp10_Data2Json(&data, buf, sizeof(buf), true);
    USART2_Printf("JSON格式：%s\n", buf);
    Gp10_Data2ChineseInfo(&data, buf, sizeof(buf));
    USART1_Printf("中文完整信息：%s\n", buf);
    Gp10_Data2SimpleChinese(&data, buf, sizeof(buf));
    USART1_Printf("中文简要信息：%s\n", buf);
}


void USART_Service_Init() {
    USART_Init(USART1, 115200);
//    USART_Init(USART2, 9600);
//    USART_Init(USART3, 115200);
}
