#ifndef SRC_REGISTERS_HPP
#define SRC_REGISTERS_HPP

#include <stdint.h>

// Registers for ICM-20948
// Taken from ICM-20948 DataSheet
// Scoped enums are used since register names are duplicated
// between User Banks

namespace imu {

enum class UserBank0Registers : uint8_t {
    kWHO_AM_I             = 0x00,
    kUSER_CTRL            = 0x03,
    kLP_CONFIG            = 0x05,
    kPWR_MGMT_1           = 0x06,
    kPWR_MGMT_2           = 0x07,
    kINT_PIN_CFG          = 0x0F,
    kINT_ENABLE           = 0x10,
    kINT_ENABLE_1         = 0x11,
    kINT_ENABLE_2         = 0x12,
    kINT_ENABLE_3         = 0x13,
    kI2C_MST_STATUS       = 0x17,
    kINT_STATUS           = 0x19,
    kINT_STATUS_1         = 0x1A,
    kINT_STATUS_2         = 0x1B,
    kINT_STATUS_3         = 0x1C,
    kDELAY_TIMEH          = 0x28,
    kDELAY_TIMEL          = 0x29,
    kACCEL_XOUT_H         = 0x2D,
    kACCEL_XOUT_L         = 0x2E,
    kACCEL_YOUT_H         = 0x2F,
    kACCEL_YOUT_L         = 0x30,
    kACCEL_ZOUT_H         = 0x31,
    kACCEL_ZOUT_L         = 0x32,
    kGYRO_XOUT_H          = 0x33,
    kGYRO_XOUT_L          = 0x34,
    kGYRO_YOUT_H          = 0x35,
    kGYRO_YOUT_L          = 0x36,
    kGYRO_ZOUT_H          = 0x37,
    kGYRO_ZOUT_L          = 0x38,
    kTEMP_OUT_H           = 0x39,
    kTEMP_OUT_L           = 0x3A,
    kEXT_SLV_SENS_DATA_00 = 0x3B,
    kEXT_SLV_SENS_DATA_01 = 0x3C,
    kEXT_SLV_SENS_DATA_02 = 0x3D,
    kEXT_SLV_SENS_DATA_03 = 0x3E,
    kEXT_SLV_SENS_DATA_04 = 0x3F,
    kEXT_SLV_SENS_DATA_05 = 0x40,
    kEXT_SLV_SENS_DATA_06 = 0x41,
    kEXT_SLV_SENS_DATA_07 = 0x42,
    kEXT_SLV_SENS_DATA_08 = 0x43,
    kEXT_SLV_SENS_DATA_09 = 0x44,
    kEXT_SLV_SENS_DATA_10 = 0x45,
    kEXT_SLV_SENS_DATA_11 = 0x46,
    kEXT_SLV_SENS_DATA_12 = 0x47,
    kEXT_SLV_SENS_DATA_13 = 0x48,
    kEXT_SLV_SENS_DATA_14 = 0x49,
    kEXT_SLV_SENS_DATA_15 = 0x4A,
    kEXT_SLV_SENS_DATA_16 = 0x4B,
    kEXT_SLV_SENS_DATA_17 = 0x4C,
    kEXT_SLV_SENS_DATA_18 = 0x4D,
    kEXT_SLV_SENS_DATA_19 = 0x4E,
    kEXT_SLV_SENS_DATA_20 = 0x4F,
    kEXT_SLV_SENS_DATA_21 = 0x50,
    kEXT_SLV_SENS_DATA_22 = 0x51,
    kEXT_SLV_SENS_DATA_23 = 0x52,
    kFIFO_EN_1            = 0x66,
    kFIFO_EN_2            = 0x67,
    kFIFO_RST             = 0x68,
    kFIFO_MODE            = 0x69,
    kFIFO_COUNTH          = 0x70,
    kFIFO_COUNTL          = 0x71,
    kFIFO_R_W             = 0x72,
    kDATA_RDY_STATUS      = 0x74,
    kFIFO_CFG             = 0x76,
    kREG_BANK_SEL         = 0x7F
};

enum class UserBank1Registers : uint8_t {
    kSELF_TEST_X_GYRO   = 0x02,
    kSELF_TEST_Y_GYRO   = 0x03,
    kSELF_TEST_Z_GYRO   = 0x04,
    kSELF_TEST_X_ACCEL  = 0x0E,
    kSELF_TEST_Y_ACCEL  = 0x0F,
    kSELF_TEST_Z_ACCEL  = 0x10,
    kXA_OFFS_H          = 0x14,
    kXA_OFFS_L          = 0x15,
    kYA_OFFS_H          = 0x17,
    kYA_OFFS_L          = 0x18,
    kZA_OFFS_H          = 0x1A,
    kZA_OFFS_L          = 0x1B,
    kTIMEBASE_CORRECTIO = 0x28,
    kREG_BANK_SEL       = 0x7F
};

enum class UserBank2Registers : uint8_t {
    kGYRO_SMPLRT_DIV    = 0x00,
    kGYRO_CONFIG_1      = 0x01,
    kGYRO_CONFIG_2      = 0x02,
    kXG_OFFS_USRH       = 0x03,
    kXG_OFFS_USRL       = 0x04,
    kYG_OFFS_USRH       = 0x05,
    kYG_OFFS_USRL       = 0x06,
    kZG_OFFS_USRH       = 0x07,
    kZG_OFFS_USRL       = 0x08,
    kODR_ALIGN_EN       = 0x09,
    kACCEL_SMPLRT_DIV_1 = 0x10,
    kACCEL_SMPLRT_DIV_2 = 0x11,
    kACCEL_INTEL_CTRL   = 0x12,
    kACCEL_WOM_THR      = 0x13,
    kACCEL_CONFIG       = 0x14,
    kACCEL_CONFIG_2     = 0x15,
    kFSYNC_CONFIG       = 0x52,
    kTEMP_CONFIG        = 0x53,
    kMOD_CTRL_USR       = 0x54,
    kREG_BANK_SEL       = 0x7F
};

enum class UserBank3Registers : uint8_t {
    kI2C_MST_ODR_CONFIG = 0x00,
    kI2C_MST_CTRL       = 0x01,
    kI2C_MST_DELAY_CTRL = 0x02,
    kI2C_SLV0_ADDR      = 0x03,
    kI2C_SLV0_REG       = 0x04,
    kI2C_SLV0_CTRL      = 0x05,
    kI2C_SLV0_DO        = 0x06,
    kI2C_SLV1_ADDR      = 0x07,
    kI2C_SLV1_REG       = 0x08,
    kI2C_SLV1_CTRL      = 0x09,
    kI2C_SLV1_DO        = 0x0A,
    kI2C_SLV2_ADDR      = 0x0B,
    kI2C_SLV2_REG       = 0x0C,
    kI2C_SLV2_CTRL      = 0x0D,
    kI2C_SLV2_DO        = 0x0E,
    kI2C_SLV3_ADDR      = 0x0F,
    kI2C_SLV3_REG       = 0x10,
    kI2C_SLV3_CTRL      = 0x11,
    kI2C_SLV3_DO        = 0x12,
    kI2C_SLV4_ADDR      = 0x13,
    kI2C_SLV4_REG       = 0x14,
    kI2C_SLV4_CTRL      = 0x15,
    kI2C_SLV4_DO        = 0x16,
    kI2C_SLV4_DI        = 0x17,
    kREG_BANK_SEL       = 0x7F
};

} // namespace imu

#endif // SRC_REGISTERS_HPP