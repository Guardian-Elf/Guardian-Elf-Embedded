//
// Created by pc on 2025/12/2.
//

#include <stdio.h>
#include <stdlib.h>

#include "GP10.h"

#define MAX_LINE_LENGTH 13
#define MAX_Field_LENGTH 25

GP10Data GP10_Data;


/**
 * @brief 分割大句子为独立的NMEA语句
 * @param str 输入的大字符串（包含多个NMEA句子）
 * @param sentences 输出：指向各句子开头的指针数组
 * @return true-成功分割，false-失败
 *
 * 原理：在句子末尾添加\0，sentences中的指针指向每个句子开头
 *       会修改str内容（将换行符替换为'\0'）
 */
bool split_nmea_static(char *str, char *sentences[MAX_LINE_LENGTH]) {
    if (str == NULL || sentences == NULL) {
        return false;
    }

    // 初始化句子数组
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        sentences[i] = NULL;
    }

    int sentence_count = 0;
    char *sentence_start = NULL;

    for (int i = 0; str[i] != '\0' && sentence_count < MAX_LINE_LENGTH; i++) {
        // 查找句子开始（$符号）
        if (str[i] == '$') {
            if (sentence_start != NULL && i > 0) {
                // 结束前一个句子（在当前位置前一个字符处加\0）
                str[i - 1] = '\0';
            }
            sentence_start = &str[i];
        }
            // 句子结束（换行符）
        else if (str[i] == '\n' || str[i] == '\r') {
            if (sentence_start != NULL) {
                // 在当前字符处结束句子
                str[i] = '\0';
                sentences[sentence_count] = sentence_start;
                sentence_count++;
                sentence_start = NULL;
            }
        }
    }

    // 处理最后一个句子
    if (sentence_start != NULL && sentence_count < MAX_LINE_LENGTH) {
        sentences[sentence_count] = sentence_start;
        sentence_count++;
    }

    return (sentence_count > 0);
}

/**
 * @brief 按逗号分割单个NMEA句子
 * @param sentence 单个NMEA句子
 * @param fields 输出：指向各字段的指针数组
 * @return true-成功分割，false-失败
 *
 * 原理：将逗号替换为'\0'，fields中的指针指向sentence中的字段位置
 *       会修改sentence内容（将逗号替换为'\0'）
 */
bool split_nmea_by_comma(char *sentence, char *fields[MAX_Field_LENGTH]) {
    if (sentence == NULL || sentence[0] != '$') {
        return false;
    }

    // 初始化字段数组
    for (int i = 0; i < MAX_Field_LENGTH; i++) {
        fields[i] = NULL;
    }

    int field_count = 0;
    char *field_start = sentence;

    for (int i = 0; sentence[i] != '\0' && field_count < MAX_Field_LENGTH; i++) {
        if (sentence[i] == ',') {
            // 字段结束，替换逗号为\0
            sentence[i] = '\0';
            fields[field_count] = field_start;
            field_count++;
            field_start = &sentence[i + 1];
        } else if (sentence[i] == '*') {
            // 校验和开始
            sentence[i] = '\0';

            // 当前字段（校验和之前的部分）
            if (field_start < &sentence[i]) {
                fields[field_count] = field_start;
                field_count++;
            }

            // 校验和字段
            if (sentence[i + 1] != '\0' && field_count < 25) {
                fields[field_count] = &sentence[i + 1];
                field_count++;
            }

            break;
        }
    }

    // 处理最后一个字段（如果没有以逗号或*结束）
    if (field_count < 25 && field_start[0] != '\0') {
        fields[field_count] = field_start;
        field_count++;
    }

    return (field_count > 0);
}

