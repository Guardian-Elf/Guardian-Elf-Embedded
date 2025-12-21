//
// Created by pc on 2025/12/6.
//

#include "../Inc/OLED_Service.h"
#include "BMP280.h"
#include "AHT20.h"
#include "BH1750.h"
#include "MPU6050.h"


void OLED_Show_BaseInfo(void) {
    OLED_ShowString(16, 0, "--时间信息--", OLED_8X16);
    OLED_ShowString(0, 16, "日期: ", OLED_8X16);
    OLED_ShowString(0, 32, "时间: ", OLED_8X16);
    OLED_ShowString(0, 48, "偏移: ", OLED_8X16);
    OLED_SavePage();
    OLED_Clear();

    OLED_AddPage();
    OLED_ShowString(16, 0, "--坐标信息--", OLED_8X16);
    OLED_ShowString(0, 16, "经度: ", OLED_8X16);
    OLED_ShowString(0, 32, "纬度: ", OLED_8X16);
    OLED_ShowString(0, 48, "海拔: ", OLED_8X16);
    OLED_SavePage();
    OLED_Clear();

    OLED_AddPage();
    OLED_ShowString(16, 0, "--运动信息--", OLED_8X16);
    OLED_ShowString(0, 16, "速度: ", OLED_8X16);
    OLED_ShowString(0, 32, "磁偏角: ", OLED_8X16);
    OLED_SavePage();
    OLED_Clear();

    OLED_AddPage();
    OLED_ShowString(16, 0, "--卫星信息--", OLED_8X16);
    OLED_ShowString(0, 16, "GPS: ", OLED_8X16);
    OLED_ShowString(0, 32, "北斗: ", OLED_8X16);
    OLED_SavePage();
    OLED_Clear();

    OLED_AddPage();
    OLED_ShowString(0, 0, "-加速度信息(g)-", OLED_8X16);
    OLED_ShowString(0, 16, "X: ", OLED_8X16);
    OLED_ShowString(0, 32, "Y: ", OLED_8X16);
    OLED_ShowString(0, 48, "Z: ", OLED_8X16);
    OLED_SavePage();
    OLED_Clear();

    OLED_AddPage();
    OLED_ShowString(2, 0, "角速度信息(°/s)", OLED_8X16);
    OLED_ShowString(0, 16, "X: ", OLED_8X16);
    OLED_ShowString(0, 32, "Y: ", OLED_8X16);
    OLED_ShowString(0, 48, "Z: ", OLED_8X16);
    OLED_SavePage();
    OLED_Clear();

    OLED_AddPage();
    OLED_ShowString(16, 0, "--环境信息--", OLED_8X16);
    OLED_ShowString(0, 16, "L: ", OLED_8X16);
    OLED_ShowString(0, 32, "P: ", OLED_8X16);
    OLED_ShowString(0, 48, "H: ", OLED_8X16);
    OLED_ShowString(64, 48, "T: ", OLED_8X16);
    OLED_SavePage();

    OLED_Update();

    OLED_ChangeMode(OLED_SWITCH_MODE_VERTICAL);
}


void OLED_Update_Data(void) {
    static double t, p;
    static float h;

    switch (OLED_Page_Id) {
        case 4:
            MPU6050_ReadAccel();
            OLED_ShowFloatNum(20, 16, MPU6050_Data.ax_t, 3, 2, OLED_8X16);
            OLED_ShowFloatNum(20, 32, MPU6050_Data.ay_t, 3, 2, OLED_8X16);
            OLED_ShowFloatNum(20, 48, MPU6050_Data.az_t, 3, 2, OLED_8X16);

            break;
        case 5:
            MPU6050_ReadGyro();
            OLED_ShowFloatNum(20, 16, MPU6050_Data.gx_t, 3, 2, OLED_8X16);
            OLED_ShowFloatNum(20, 32, MPU6050_Data.gy_t, 3, 2, OLED_8X16);
            OLED_ShowFloatNum(20, 48, MPU6050_Data.gz_t, 3, 2, OLED_8X16);
            break;
        case 6:
            BMP280_ReadData(&p, &t);

            OLED_ShowFloatNum(80, 48, t, 2, 2, OLED_8X16);
            OLED_ShowFloatNum(16, 32, p, 6, 3, OLED_8X16);
            AHT20_ReadData((float *) &t, &h);
            OLED_ShowFloatNum(16, 48, h, 2, 2, OLED_8X16);
            OLED_ShowNum(16, 16, BH1750_GetLux(), 5, OLED_8X16);

            break;
        default:
            break;
    }


    OLED_Update();
}

