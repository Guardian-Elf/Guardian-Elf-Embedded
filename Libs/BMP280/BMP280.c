//
// Created by pc on 2025/12/20.
//

#include "BMP280.h"

#define BMP280_ADDR 0x77  // 7位写地址

// 寄存器宏
#define BMP280_REG_ID 0xD0 // 设备id
#define BMP280_REG_CONFIG 0xF5 // 配置寄存器
#define BMP280_REG_CTRL_MEAS 0xF4 // 控制测量

#define BMP280_REG_TEMP_XLSB 0xFC // 温度低字节
#define BMP280_REG_TEMP_LSB 0xFB // 温度中字节
#define BMP280_REG_TEMP_MSB 0xFA // 温度高字节

#define BMP280_REG_PRESS_XLSB 0xF9 // 压力低字节
#define BMP280_REG_PRESS_LSB 0xF8 // 压力中字节
#define BMP280_REG_PRESS_MSB 0xF7 // 压力高字节

//补偿寄存器
#define BMP280_DIG_T1_LSB_REG   0x88
#define BMP280_DIG_T1_MSB_REG   0x89
#define BMP280_DIG_T2_LSB_REG   0x8A
#define BMP280_DIG_T2_MSB_REG   0x8B
#define BMP280_DIG_T3_LSB_REG   0x8C
#define BMP280_DIG_T3_MSB_REG   0x8D
#define BMP280_DIG_P1_LSB_REG   0x8E
#define BMP280_DIG_P1_MSB_REG   0x8F
#define BMP280_DIG_P2_LSB_REG   0x90
#define BMP280_DIG_P2_MSB_REG   0x91
#define BMP280_DIG_P3_LSB_REG   0x92
#define BMP280_DIG_P3_MSB_REG   0x93
#define BMP280_DIG_P4_LSB_REG   0x94
#define BMP280_DIG_P4_MSB_REG   0x95
#define BMP280_DIG_P5_LSB_REG   0x96
#define BMP280_DIG_P5_MSB_REG   0x97
#define BMP280_DIG_P6_LSB_REG   0x98
#define BMP280_DIG_P6_MSB_REG   0x99
#define BMP280_DIG_P7_LSB_REG   0x9A
#define BMP280_DIG_P7_MSB_REG   0x9B
#define BMP280_DIG_P8_LSB_REG   0x9C
#define BMP280_DIG_P8_MSB_REG   0x9D
#define BMP280_DIG_P9_LSB_REG   0x9E
#define BMP280_DIG_P9_MSB_REG   0x9F

int t_fine = 0;

struct BMP280_ComReg
{
    uint16_t T1;
    int16_t	T2;
    int16_t	T3;
    uint16_t P1;
    int16_t	P2;
    int16_t	P3;
    int16_t	P4;
    int16_t	P5;
    int16_t	P6;
    int16_t	P7;
    int16_t	P8;
    int16_t	P9;
} BMP280_ComReg;

double BMP280_Temp, BMP280_Press;


uint8_t BMP280_ReadReg(uint8_t reg) {

    return I2C_ReadReg(BMP280_ADDR, reg);
}

void BMP280_WriteReg(uint8_t reg, uint8_t data) {
    I2C_WriteReg(BMP280_ADDR, reg, data);
}

// 获取设备id
uint8_t BMP280_GetDeviceID() {
    return BMP280_ReadReg(BMP280_REG_ID);
}

//  辅助函数，获取完整补偿值
int16_t BMP280_GetCalibrationValue(uint16_t regM, uint16_t regL) {
    return (int16_t)(((uint16_t)BMP280_ReadReg(regM)<<8) | BMP280_ReadReg(regL));
}