//  校验数据是否异常（检测连续逗号）
bool check_data_validity(const char *str) {
    if (str == NULL) {
        return false;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',' && str[i + 1] == ',') {
            return false;
        }
    }
    return true;
}
// 时间转换(示例：080501.000)
void TimeConvert(const char *str, Time *time) {
    time->hour = (str[0] - '0') * 10 + (str[1] - '0');
    time->minute = (str[2] - '0') * 10 + (str[3] - '0');
    time->second = (str[4] - '0') * 10 + (str[5] - '0');
    time->millisecond = (str[7] - '0') * 100 + (str[8] - '0') * 10 + (str[9] - '0');
}

// 经度转换(示例：11711.9362)
void LongitudeConvert(const char *str, Coordinate *coordinate) {
    coordinate->longitude_deg = (str[0] - '0') * 100 + (str[1] - '0') * 10 + (str[2] - '0');
    coordinate->longitude_dm = (str[3] - '0') * 10 + (str[4] - '0') + (str[6] - '0') / 10.0 + (str[7] - '0') / 100.0 +
                               (str[8] - '0') / 1000.0;

}

// 纬度转换(示例：3150.7821)
void LatitudeConvert(const char *str, Coordinate *coordinate) {
    coordinate->latitude_deg = (str[0] - '0') * 10 + (str[1] - '0');
    coordinate->latitude_dm = (str[2] - '0') * 10 + (str[3] - '0') + (str[5] - '0') / 10.0 + (str[6] - '0') / 100.0 +
                              (str[7] - '0') / 1000.0;

}

// 数据转换函数
void GP10_DataConvert(char *str, GP10Data *data) {
    if (!check_data_validity(str)) {
        data->valid = false;
        return;
    }
    char *sentences[MAX_LINE_LENGTH];
    bool flag = split_nmea_static(str, sentences);

    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        if (flag) {
            char *sentence = sentences[i];
            char *fields[MAX_Field_LENGTH];

            //读取数据
            switch (i) {
                case 0:
                    split_nmea_by_comma(sentence, fields);

                    // 海拔
                    data->coordinate.altitude_m = strtof(fields[9], NULL);
                    // 大地水准面高度
                    data->coordinate.altitude_agl = strtof(fields[10], NULL);
                    break;
                case 1:
                    split_nmea_by_comma(sentence, fields);

                    // 纬度
                    LatitudeConvert(fields[1], &data->coordinate);
                    // 经度
                    LongitudeConvert(fields[3], &data->coordinate);
                    // 纬度方向
                    data->coordinate.lat_dir = fields[2][0];
                    // 经度方向
                    data->coordinate.lon_dir = fields[4][0];

                    break;
                case 4:
                    split_nmea_by_comma(sentence, fields);
                    // GPS卫星数
                    data->satellite.gps_satellites = strtol(fields[3], NULL, 10);
                    break;
                case 7:
                    split_nmea_by_comma(sentence, fields);
                    // 北斗卫星数
                    data->satellite.bds_satellites = strtol(fields[3], NULL, 10);
                    break;
                case 10:
                    split_nmea_by_comma(sentence, fields);

                    // 地面速度(节)
                    data->motion.speed_knots = strtof(fields[7], NULL);
                    // 转换成米/小时
                    data->motion.speed_kmh = (float) (data->motion.speed_knots * 1.852);
                    // 真北航向
                    data->motion.course_true = strtof(fields[8], NULL);
                    // 磁偏角
                    data->motion.magnetic_variation = strtof(fields[10], NULL);
                    // 磁偏角方向
                    data->motion.variation_dir = fields[11][0];

                    // 打印
                    break;
                case 12:
                    split_nmea_by_comma(sentence, fields);

                    //  时间
                    char *time_str = fields[1];


                    TimeConvert(time_str, &data->time);

                    data->time.day = (fields[2][0] - '0') * 10 + (fields[2][1] - '0');
                    data->time.month = (fields[3][0] - '0') * 10 + (fields[3][1] - '0');
                    data->time.year =
                            (fields[4][0] - '0') * 1000 + (fields[4][1] - '0') * 100 + (fields[4][2] - '0') * 10 +
                            (fields[4][3] - '0');
                    data->time.offset_hour = (fields[5][0] - '0') * 10 + (fields[5][1] - '0');
                    data->time.offset_minute = (fields[6][0] - '0') * 10 + (fields[6][1] - '0');
                    break;
                default:
                    break;
            }

        }
    }
}


