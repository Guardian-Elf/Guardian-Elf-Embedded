//
// Created by pc on 2025/12/2.
//

#ifndef GUARDIANELF_GP10_H
#define GUARDIANELF_GP10_H

#include "GP10_data_struct.h"

// 声明数据结构体
extern GP10Data GP10_Data;

// 数据转换
void GP10_DataConvert(char * str, GP10Data * data);

/**
 * @brief 转换GP10Data定位信息为JSON格式字符串
 * @param data GP10Data结构体指针
 * @param json_str 输出字符数组（缓冲区）
 * @param str_size 字符数组大小
 * @param indent 是否缩进格式化（true: 格式化，false: 紧凑）
 * @return 成功返回0，失败返回-1
 */
int Gp10_Data2Json(const GP10Data *data, char json_str[], size_t str_size, bool indent);

/**
 * @brief 转换GP10Data定位信息为中文信息字符串
 * @param data GP10Data结构体指针
 * @param info_str 输出字符数组（缓冲区）
 * @param str_size 字符数组大小
 * @return 成功返回0，失败返回-1
 */
int Gp10_Data2ChineseInfo(const GP10Data *data, char info_str[], size_t str_size);

/**
 * @brief 简化的中文信息版本（单行显示）
 * @param data GP10Data结构体指针
 * @param info_str 输出字符数组（缓冲区）
 * @param str_size 字符数组大小
 * @return 成功返回0，失败返回-1
 */
int Gp10_Data2SimpleChinese(const GP10Data *data, char info_str[], size_t str_size);

#endif //GUARDIANELF_GP10_H
