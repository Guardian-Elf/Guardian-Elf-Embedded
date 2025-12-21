//
// Created by pc on 2025/12/6.
//

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "OLED_Menu.h"
#include "USART.h"

// 页缓存
uint8_t OLED_Pages[OLED_PAGE_NUM][8][128];
// 当前页index
uint8_t OLED_Page_Id = 0;
// 记录以及使用的页码
uint8_t OLED_Page_Used[OLED_PAGE_NUM] = {0};

// 雪花屏样本数据，用于实现渐变切屏
uint8_t OLED_Snow_Screen[8][128];
// 是否初始化
bool OLED_Is_Snow_Init = false;

// 切换模式
enum OLED_Switch_Mode OLED_Switch_Mode = OLED_SWITCH_MODE_VERTICAL;

// 动态切换，向上滑动（带步长）
void OLED_SwitchPage_Dynamic_Up_With_Step(uint8_t step) {
    for (int move = 0; move < 8; move += step) {
        // 每次移动step行
        for (int i = 0; i < 8 - step; i++) {
            memcpy(OLED_DisplayBuf[i], OLED_DisplayBuf[i + step],
                   sizeof(OLED_DisplayBuf[0]));
        }

        // 填充底部新的数据
        for (int i = 0; i < step; i++) {
            if (move + i < 8) {
                memcpy(OLED_DisplayBuf[8 - step + i],
                       OLED_Pages[OLED_Page_Id][move + i],
                       sizeof(OLED_DisplayBuf[0]));
            }
        }

        OLED_Update();
    }

    // 确保最终显示完整页面
    OLED_SwitchPage(OLED_Page_Id);
    OLED_Update();
}

// 动态切换，向下滑动（带步长）
void OLED_SwitchPage_Dynamic_Down_With_Step(uint8_t step) {
    for (int move = 0; move < 8; move += step) {
        // 每次移动step行
        for (int i = 7; i >= step; i--) {
            memcpy(OLED_DisplayBuf[i], OLED_DisplayBuf[i - step],
                   sizeof(OLED_DisplayBuf[0]));
        }

        // 填充顶部新的数据
        for (int i = 0; i < step; i++) {
            if (move + i < 8) {
                memcpy(OLED_DisplayBuf[i],
                       OLED_Pages[OLED_Page_Id][7 - (move + i)],
                       sizeof(OLED_DisplayBuf[0]));
            }
        }

        OLED_Update();
    }

    // 确保最终显示完整页面
    OLED_SwitchPage(OLED_Page_Id);
    OLED_Update();
}

// 动态切换，向左滑动
void OLED_SwitchPage_Dynamic_Left_With_Step(uint8_t step) {
    for (int i = 0; i < (int) (128 / step); i++) {
        for (int j = 0; j < (int) (128 / step) - 1; j++) {
            for (int k = 0; k < 8; k++) {
                for (int p = 0; p < step; p++) {
                    OLED_DisplayBuf[k][j * step + p] =
                            OLED_DisplayBuf[k][j * step + step + p];
                }
            }

        }
        for (int k = 0; k < 8; k++) {
            for (int p = 0; p < step; p++) {
                OLED_DisplayBuf[k][127 - step + p] =
                        OLED_Pages[OLED_Page_Id][k][i * step + p];
            }

        }

        OLED_Update();

    }
    OLED_SwitchPage(OLED_Page_Id);
    OLED_Update();
}

// 动态切换，向右滑动
void OLED_SwitchPage_Dynamic_Right_With_Step(uint8_t step) {
    for (int i = (int) (128 / step) - 1; i >= 0; i--) {
        for (int j = (int) (128 / step) - 1; j >= 1; j--) {
            for (int k = 0; k < 8; k++) {
                for (int p = 0; p < step; p++) {
                    OLED_DisplayBuf[k][j * step + p] =
                            OLED_DisplayBuf[k][j * step - step + p];
                }
            }

        }
        for (int k = 0; k < 8; k++) {
            for (int p = 0; p < step; p++) {
                OLED_DisplayBuf[k][p] =
                        OLED_Pages[OLED_Page_Id][k][i * step + p];
            }

        }

        OLED_Update();

    }
    OLED_SwitchPage(OLED_Page_Id);
    OLED_Update();
}

