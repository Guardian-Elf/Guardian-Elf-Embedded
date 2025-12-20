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
#include "Interrupt_Service.h"
#include "../Libs/MPU6050/MPU6050.h"
#include "MPU6050_Reg.h"

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量

void App(void) {
//    USART_Service_Init();
//    Interrupt_Service_Init();
//    OLED_Show_BaseInfo();

    /*模块初始化*/
    Delay_Init();
    USART1_Init();
    OLED_Init();		//OLED初始化
    MPU6050_Init();		//MPU6050初始化

    /*显示ID号*/
    OLED_ShowString(1, 1, "ID:", OLED_8X16);		//显示静态字符串
    ID = MPU6050_GetID();				//获取MPU6050的ID号
    OLED_ShowHexNum(1, 4, ID, 2, OLED_8X16);		//OLED显示ID号

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
        OLED_ShowFloatNum(48, 48, MPU6050_Data.temp_t, 3, 2,OLED_8X16);
        OLED_ShowSignedNum(48, 48, MPU6050_Data.temp, 5,OLED_8X16);





        OLED_Update();
    }



}






