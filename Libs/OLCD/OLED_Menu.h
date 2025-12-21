//
// Created by pc on 2025/12/6.
//

#ifndef GUARDIANELF_OLED_MENU_H
#define GUARDIANELF_OLED_MENU_H

// 页数
#include <stdint.h>
#include "OLED.h"
#include <stdbool.h>

#define OLED_PAGE_NUM 8

extern uint8_t OLED_Page_Id;

// 切换种类枚举
enum OLED_Switch_Type {
    OLED_SWITCH_DYNAMIC_UP,
    OLED_SWITCH_DYNAMIC_DOWN,
    OLED_SWITCH_DYNAMIC_LEFT,
    OLED_SWITCH_DYNAMIC_RIGHT,
};

// 切换模式(横向纵向)
enum OLED_Switch_Mode {
    OLED_SWITCH_MODE_HORIZONTAL,
    OLED_SWITCH_MODE_VERTICAL,
    OLED_SWITCH_MODE_GRADIENT,
    OLED_SWITCH_MODE_SNOWFLAKE,
};

// 切换页
void OLED_SwitchPage(uint8_t page);

// 保存页
void OLED_SavePage();

// 动态切换页
void OLED_SwitchPage_Dynamic(uint8_t page_id, enum OLED_Switch_Type type);

// 动态切换页带步长
void OLED_SwitchPage_Dynamic_With_Step(uint8_t page_id, enum OLED_Switch_Type type, uint8_t step);

// 渐变切换页
void OLED_SwitchPage_Gradient(uint8_t page_id, uint8_t n, bool is_snow);

// 渐变切换页(标准参数)
void OLED_SwitchPage_Gradient_S(uint8_t page_id, bool is_snow);


// 添加页
void OLED_AddPage();

// 下一页
void OLED_NextPage();

// 上一页
void OLED_PrevPage();

// 改变切换页面模式(横向纵向)
void OLED_ChangeMode(enum OLED_Switch_Mode mode);


#endif //GUARDIANELF_OLED_MENU_H
