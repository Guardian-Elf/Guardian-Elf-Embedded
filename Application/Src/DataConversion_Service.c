//
// Created by pc on 2025/12/26.
//

#include <stdio.h>
#include "DataConversion_Service.h"

// 将结构体转换为json字符串
void EnvironmentDataToJson(EnvironmentData *data, char *json_str, size_t size, bool indent) {
    if (data == NULL || json_str == NULL || size == 0) {
        if (size > 0) {
            json_str[0] = '\0';
        }
        return;
    }

    if (indent) {
        // 带格式化的JSON（多行）
        snprintf(json_str, size,
                 "{\n"
                 "  \"temperature\": %.2f,\n"
                 "  \"humidity\": %.1f,\n"
                 "  \"pressure\": %.2f,\n"
                 "  \"illuminance\": %d\n"
                 "}",
                 data->Temperature,
                 data->Humidity,
                 data->Press,
                 data->Lux
        );
    } else {
        // 紧凑JSON（单行）
        snprintf(json_str, size,
                 "{\"temperature\":%.2f,\"humidity\":%.1f,\"pressure\":%.2f,\"illuminance\":%d}",
                 data->Temperature,
                 data->Humidity,
                 data->Press,
                 data->Lux
        );
    }

    // 确保字符串以空字符结尾
    json_str[size - 1] = '\0';
}

// 将结构体转换为json字符串
void MotionDataToJson(MotionData *data, char *json_str, size_t size, bool indent) {
    if (data == NULL || json_str == NULL || size == 0) {
        if (size > 0) {
            json_str[0] = '\0';
        }
        return;
    }

    if (indent) {
        // 与Java SpeedData实体类字段名完全一致
        snprintf(json_str, size,
                 "{\n"
                 "  \"accelX\": %.4f,\n"
                 "  \"accelY\": %.4f,\n"
                 "  \"accelZ\": %.4f,\n"
                 "  \"gyroX\": %.4f,\n"
                 "  \"gyroY\": %.4f,\n"
                 "  \"gyroZ\": %.4f\n"
                 "}",
                 data->Ax, data->Ay, data->Az,
                 data->Gx, data->Gy, data->Gz
        );
    } else {
        snprintf(json_str, size,
                 "{\"accelX\":%.4f,\"accelY\":%.4f,\"accelZ\":%.4f,\"gyroX\":%.4f,\"gyroY\":%.4f,\"gyroZ\":%.4f}",
                 data->Ax, data->Ay, data->Az,
                 data->Gx, data->Gy, data->Gz
        );
    }

    // 确保字符串以空字符结尾
    json_str[size - 1] = '\0';
}
