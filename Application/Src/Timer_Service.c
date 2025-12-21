//
// Created by pc on 2025/12/21.
//

#include "Timer_Service.h"
#include "USART.h"

// 测量数据并发送
void Timer_Interrupt_2(void) {
    AHT20_ReadData();
    BH1750_GetLux();
    MPU6050_ReadAccel();
    MPU6050_ReadGyro();
    BMP280_ReadData();

}