int Gp10_Data2Json(const GP10Data *data, char json_str[], size_t str_size, bool indent) {
    if (data == NULL || json_str == NULL || str_size == 0 || !data->valid) {
        snprintf(json_str, str_size, "无效数据");
        return -1;
    }

    const char *nl = indent ? "\n" : "";
    const char *tab = indent ? "    " : "";
    const char *space = indent ? " " : "";

    int offset = 0;

    // 开始构建JSON
    offset += snprintf(json_str + offset, str_size - offset,
                       "{%s", nl);

    // 坐标信息
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s\"coordinate\":%s{%s", tab, space, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"latitude_deg\":%s%d,%s", tab, tab, space,
                       data->coordinate.latitude_deg, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"longitude_deg\":%s%d,%s", tab, tab, space,
                       data->coordinate.longitude_deg, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"latitude_dm\":%s%.6f,%s", tab, tab, space,
                       data->coordinate.latitude_dm, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"longitude_dm\":%s%.6f,%s", tab, tab, space,
                       data->coordinate.longitude_dm, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"altitude_m\":%s%.2f,%s", tab, tab, space,
                       data->coordinate.altitude_m, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"altitude_agl\":%s%.2f,%s", tab, tab, space,
                       data->coordinate.altitude_agl, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"lat_dir\":%s\"%c\",%s", tab, tab, space,
                       data->coordinate.lat_dir, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"lon_dir\":%s\"%c\"%s", tab, tab, space,
                       data->coordinate.lon_dir, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s}%s,%s", tab, nl, nl);

    // 时间信息
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s\"time\":%s{%s", tab, space, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"utc_time\":%s\"%02d:%02d:%02d.%03d\",%s", tab, tab, space,
                       data->time.hour, data->time.minute, data->time.second,
                       data->time.millisecond, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"date\":%s\"%04d-%02d-%02d\",%s", tab, tab, space,
                       data->time.year, data->time.month, data->time.day, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"timezone_offset\":%s\"%+02d:%02d\"%s", tab, tab, space,
                       data->time.offset_hour, data->time.offset_minute, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s}%s,%s", tab, nl, nl);

    // 运动信息
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s\"motion\":%s{%s", tab, space, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"speed_knots\":%s%.2f,%s", tab, tab, space,
                       data->motion.speed_knots, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"speed_kmh\":%s%.2f,%s", tab, tab, space,
                       data->motion.speed_kmh, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"course_true\":%s%.1f,%s", tab, tab, space,
                       data->motion.course_true, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"magnetic_variation\":%s%.1f,%s", tab, tab, space,
                       data->motion.magnetic_variation, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"variation_dir\":%s\"%c\"%s", tab, tab, space,
                       data->motion.variation_dir, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s}%s,%s", tab, nl, nl);

    // 卫星信息
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s\"satellite\":%s{%s", tab, space, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"gps_satellites\":%s%d,%s", tab, tab, space,
                       data->satellite.gps_satellites, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s%s\"bds_satellites\":%s%d%s", tab, tab, space,
                       data->satellite.bds_satellites, nl);
    offset += snprintf(json_str + offset, str_size - offset,
                       "%s}%s", tab, nl);

    // 结束JSON
    snprintf(json_str + offset, str_size - offset,
             "}%s", nl);

    return 0;
}



