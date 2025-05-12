/**
 * @file   lsm303.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  LSM303 Director Driver
 * @date   2025-04-05
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "lsm303.h"

#include <math.h>

uint32_t HW_DIRECT;

extern I2C_HandleTypeDef hi2c2;
#define DIRECT_I2C hi2c2

#define LSM303_ACC_ADDR (0x19 << 1)
#define LSM303_MAG_ADDR (0x1E << 1)

// 寄存器配置宏
#define ACC_NORMAL_MODE 0x2F // 10Hz低功耗模式
#define ACC_SLEEP_MODE 0x08  // 关闭输出+低功耗
#define MAG_CONTINUOUS 0x00  // 连续转换模式
#define MAG_SLEEP 0x03       // 睡眠模式

// 初始化定义
#define CTRL_REG1_A 0x20
#define CTRL_REG4_A 0x23
#define OUT_X_L_A 0x28
#define CRA_REG_M 0x00
#define CRB_REG_M 0x01
#define MR_REG_M 0x02
#define OUT_X_H_M 0x03

static void DIRECT_readAccelerometer(int16_t *acc) {
  uint8_t reg = OUT_X_L_A;
  uint8_t data[6];

  // 读取加速度计数据
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_ACC_ADDR, &reg, 1, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(&hi2c2, LSM303_ACC_ADDR, data, sizeof(data),
                         HAL_MAX_DELAY);

  acc[0] = (int16_t)(data[1] << 8 | data[0]); // X
  acc[1] = (int16_t)(data[3] << 8 | data[2]); // Y
  acc[2] = (int16_t)(data[5] << 8 | data[4]); // Z
}

static void DIRECT_readMagnetometer(int16_t *mag) {
  uint8_t reg = OUT_X_H_M;
  uint8_t data[6];

  // 读取磁力计数据
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_MAG_ADDR, &reg, 1, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(&hi2c2, LSM303_MAG_ADDR, data, sizeof(data),
                         HAL_MAX_DELAY);

  mag[0] = (int16_t)(data[0] << 8 | data[1]); // X
  mag[1] = (int16_t)(data[4] << 8 | data[5]); // Y
  mag[2] = (int16_t)(data[2] << 8 | data[3]); // Z
}

static float DIRECT_calculateAzimuth(const int16_t *acc, const int16_t *mag) {
  // 加速度归一化处理
  const float acc_scale = 4.0f / 32768.0f; // ±4g量程下的灵敏度
  float ax = acc[0] * acc_scale;
  float ay = acc[1] * acc_scale;
  float az = acc[2] * acc_scale;

  // 计算俯仰角和横滚角
  float pitch = atan2f(-ax, sqrtf(ay * ay + az * az));
  float roll = atan2f(ay, az);

  // 磁力计数据转换（根据量程调整）
  const float mag_scale = 0.00092f; // ±4.0高斯下的灵敏度
  float mx = mag[0] * mag_scale;
  float my = mag[1] * mag_scale;
  float mz = mag[2] * mag_scale;

  // 坐标系补偿计算
  float xh = mx * cosf(pitch) + mz * sinf(pitch);
  float yh = mx * sinf(roll) * sinf(pitch) + my * cosf(roll) -
             mz * sinf(roll) * cosf(pitch);

  // 计算方位角（0-360度）
  float azimuth = atan2f(yh, xh) * 180.0f / 3.14f;
  if (azimuth < 0)
    azimuth += 360.0f;

  return azimuth;
}

static void DIRECT_wakeup(void) {
  // 加速度计唤醒配置
  uint8_t wake_acc[] = {CTRL_REG1_A, ACC_NORMAL_MODE};
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_ACC_ADDR, wake_acc, sizeof(wake_acc),
                          HAL_MAX_DELAY);

  // 磁力计唤醒配置
  uint8_t wake_mag[] = {MR_REG_M, MAG_CONTINUOUS};
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_MAG_ADDR, wake_mag, sizeof(wake_mag),
                          HAL_MAX_DELAY);

  delay(15);
}

static void DIRECT_sleep(void) {
  // 加速度计睡眠
  uint8_t sleep_acc[] = {CTRL_REG1_A, ACC_SLEEP_MODE};
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_ACC_ADDR, sleep_acc, sizeof(sleep_acc),
                          HAL_MAX_DELAY);

  // 磁力计睡眠
  uint8_t sleep_mag[] = {MR_REG_M, MAG_SLEEP};
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_MAG_ADDR, sleep_mag, sizeof(sleep_mag),
                          HAL_MAX_DELAY);
}

void DIRECT_init(void) {
  // 初始化加速度计
  uint8_t init_acc[] = {CTRL_REG1_A, 0x2F}; // 10Hz采样，低功耗模式
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_ACC_ADDR, init_acc, sizeof(init_acc),
                          HAL_MAX_DELAY);

  uint8_t scale_acc[] = {CTRL_REG4_A, 0x10}; // ±4g量程
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_ACC_ADDR, scale_acc, sizeof(scale_acc),
                          HAL_MAX_DELAY);

  // 初始化磁力计
  uint8_t init_mag[] = {CRA_REG_M, 0x10}; // 15Hz采样率
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_MAG_ADDR, init_mag, sizeof(init_mag),
                          HAL_MAX_DELAY);

  uint8_t scale_mag[] = {CRB_REG_M, 0x80}; // ±4.0高斯
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_MAG_ADDR, scale_mag, sizeof(scale_mag),
                          HAL_MAX_DELAY);

  uint8_t mode_mag[] = {MR_REG_M, 0x00}; // 连续转换模式
  HAL_I2C_Master_Transmit(&hi2c2, LSM303_MAG_ADDR, mode_mag, sizeof(mode_mag),
                          HAL_MAX_DELAY);

  DIRECT_sleep();
}

void DIRECT_fresh(void) {
  DIRECT_wakeup();

  int16_t acc[3], mag[3];
  DIRECT_readAccelerometer(acc);
  DIRECT_readMagnetometer(mag);

  float azimuth = DIRECT_calculateAzimuth(acc, mag);
  if (azimuth < 0)
    azimuth += 360.0f;
  HW_DIRECT = (uint32_t)(azimuth * 100.0f);

  DIRECT_sleep();
}
