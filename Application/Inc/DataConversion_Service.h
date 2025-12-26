//
// Created by pc on 2025/12/26.
//

#include <stddef.h>
#include <stdbool.h>

#ifndef GUARDIANELF_DATACONVERSION_SERVICE_H
#define GUARDIANELF_DATACONVERSION_SERVICE_H

// 环境数据结构体
typedef struct {
    // 温度
    float Temperature;
    // 湿度
    float Humidity;
    // 压强
    float Press;
    // 光照强度
    int Lux;
} EnvironmentData;

// 运动数据结构体
typedef struct {
    // x加速度
    float Ax;
    // y加速度
    float Ay;
    // z加速度
    float Az;
    // x角速度
    float Gx;
    // y角速度
    float Gy;
    // z角速度
    float Gz;
} MotionData;

#endif //GUARDIANELF_DATACONVERSION_SERVICE_H

// 将结构体转换位json字符串
void EnvironmentDataToJson(EnvironmentData *data, char *json_str, size_t size, bool indent);

// 将结构体转换位json字符串
void MotionDataToJson(MotionData *data, char *json_str, size_t size, bool indent);