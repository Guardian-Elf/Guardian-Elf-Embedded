//
// Created by pc on 2025/12/31.
//

#ifndef GUARDIANELF_ENUM_H
#define GUARDIANELF_ENUM_H

// 串口发送的数据操作码，用于判断每次串口发送的数据作用
enum SerialDataOperationCode {
    // 设备id
    SerialDataOperationCode_DeviceId = 0x00,
    // 定位数据
    SerialDataOperationCode_LocationData = 0x01,
    // 环境数据
    SerialDataOperationCode_EnvironmentData = 0x02,
    // 运动数据
    SerialDataOperationCode_MotionData = 0x03
};

#endif //GUARDIANELF_ENUM_H