/**
 * 初始化雪花屏（简单版）
 * 生成随机的雪花噪点，密度约30%
 */
void OLED_Snow_Init(void) {
    // 使用系统时间作为随机种子
    uint32_t seed = HAL_GetTick();

    for (uint8_t page = 0; page < 8; page++) {
        for (uint8_t col = 0; col < 128; col++) {
            // 简单的随机数生成
            seed = seed * 1103515245 + 12345;
            uint32_t rand_val = (seed >> 16) & 0xFFFF;

            // 每个像素有30%概率点亮
            uint8_t byte_data = 0;
            for (uint8_t bit = 0; bit < 8; bit++) {
                if ((rand_val >> bit) & 0x01) {  // 使用随机值的不同位
                    if ((rand_val >> (bit + 8)) % 100 < 30) {  // 30%概率
                        byte_data |= (1 << bit);
                    }
                }
            }

            OLED_Snow_Screen[page][col] = byte_data;
        }
    }
}

// 动态切换，渐变
void OLED_SwitchPage_Gradient(uint8_t page_id, uint8_t n, bool is_snow) {
    if (!OLED_Is_Snow_Init && is_snow) {
        OLED_Snow_Init();
    }
    OLED_Page_Id = page_id;
    // 是否渐变完成标志
    bool is_gradient_done = false;
    // 最多渐变次数
    for (int i = 0; i < n; i++) {
        if (i > (int) (n / 2)) {
            is_gradient_done = true;
        }
        // 遍历页面像素，随机改变
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 128; k++) {
                if (i <= (int) (n / 2) && is_snow) {
                    if (rand() % (n - i * 2)  == 0) {
                        OLED_DisplayBuf[j][k] = OLED_Snow_Screen[j][k];
                    }
                    continue;
                }

                if (OLED_DisplayBuf[j][k] != OLED_Pages[OLED_Page_Id][j][k]) {
                    is_gradient_done = false;
                    if (rand() % (n - i) == 0) {
                        OLED_DisplayBuf[j][k] = OLED_Pages[OLED_Page_Id][j][k];
                    }
                }
            }
        }
        OLED_Update();

        if (is_gradient_done) {
            break;
        }
    }
    OLED_SwitchPage(OLED_Page_Id);
    OLED_Update();
}

// 渐变切换页(标准参数)
void OLED_SwitchPage_Gradient_S(uint8_t page_id, bool is_snow) {
    OLED_SwitchPage_Gradient(page_id, 20, is_snow);
}

// 切换页面：将缓存内容拷贝到OLED_DisplayBuf
void OLED_SwitchPage(uint8_t page_id) {
    OLED_Page_Id = page_id;
    memcpy(OLED_DisplayBuf, OLED_Pages[OLED_Page_Id],
           sizeof(OLED_DisplayBuf));  // 拷贝整个显存数组
    OLED_Update();  // 调用OLED驱动更新显示
}

// 切换页面：将缓存内容拷贝到OLED_DisplayBuf
void OLED_SwitchPage_Dynamic_With_Step(uint8_t page_id, enum OLED_Switch_Type type, uint8_t step) {
    OLED_Page_Id = page_id;
    switch (type) {
        case OLED_SWITCH_DYNAMIC_UP:
            OLED_SwitchPage_Dynamic_Up_With_Step(step);
            break;
        case OLED_SWITCH_DYNAMIC_DOWN:
            OLED_SwitchPage_Dynamic_Down_With_Step(step);
            break;
        case OLED_SWITCH_DYNAMIC_LEFT:
            OLED_SwitchPage_Dynamic_Left_With_Step(step);
            break;
        case OLED_SWITCH_DYNAMIC_RIGHT:
            OLED_SwitchPage_Dynamic_Right_With_Step(step);
            break;
        default:
            break;
    }
    OLED_Update();  // 调用OLED驱动更新显示

}