void BMP280_Init() {
    BMP280_WriteReg(BMP280_REG_CONFIG, 0x04);
    BMP280_WriteReg(BMP280_REG_CTRL_MEAS, 0x2F);

    //温度传感器的矫正值
    BMP280_ComReg.T1 = BMP280_GetCalibrationValue(BMP280_DIG_T1_MSB_REG, BMP280_DIG_T1_LSB_REG);
    BMP280_ComReg.T2 = BMP280_GetCalibrationValue(BMP280_DIG_T2_MSB_REG, BMP280_DIG_T2_LSB_REG);
    BMP280_ComReg.T3 = BMP280_GetCalibrationValue(BMP280_DIG_T3_MSB_REG, BMP280_DIG_T3_LSB_REG);
    //气压传感器的矫正值
    BMP280_ComReg.P1 = BMP280_GetCalibrationValue(BMP280_DIG_P1_MSB_REG, BMP280_DIG_P1_LSB_REG);
    BMP280_ComReg.P2 = BMP280_GetCalibrationValue(BMP280_DIG_P2_MSB_REG, BMP280_DIG_P2_LSB_REG);
    BMP280_ComReg.P3 = BMP280_GetCalibrationValue(BMP280_DIG_P3_MSB_REG, BMP280_DIG_P3_LSB_REG);
    BMP280_ComReg.P4 = BMP280_GetCalibrationValue(BMP280_DIG_P4_MSB_REG, BMP280_DIG_P4_LSB_REG);
    BMP280_ComReg.P5 = BMP280_GetCalibrationValue(BMP280_DIG_P5_MSB_REG, BMP280_DIG_P5_LSB_REG);
    BMP280_ComReg.P6 = BMP280_GetCalibrationValue(BMP280_DIG_P6_MSB_REG, BMP280_DIG_P6_LSB_REG);
    BMP280_ComReg.P7 = BMP280_GetCalibrationValue(BMP280_DIG_P7_MSB_REG, BMP280_DIG_P7_LSB_REG);
    BMP280_ComReg.P8 = BMP280_GetCalibrationValue(BMP280_DIG_P8_MSB_REG, BMP280_DIG_P8_LSB_REG);
    BMP280_ComReg.P9 = BMP280_GetCalibrationValue(BMP280_DIG_P9_MSB_REG, BMP280_DIG_P9_LSB_REG);
}

double BMP280_compensate_P(int adc_P)
{
    double var1, var2, p;
    var1 = ((double)t_fine/2.0) - 64000.0;
    var2 = var1 * var1 * ((double)BMP280_ComReg.P6) / 32768.0;
    var2 = var2 + var1 * ((double)BMP280_ComReg.P5) * 2.0;
    var2 = (var2/4.0)+(((double)BMP280_ComReg.P4) * 65536.0);
    var1 = (((double)BMP280_ComReg.P3) * var1 * var1 / 524288.0 + ((double)BMP280_ComReg.P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0)*((double)BMP280_ComReg.P1);
    if (var1 == 0.0){
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)BMP280_ComReg.P9) * p * p / 2147483648.0;
    var2 = p * ((double)BMP280_ComReg.P8) / 32768.0;
    p = p + (var1 + var2 + ((double)BMP280_ComReg.P7)) / 16.0;
    return p;
}

double BMP280_compensate_T(int adc_T)
{
    double var1, var2, T;
    var1 = (((double)adc_T)/16384.0 - ((double)BMP280_ComReg.T1)/1024.0) * ((double)BMP280_ComReg.T2);
    var2 = ((((double)adc_T)/131072.0 - ((double)BMP280_ComReg.T1)/8192.0) *
            (((double)adc_T)/131072.0 - ((double) BMP280_ComReg.T1)/8192.0)) * ((double)BMP280_ComReg.T3);
    t_fine = (int)(var1 + var2);
    T = (var1 + var2) / 5120.0;
    return T;
}

// 测量
void BMP280_ReadData() {
    int32_t press_raw, temp_raw;
    press_raw = (BMP280_ReadReg(BMP280_REG_PRESS_MSB) << 12) | (BMP280_ReadReg(BMP280_REG_PRESS_LSB) << 4) | (BMP280_ReadReg(BMP280_REG_PRESS_XLSB) >> 4);
    temp_raw = (BMP280_ReadReg(BMP280_REG_TEMP_MSB) << 12) | (BMP280_ReadReg(BMP280_REG_TEMP_LSB) << 4) | (BMP280_ReadReg( BMP280_REG_TEMP_XLSB) >> 4);

    BMP280_Temp = BMP280_compensate_T(temp_raw);
    BMP280_Press = BMP280_compensate_P(press_raw);

}


