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
#include "Timer.h"
#include "AHT20.h"
#include "BMP280.h"
#include "Interrupt_Service.h"
#include "../Libs/MPU6050/MPU6050.h"
#include "MPU6050_Reg.h"
#include "DMA.h"
#include "W25Q64.h"


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
#include "Timer.h"
#include "AHT20.h"
#include "BMP280.h"
#include "Interrupt_Service.h"
#include "../Libs/MPU6050/MPU6050.h"
#include "MPU6050_Reg.h"
#include "DMA.h"
#include "W25Q64.h"
#include "SPI.h"


void App(void) {
    // 初始化硬件
    Delay_Init();
    I2C_Init();
    Timer_Init_2();
    USART_Service_Init();
    Interrupt_Service_Init();

    // 初始化外设
    OLED_Init();
    BMP280_Init();
    AHT20_Init();
    MPU6050_Init();


    // 启动程序


    OLED_Show_BaseInfo();

    while (1)
    {



        OLED_Update_Data();
    }



}





