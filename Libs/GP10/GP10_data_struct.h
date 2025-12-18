//
// Created by pc on 2025/12/2.
//

#ifndef GUARDIANELF_GP10_DATA_STRUCT_H
#define GUARDIANELF_GP10_DATA_STRUCT_H


#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// ==================== 坐标结构（数值类型）====================
typedef struct {
    // 十进制坐标（便于计算）
    uint8_t latitude_deg;      // 纬度（度，十进制）
    uint8_t longitude_deg;     // 经度（度，十进制）
    double latitude_dm;      // 纬度（分）
    double longitude_dm;     // 经度（分）
    float altitude_m;         // 海拔高度（米）
    float altitude_agl; // 大地水准面高度

    // 方向
    char lat_dir;             // 纬度方向 N/S
    char lon_dir;             // 经度方向 E/W
} Coordinate;

// ==================== 时间结构（数值类型）====================
typedef struct {
    // UTC时间
    uint8_t hour;             // 时 (0-23)
    uint8_t minute;           // 分 (0-59)
    uint8_t second;           // 秒 (0-59)
    uint16_t millisecond;     // 毫秒 (0-999)

    // 日期
    uint8_t day;              // 日 (1-31)
    uint8_t month;            // 月 (1-12)
    uint16_t year;            // 年 (如2024)

    // 偏移
    uint8_t offset_hour;
    uint8_t offset_minute;
} Time;

// ==================== 运动信息（数值类型）====================
typedef struct {
    float speed_knots;        // 地面速度（节）
    float speed_kmh;          // 地面速度（公里/小时）
    float course_true;        // 真北航向（度）
    float magnetic_variation; // 磁偏角（度）
    char variation_dir;       // 磁偏角方向 E/W

} MotionInfo;

// ==================== 卫星信息 ====================
typedef struct {
    uint8_t gps_satellites;        // GPS卫星数
    uint8_t bds_satellites;        // 北斗卫星数
} SatelliteInfo;

// ==================== 主数据结构体 ====================
typedef struct {
    // 核心数据
    Coordinate coordinate;
    Time time;
    MotionInfo motion;
    SatelliteInfo satellite;
    // 设备标识
    char device_id[20];
    // 是否有效
    bool valid;
} GP10Data;
#endif //GUARDIANELF_GP10_DATA_STRUCT_H

