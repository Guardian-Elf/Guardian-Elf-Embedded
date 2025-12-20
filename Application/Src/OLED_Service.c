//
// Created by pc on 2025/12/6.
//

#include "../Inc/OLED_Service.h"
#include "BH1750.h"
#include "I2C.h"
#include "USART1.h"

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
    OLED_ShowString(16, 0, "--环境信息--", OLED_8X16);
    OLED_ShowString(0, 16, "L: ", OLED_8X16);
    OLED_ShowString(0, 32, "P: ", OLED_8X16);
    OLED_ShowString(0, 48, "H: ", OLED_8X16);
    OLED_ShowString(64, 48, "T: ", OLED_8X16);
    OLED_SavePage();

    OLED_Update();

    OLED_ChangeMode(OLED_SWITCH_MODE_VERTICAL);


}