void OLED_SwitchPage_Dynamic(uint8_t page_id, enum OLED_Switch_Type type) {
    OLED_Page_Id = page_id;
    switch (type) {
        case OLED_SWITCH_DYNAMIC_UP:
            OLED_SwitchPage_Dynamic_Up_With_Step(1);
            break;
        case OLED_SWITCH_DYNAMIC_DOWN:
            OLED_SwitchPage_Dynamic_Down_With_Step(1);
            break;
        case OLED_SWITCH_DYNAMIC_LEFT:
            OLED_SwitchPage_Dynamic_Left_With_Step(16);
            break;
        case OLED_SWITCH_DYNAMIC_RIGHT:
            OLED_SwitchPage_Dynamic_Right_With_Step(16);
            break;
        default:
            break;

    }
    OLED_Update();  // 调用OLED驱动更新显示
}

// 保存页面：将OLED_DisplayBuf的内容拷贝到缓存
void OLED_SavePage() {
    memcpy(OLED_Pages[OLED_Page_Id], OLED_DisplayBuf,
           sizeof(OLED_DisplayBuf));  // 拷贝整个显存数组
    OLED_Page_Used[OLED_Page_Id] = 1;
}

// 添加页面
void OLED_AddPage() {
    bool is_add = true;
    int new_page = (OLED_Page_Id + 1) % OLED_PAGE_NUM;
    while (is_add) {
        if (new_page == OLED_Page_Id) {
            USART1_Printf("已满，无法添加页面\r\n");
            return;
        }

        if (OLED_Page_Used[new_page] == 0) {
            OLED_Page_Id = new_page;
            is_add = false;
            return;
        }
        new_page = (new_page + 1) % OLED_PAGE_NUM;
    }

}

// 下一页
void OLED_NextPage() {
    OLED_SavePage();
    bool is_next = true;
    int next_page = (OLED_Page_Id + 1) % OLED_PAGE_NUM;
    while (is_next) {
        if (OLED_Page_Used[next_page] == 1) {
            if (OLED_Switch_Mode == OLED_SWITCH_MODE_HORIZONTAL) {
                OLED_SwitchPage_Dynamic(next_page, OLED_SWITCH_DYNAMIC_LEFT);
            } else if (OLED_Switch_Mode == OLED_SWITCH_MODE_VERTICAL) {
                OLED_SwitchPage_Dynamic(next_page, OLED_SWITCH_DYNAMIC_UP);
            } else if (OLED_Switch_Mode == OLED_SWITCH_MODE_GRADIENT) {
                OLED_SwitchPage_Gradient_S(next_page, false);
            } else if (OLED_Switch_Mode == OLED_SWITCH_MODE_SNOWFLAKE) {
                OLED_SwitchPage_Gradient_S(next_page, true);
            }
            is_next = false;
            return;
        }
        next_page = (next_page + 1) % OLED_PAGE_NUM;
    }
}

// 上一页
void OLED_PrevPage() {
    OLED_SavePage();
    bool is_prev = true;
    int prev_page = (OLED_Page_Id - 1 + OLED_PAGE_NUM) % OLED_PAGE_NUM;
    while (is_prev) {
        if (OLED_Page_Used[prev_page] == 1) {
            if (OLED_Switch_Mode == OLED_SWITCH_MODE_HORIZONTAL) {
                OLED_SwitchPage_Dynamic(prev_page, OLED_SWITCH_DYNAMIC_RIGHT);
            } else if (OLED_Switch_Mode == OLED_SWITCH_MODE_VERTICAL) {
                OLED_SwitchPage_Dynamic(prev_page, OLED_SWITCH_DYNAMIC_DOWN);
            } else if (OLED_Switch_Mode == OLED_SWITCH_MODE_GRADIENT) {
                OLED_SwitchPage_Gradient_S(prev_page,  false);
            } else if (OLED_Switch_Mode == OLED_SWITCH_MODE_SNOWFLAKE) {
                OLED_SwitchPage_Gradient_S(prev_page, true);
            }
            is_prev = false;
            return;
        }
        prev_page = (prev_page - 1 + OLED_PAGE_NUM) % OLED_PAGE_NUM;
    }
}

void OLED_ChangeMode(enum OLED_Switch_Mode mode) {
    OLED_Switch_Mode = mode;
}

