//
// Created by pc on 2025/12/5.
//

#include "app.h"
#include "USART.h"
#include "OLED.h"
#include "MPU6050.h"
#include "Delay.h"
#include "Inc/OLED_Service.h"
#include "Inc/USART_Service.h"
#include "BH1750.h"
#include "AHT20.h"
#include "BMP280.h"
#include "Interrupt_Service.h"
#include "../Libs/MPU6050/MPU6050.h"
#include "MPU6050_Reg.h"

void App(void) {
    /*模块初始化*/
    Delay_Init();
    OLED_Init();
    I2C_Init();
    BMP280_Init();
    AHT20_Init();
    MPU6050_Init();

    USART_Service_Init();

    Interrupt_Service_Init();
    OLED_Show_BaseInfo();


    while (1)
    {
        OLED_Update_Data();
    }



}