int Gp10_Data2ChineseInfo(const GP10Data *data, char info_str[], size_t str_size) {
    if (data == NULL || info_str == NULL || str_size == 0 || !data->valid) {
        snprintf(info_str, str_size, "无效数据");
        return -1;
    }

    int offset = 0;

    // 标题
    offset += snprintf(info_str + offset, str_size - offset,
                       "====== GPS/北斗定位信息 ======\n\n");

    // 位置信息
    offset += snprintf(info_str + offset, str_size - offset,
                       "【位置信息】\n");
    offset += snprintf(info_str + offset, str_size - offset,
                       "纬度: %d° %.6f' %c\n",
                       data->coordinate.latitude_deg,
                       data->coordinate.latitude_dm,
                       data->coordinate.lat_dir);
    offset += snprintf(info_str + offset, str_size - offset,
                       "经度: %d° %.6f' %c\n",
                       data->coordinate.longitude_deg,
                       data->coordinate.longitude_dm,
                       data->coordinate.lon_dir);

    offset += snprintf(info_str + offset, str_size - offset,
                       "海拔: %.2f米\n", data->coordinate.altitude_m);
    offset += snprintf(info_str + offset, str_size - offset,
                       "大地水准面高度: %.2f米\n\n", data->coordinate.altitude_agl);

    // 时间信息
    offset += snprintf(info_str + offset, str_size - offset,
                       "【时间信息】\n");
    offset += snprintf(info_str + offset, str_size - offset,
                       "UTC时间: %04d-%02d-%02d %02d:%02d:%02d.%03d\n",
                       data->time.year, data->time.month, data->time.day,
                       data->time.hour, data->time.minute, data->time.second,
                       data->time.millisecond);

    // 计算北京时间（UTC+8）
    int beijing_hour = data->time.hour + 8;
    int beijing_day = data->time.day;
    int beijing_month = data->time.month;
    int beijing_year = data->time.year;

    if (beijing_hour >= 24) {
        beijing_hour -= 24;
        beijing_day++;
    }

    offset += snprintf(info_str + offset, str_size - offset,
                       "北京时间: %04d-%02d-%02d %02d:%02d:%02d.%03d\n\n",
                       beijing_year, beijing_month, beijing_day,
                       beijing_hour, data->time.minute, data->time.second, data->time.millisecond);

    // 运动信息
    offset += snprintf(info_str + offset, str_size - offset,
                       "【运动信息】\n");
    offset += snprintf(info_str + offset, str_size - offset,
                       "地面速度: %.2f 节 (约 %.2f km/h)\n",
                       data->motion.speed_knots, data->motion.speed_kmh);

    // 速度状态描述
    const char *speed_status;
    if (data->motion.speed_knots < 0.1f) {
        speed_status = "静止";
    } else if (data->motion.speed_knots < 5.0f) {
        speed_status = "低速移动";
    } else if (data->motion.speed_knots < 20.0f) {
        speed_status = "中速移动";
    } else {
        speed_status = "高速移动";
    }

    offset += snprintf(info_str + offset, str_size - offset,
                       "速度状态: %s\n", speed_status);

    offset += snprintf(info_str + offset, str_size - offset,
                       "航向: %.1f° (真北方向)\n", data->motion.course_true);

    // 航向方位描述
    const char *direction;
    float course = data->motion.course_true;
    if (course >= 337.5 || course < 22.5) {
        direction = "北";
    } else if (course >= 22.5 && course < 67.5) {
        direction = "东北";
    } else if (course >= 67.5 && course < 112.5) {
        direction = "东";
    } else if (course >= 112.5 && course < 157.5) {
        direction = "东南";
    } else if (course >= 157.5 && course < 202.5) {
        direction = "南";
    } else if (course >= 202.5 && course < 247.5) {
        direction = "西南";
    } else if (course >= 247.5 && course < 292.5) {
        direction = "西";
    } else {
        direction = "西北";
    }

    offset += snprintf(info_str + offset, str_size - offset,
                       "方位: %s方向\n", direction);

    if (data->motion.magnetic_variation != 0.0f) {
        offset += snprintf(info_str + offset, str_size - offset,
                           "磁偏角: %.1f° %c\n\n",
                           data->motion.magnetic_variation, data->motion.variation_dir);
    } else {
        offset += snprintf(info_str + offset, str_size - offset,
                           "磁偏角: 无\n\n");
    }

    // 卫星信息
    offset += snprintf(info_str + offset, str_size - offset,
                       "【卫星信息】\n");
    offset += snprintf(info_str + offset, str_size - offset,
                       "GPS卫星: %d 颗\n", data->satellite.gps_satellites);
    offset += snprintf(info_str + offset, str_size - offset,
                       "北斗卫星: %d 颗\n", data->satellite.bds_satellites);
    offset += snprintf(info_str + offset, str_size - offset,
                       "总计: %d 颗卫星\n",
                       data->satellite.gps_satellites + data->satellite.bds_satellites);

    // 定位质量评估
    int total_access = data->satellite.gps_satellites + data->satellite.bds_satellites;
    const char *quality;

    if (total_access >= 10) {
        quality = "极好";
    } else if (total_access >= 6) {
        quality = "良好";
    } else if (total_access >= 4) {
        quality = "一般";
    } else {
        quality = "较差";
    }

    offset += snprintf(info_str + offset, str_size - offset,
                       "定位质量: %s\n\n", quality);

    // 系统状态
    offset += snprintf(info_str + offset, str_size - offset,
                       "【系统状态】\n");
    offset += snprintf(info_str + offset, str_size - offset,
                       "定位模式: %s定位\n",
                       (data->satellite.gps_satellites > 0 && data->satellite.bds_satellites > 0)
                       ? "GPS+北斗双模" : "单模");

    // 数据有效性检查
    int valid_count = 0;
    if (data->coordinate.lat_dir == 'N' || data->coordinate.lat_dir == 'S') valid_count++;
    if (data->coordinate.lon_dir == 'E' || data->coordinate.lon_dir == 'W') valid_count++;
    if (data->time.year >= 2020 && data->time.year <= 2100) valid_count++;

    offset += snprintf(info_str + offset, str_size - offset,
                       "数据有效性: %s\n",
                       valid_count >= 3 ? "有效" : "需检查");

    snprintf(info_str + offset, str_size - offset,
             "==============================\n");

    return 0;
}


