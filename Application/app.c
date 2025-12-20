//
// Created by pc on 2025/12/5.
//

#include "app.h"
#include "USART1.h"
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

double t,  p;
float h;

void App(void) {
    /*模块初始化*/
    Delay_Init();
    USART1_Init();
    OLED_Init();
    I2C_Init();
    BMP280_Init();
    AHT20_Init();
    MPU6050_Init();

    USART_Service_Init();
    Interrupt_Service_Init();
    OLED_Show_BaseInfo();

    uint8_t devices[5] = {0};
    uint8_t num = I2C_ScanDevice(devices, 5);

    while (1)
    {
        MPU6050_ReadAccel();
        MPU6050_ReadGyro();
        MPU6050_ReadTemp();
        OLED_ShowFloatNum(0, 0, MPU6050_Data.ax_t, 3, 2, OLED_8X16);					//OLED显示数据
        OLED_ShowFloatNum(0, 16, MPU6050_Data.ay_t, 3, 2,OLED_8X16);
        OLED_ShowFloatNum(0, 32, MPU6050_Data.az_t, 3, 2,OLED_8X16);
        OLED_ShowFloatNum(64, 0, MPU6050_Data.gx_t, 3, 2,OLED_8X16);
        OLED_ShowFloatNum(64, 16, MPU6050_Data.gy_t, 3, 2,OLED_8X16);
        OLED_ShowFloatNum(64, 32, MPU6050_Data.gz_t, 3, 2,OLED_8X16);
//        OLED_ShowSignedNum(0, 0, MPU6050_Data.ax, 5, OLED_8X16);					//OLED显示数据
//        OLED_ShowSignedNum(0, 16, MPU6050_Data.ay, 5, OLED_8X16);
//        OLED_ShowSignedNum(0, 32, MPU6050_Data.az, 5, OLED_8X16);
//        OLED_ShowSignedNum(64, 0, MPU6050_Data.gx, 5, OLED_8X16);
//        OLED_ShowSignedNum(64, 16, MPU6050_Data.gy, 5, OLED_8X16);
//        OLED_ShowSignedNum(64, 32, MPU6050_Data.gz, 5, OLED_8X16);
//        OLED_ShowSignedNum(48, 48, MPU6050_Data.temp, 5, OLED_8X16);


//        BMP280_ReadData( &p, &t);
//
//        OLED_ShowNum(80, 16, num, 2, OLED_8X16);
//
//
//        OLED_ShowFloatNum(80, 48, t, 2, 2, OLED_8X16);
//        OLED_ShowFloatNum(16, 32, p, 6, 3, OLED_8X16);
//        AHT20_ReadData((float *) &t, &h);
//        OLED_ShowFloatNum(16, 48, h, 2, 2, OLED_8X16);
//        OLED_ShowNum(16, 16, BH1750_GetLux(), 5, OLED_8X16);
        OLED_Update();
    }



}






