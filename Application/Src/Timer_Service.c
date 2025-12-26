//
// Created by pc on 2025/12/21.
//

#include <stdio.h>
#include "Timer_Service.h"
#include "USART.h"
#include "AHT20.h"
#include "MPU6050.h"
#include "BH1750.h"
#include "BMP280.h"
#include "GP10.h"
#include "DataConversion_Service.h"
#include "W25Q64_Service.h"




// 测量数据并发送
void Timer_Interrupt_2(void) {
    EnvironmentData environmentData;
    MotionData motion_data;


    float t = (float )((AHT20_Temperature + BMP280_Temp) / 2.0);
    environmentData.Temperature = t;
    environmentData.Humidity = AHT20_Humidity;
    environmentData.Press = (float)BMP280_Press;
    environmentData.Lux = (int)BH1750_Lux;

    motion_data.Ax = MPU6050_Data.ax_t;
    motion_data.Ay = MPU6050_Data.ay_t;
    motion_data.Az = MPU6050_Data.az_t;
    motion_data.Gx = MPU6050_Data.gx_t;
    motion_data.Gy = MPU6050_Data.gy_t;
    motion_data.Gz = MPU6050_Data.gz_t;

    char e_data[500] = {0};
    char m_data[500] = {0};
    char json_str[500] = {0};

    EnvironmentDataToJson(&environmentData, e_data, sizeof(e_data), false);
    MotionDataToJson(&motion_data, m_data, sizeof(m_data), false);
    Gp10_Data2Json(&GP10_Data, json_str, sizeof(json_str), false);


    USART1_Printf("发送数据\r\n");
    USART3_Printf((char * )W25Q64_Device_Id);
    USART3_Printf("\n");
    USART3_Printf(json_str);
    USART3_Printf("\n");
    USART3_Printf(e_data);
    USART3_Printf("\n");
    USART3_Printf(m_data);
    USART3_Printf("::end::");
}