int Gp10_Data2SimpleChinese(const GP10Data *data, char info_str[], size_t str_size) {
    if (data == NULL || info_str == NULL || str_size == 0 || !data->valid) {
        snprintf(info_str, str_size, "无效数据");
        return -1;
    }


    // 简化的单行信息
    snprintf(info_str, str_size,
             "位置: %c %d°%.4f',%c %d°%.4f' | "
             "海拔: %.1fm | "
             "时间: %02d:%02d:%02d | "
             "速度: %.1fkm/h | "
             "航向: %.0f°%s | "
             "卫星: %d(GPS)+%d(北斗)=%d",

             data->coordinate.lat_dir, data->coordinate.latitude_deg, data->coordinate.latitude_dm ,
             data->coordinate.lon_dir, data->coordinate.longitude_deg, data->coordinate.longitude_dm ,
             data->coordinate.altitude_m,
             data->time.hour, data->time.minute, data->time.second,
             data->motion.speed_kmh,
             data->motion.course_true,
            // 简化的方位
             (data->motion.course_true >= 0 && data->motion.course_true < 45) ? "北" :
             (data->motion.course_true < 90) ? "东北" :
             (data->motion.course_true < 135) ? "东" :
             (data->motion.course_true < 180) ? "东南" :
             (data->motion.course_true < 225) ? "南" :
             (data->motion.course_true < 270) ? "西南" :
             (data->motion.course_true < 315) ? "西" : "西北",
             data->satellite.gps_satellites,
             data->satellite.bds_satellites,
             data->satellite.gps_satellites + data->satellite.bds_satellites);

    return 0;
}
