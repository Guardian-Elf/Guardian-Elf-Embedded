#include <math.h>
#include "main.h"
#include "I2C.h"
#include "MPU6050_Reg.h"
#include "MPU6050.h"

#define MPU6050_ADDR        0x68        // 7位地址
#define MPU6050_ADDRESS        0xD0        //MPU6050的I2C从机地址

// 根据配置的灵敏度定义
#define ACCEL_SENSITIVITY_2G   16384.0f    // ±2g: 16384 LSB/g
#define ACCEL_SENSITIVITY_4G   8192.0f     // ±4g: 8192 LSB/g
#define ACCEL_SENSITIVITY_8G   4096.0f     // ±8g: 4096 LSB/g
#define ACCEL_SENSITIVITY_16G  2048.0f     // ±16g: 2048 LSB/g

#define GYRO_SENSITIVITY_250   131.0f      // ±250°/s: 131 LSB/°/s
#define GYRO_SENSITIVITY_500   65.5f       // ±500°/s: 65.5 LSB/°/s
#define GYRO_SENSITIVITY_1000  32.8f       // ±1000°/s: 32.8 LSB/°/s
#define GYRO_SENSITIVITY_2000  16.4f       // ±2000°/s: 16.4 LSB/°/s

// 当前配置（根据MPU6050_Init中的配置）
#define CURRENT_ACCEL_SENSITIVITY  ACCEL_SENSITIVITY_2G  // ±16g
#define CURRENT_GYRO_SENSITIVITY   GYRO_SENSITIVITY_250  // ±2000°/s

MPU6050Data MPU6050_Data;

/**
  * 函    数：MPU6050初始化
  * 参    数：无
  * 返 回 值：无
  */
void MPU6050_Init(void) {

    // 先复位设备
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x80);  // 设备复位

    // 取消睡眠模式
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);

    // 先配置为±2g（最低量程）
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x00);  // ±2g
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x00);   // ±250°/s

    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);


}

/**
  * 函    数：MPU6050写寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data) {
    I2C_WriteReg(MPU6050_ADDR, RegAddress, Data);
}

/**
  * 函    数：MPU6050读寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 返 回 值：读取寄存器的数据，范围：0x00~0xFF
  */
uint8_t MPU6050_ReadReg(uint8_t RegAddress) {
    return I2C_ReadReg(MPU6050_ADDR, RegAddress);
}


/**
  * 函    数：MPU6050获取ID号
  * 参    数：无
  * 返 回 值：MPU6050的ID号
  */
uint8_t MPU6050_GetID(void) {
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);        //返回WHO_AM_I寄存器的值
}


// 数据的读取与转换

/**
  * 函    数：原始加速度值转换为g（重力加速度）
  * 参    数：raw 原始ADC值
  * 返 回 值：加速度值（g）
  */
float MPU6050_AccelRawToG(int16_t raw)
{
    return (float)raw / CURRENT_ACCEL_SENSITIVITY;
}

/**
  * 函    数：原始陀螺仪值转换为度/秒
  * 参    数：raw 原始ADC值
  * 返 回 值：角速度值（°/s）
  */
float MPU6050_GyroRawToDPS(int16_t raw)
{
    return (float)raw / CURRENT_GYRO_SENSITIVITY;
}

/**
  * 函    数：原始温度值转换为摄氏度
  * 参    数：raw 原始ADC值
  * 返 回 值：温度值（℃）
  */
float MPU6050_TempRawToCelsius(int16_t raw)
{
    return (float)raw / 340.0f + 36.53f;
}

/**
  * 函    数：计算三轴合成加速度
  * 参    数：ax_g X轴加速度（g）
  * 参    数：ay_g Y轴加速度（g）
  * 参    数：az_g Z轴加速度（g）
  * 返 回 值：合成加速度（g）
  */
float MPU6050_CalculateTotalAccel(float ax_g, float ay_g, float az_g)
{
    return sqrtf(ax_g * ax_g + ay_g * ay_g + az_g * az_g);
}

/**
  * 函    数：一次读取16位
  * 参    数：无
  * 返 回 值：16位数据
  */
int16_t MPU6050_Read16Bit(uint8_t regL, uint8_t regH) {
    return (int16_t)((MPU6050_ReadReg(regH) << 8) | MPU6050_ReadReg(regL));
}

/**
  * 函    数：MPU6050读取加速度计数据
  * 参    数：ax ay az 加速度计X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 返 回 值：无
  */
void MPU6050_ReadAccel() {
    MPU6050_Data.ax = MPU6050_Read16Bit(MPU6050_ACCEL_XOUT_L, MPU6050_ACCEL_XOUT_H);
    MPU6050_Data.ay = MPU6050_Read16Bit(MPU6050_ACCEL_YOUT_L, MPU6050_ACCEL_YOUT_H);
    MPU6050_Data.az = MPU6050_Read16Bit(MPU6050_ACCEL_ZOUT_L, MPU6050_ACCEL_ZOUT_H);
    MPU6050_Data.ax_t = MPU6050_AccelRawToG(MPU6050_Data.ax);
    MPU6050_Data.ay_t = MPU6050_AccelRawToG(MPU6050_Data.ay);
    MPU6050_Data.az_t = MPU6050_AccelRawToG(MPU6050_Data.az);
    MPU6050_Data.totalAccel = MPU6050_CalculateTotalAccel(MPU6050_Data.ax_t, MPU6050_Data.ay_t, MPU6050_Data.az_t);
}

/**
 * 函    数：MPU6050读取温度数据
 * 参    数： temp 温度数据，使用输出参数的形式返回，范围：-32768~32767
 * 返 回 值：无
 */
void MPU6050_ReadTemp() {
    MPU6050_Data.temp = MPU6050_Read16Bit(MPU6050_TEMP_OUT_L, MPU6050_TEMP_OUT_H);
    MPU6050_Data.temp_t = MPU6050_TempRawToCelsius(MPU6050_Data.temp);
}

/**
  * 函    数：MPU6050读取陀螺仪数据
  * 参    数：gx gy gz 陀螺仪X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 返 回 值：无
  */
void MPU6050_ReadGyro() {
    MPU6050_Data.gx = MPU6050_Read16Bit(MPU6050_GYRO_XOUT_L, MPU6050_GYRO_XOUT_H);
    MPU6050_Data.gy = MPU6050_Read16Bit(MPU6050_GYRO_YOUT_L, MPU6050_GYRO_YOUT_H);
    MPU6050_Data.gz = MPU6050_Read16Bit(MPU6050_GYRO_ZOUT_L, MPU6050_GYRO_ZOUT_H);
    MPU6050_Data.gx_t = MPU6050_GyroRawToDPS(MPU6050_Data.gx);
    MPU6050_Data.gy_t = MPU6050_GyroRawToDPS(MPU6050_Data.gy);
    MPU6050_Data.gz_t = MPU6050_GyroRawToDPS(MPU6050_Data.gz);
}


