#ifndef __MPU6050_H
#define __MPU6050_H
#include "main.h"

// 数据结构体
typedef struct MPU6050Data
{
    // 原始数据
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
    int16_t temp;

    // 转换后的数据
    float ax_t;
    float ay_t;
    float az_t;
    float gx_t;
    float gy_t;
    float gz_t;
    float temp_t;

    float totalAccel;


}MPU6050Data;

extern MPU6050Data MPU6050_Data;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_ReadGyro();
void MPU6050_ReadAccel();
void MPU6050_ReadTemp();

#endif
