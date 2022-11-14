#ifndef SRC_ICM20948TYPES_HPP
#define SRC_ICM20948TYPES_HPP

// Heavily borrowed from Stephen Murphy's ICM20948 C Driver
// Reference: https://github.com/stephendpmurphy/icm20948
// Reference: ICM 20948 Datasheet

namespace imu {

namespace decode {

// Bank 0
typedef uint8_t WHO_AM_I;

typedef union {
    struct {
        uint8_t RSVD                : 1;
        uint8_t I2C_MST_RST         : 1;
        uint8_t SRAM_RST            : 1;
        uint8_t DMP_RST             : 1;
        uint8_t I2C_IF_DS           : 1;
        uint8_t I2C_MST_EN          : 1;
        uint8_t FIFO_EN             : 1;
        uint8_t DMP_EN              : 1;
    } bits;
    uint8_t byte;
} USER_CTRL;

typedef union {
    struct {
        uint8_t RSVD0               : 4;
        uint8_t GYRO_CYCLE          : 1;
        uint8_t ACCEL_CYCLE         : 1;
        uint8_t I2C_MST_CYCLE       : 1;
        uint8_t RSVD1               : 1;
    } bits;
    uint8_t byte;
} LP_CONFIG;

typedef union {
    struct {
        uint8_t CLKSEL              : 3;
        uint8_t TEMP_DIS            : 1;
        uint8_t RSVD                : 1;
        uint8_t LP_EN               : 1;
        uint8_t SLEEP               : 1;
        uint8_t DEVICE_RESET        : 1;
    } bits;
    uint8_t byte;
} PWR_MGMT_1;

typedef union {
    struct {
        uint8_t DISABLE_GYRO        : 3;
        uint8_t DISABLE_ACCEL       : 3;
        uint8_t RSVD                : 2;
    } bits;
    uint8_t byte;
} PWR_MGMT_2;

typedef union {
    struct {
        uint8_t RSVD                : 1;
        uint8_t BYPASS_EN           : 1;
        uint8_t FSYNC_INT_MODE_EN   : 1;
        uint8_t ACTL_FSYNC          : 1;
        uint8_t INT_ANYRD_2CLEAR    : 1;
        uint8_t INT1_LATCH__EN      : 1;
        uint8_t INT1_OPEN           : 1;
        uint8_t INT1_ACTL           : 1;
    } bits;
    uint8_t byte;
} INT_PIN_CFG;

typedef union {
    struct {
        uint8_t I2C_MST_INT_EN      : 1;
        uint8_t DMP_INT1_EN         : 1;
        uint8_t PLL_RDY_EN          : 1;
        uint8_t WOM_INT_EN          : 1;
        uint8_t RSVD                : 3;
        uint8_t REG_WOF_EN          : 1;
    } bits;
    uint8_t byte;
} INT_ENABLE;

typedef union {
    struct {
        uint8_t RAW_DATA_0_RDY_EN   : 1;
        uint8_t RSVD                : 7;
    } bits;
    uint8_t byte;
} INT_ENABLE_1;

typedef union {
    struct {
        uint8_t FIFO_OVERFLOW_EN    : 5;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} INT_ENABLE_2;

typedef union {
    struct {
        uint8_t FIFO_W_EN           : 5;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} INT_ENABLE_3;

typedef union {
    struct {
        uint8_t I2C_SLV0_NACK       : 1;
        uint8_t I2C_SLV1_NACK       : 1;
        uint8_t I2C_SLV2_NACK       : 1;
        uint8_t I2C_SLV3_NACK       : 1;
        uint8_t I2C_SLV4_NACK       : 1;
        uint8_t I2C_LOST_ARB        : 1;
        uint8_t I2C_SLV4_DONE       : 1;
        uint8_t PASS_THROUGH        : 1;
    } bits;
    uint8_t byte;
} I2C_MST_STATUS;

typedef union {
    struct {
        uint8_t I2C_MST_INT         : 1;
        uint8_t DMP_INT1            : 1;
        uint8_t PLL_RDY_INT         : 1;
        uint8_t WOM_INT             : 1;
        uint8_t RSVD                : 4;
    } bits;
    uint8_t byte;
} INT_STATUS;

typedef union {
    struct {
        uint8_t RAW_DATA_0_RDY_INT  : 1;
        uint8_t RSVD                : 7;
    } bits;
    uint8_t byte;
} INT_STATUS_1;

typedef union {
    struct {
        uint8_t FIFO_OVERFLOW_INT   : 5;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} INT_STATUS_2;

typedef union {
    struct {
        uint8_t FIFO_WM_INT         : 5;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} INT_STATUS_3;

typedef uint8_t DELAY_TIMEH;
typedef uint8_t DELAY_TIMEL;
typedef uint8_t ACCEL_XOUT_H;
typedef uint8_t ACCEL_XOUT_L;
typedef uint8_t ACCEL_YOUT_H;
typedef uint8_t ACCEL_YOUT_L;
typedef uint8_t ACCEL_ZOUT_H;
typedef uint8_t ACCEL_ZOUT_L;
typedef uint8_t GYRO_XOUT_H;
typedef uint8_t GYRO_XOUT_L;
typedef uint8_t GYRO_YOUT_H;
typedef uint8_t GYRO_YOUT_L;
typedef uint8_t GYRO_ZOUT_H;
typedef uint8_t GYRO_ZOUT_L;
typedef uint8_t TEMP_OUT_H;
typedef uint8_t TEMP_OUT_L;
typedef uint8_t EXT_SLV_SENS_DATA[24]; // 24 external slave sense data registers

typedef union {
    struct {
        uint8_t SLV_0_FIFO_EN       : 1;
        uint8_t SLV_1_FIFO_EN       : 1;
        uint8_t SLV_2_FIFO_EN       : 1;
        uint8_t SLV_3_FIFO_EN       : 1;
        uint8_t RSVD                : 4;
    } bits;
    uint8_t byte;
} FIFO_EN_1;

typedef union {
    struct {
        uint8_t TEMP_FIFO_EN        : 1;
        uint8_t GYRO_X_FIFO_EN      : 1;
        uint8_t GYRO_Y_FIFO_EN      : 1;
        uint8_t GYRO_Z_FIFO_EN      : 1;
        uint8_t ACCEL_FIFO_EN       : 1;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} FIFO_EN_2;

typedef union {
    struct {
        uint8_t FIFO_RESET          : 5;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} FIFO_RST;

typedef union {
    struct {
        uint8_t FIFO_MODE           : 5;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} FIFO_MODE;

typedef union {
    struct {
        uint8_t FIFO_COUNTH         : 5;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} FIFO_COUNTH;

typedef uint8_t FIFO_COUNTL;
typedef uint8_t FIFO_R_W;
typedef uint8_t DATA_RDY_STATUS;
typedef uint8_t FIFO_CFG;

typedef union {
    struct {
        uint8_t RSVD0               : 4;
        uint8_t USER_BANK           : 2;
        uint8_t RSVD1               : 2;
    } bits;
    uint8_t byte;
} REG_BANK_SEL;


// Bank 1

typedef uint8_t SELF_TEST_X_GYRO;
typedef uint8_t SELF_TEST_Y_GYRO;
typedef uint8_t SELF_TEST_Z_GYRO;
typedef uint8_t SELF_TEST_X_ACCEL;
typedef uint8_t SELF_TEST_Y_ACCEL;
typedef uint8_t SELF_TEST_Z_ACCEL;
typedef uint8_t XA_OFFS_H;

typedef union {
    struct {
        uint8_t RSVD                : 1;
        uint8_t XA_OFFS             : 7;
    } bits;
    uint8_t byte;
} XA_OFFS_L;

typedef uint8_t YA_OFFS_H;

typedef union {
    struct {
        uint8_t RSVD                : 1;
        uint8_t YA_OFFS             : 7;
    } bits;
    uint8_t byte;
} YA_OFFS_L;

typedef uint8_t ZA_OFFS_H;

typedef union {
    struct {
        uint8_t RSVD                : 1;
        uint8_t ZA_OFFS             : 7;
    } bits;
    uint8_t byte;
} ZA_OFFS_L;

typedef uint8_t TIMEBASE_CORRECTION_PLL;

// Bank 2

typedef uint8_t GYRO_SMPLRT_DIV;

typedef union {
    struct {
        uint8_t GYRO_FCHOICE        : 1;
        uint8_t GYRO_FS_SEL         : 2;
        uint8_t GYRO_DLPFCFG        : 3;
        uint8_t RSVD                : 2;
    } bits;
    uint8_t byte;
} GYRO_CONFIG_1;

typedef union {
    struct {
        uint8_t GYRO_AVGCFG         : 3;
        uint8_t ZGYRO_CTEN          : 1;
        uint8_t YGYRO_CTEN          : 1;
        uint8_t XGYRO_CTEN          : 1;
        uint8_t RSVD                : 2;
    } bits;
    uint8_t byte;
} GYRO_CONFIG_2;

typedef uint8_t XG_OFFS_USRH;
typedef uint8_t XG_OFFS_USRL;
typedef uint8_t YG_OFFS_USRH;
typedef uint8_t YG_OFFS_USRL;
typedef uint8_t ZG_OFFS_USRH;
typedef uint8_t ZG_OFFS_USRL;

typedef union {
    struct {
        uint8_t ODR_ALIGN_EN        : 1;
        uint8_t RSVD                : 7;
    } bits;
    uint8_t byte;
} ODR_ALIGN_EN;

typedef union {
    struct {
        uint8_t ACCEL_SMPLRT_DIV    : 4;
        uint8_t RSVD                : 4;
    } bits;
    uint8_t byte;
} ACCEL_SMPLRT_DIV_1;

typedef uint8_t ACCEL_SMPLRT_DIV_2;

typedef union {
    struct {
        uint8_t ACCEL_INTEL_MODE_INT    : 1;
        uint8_t ACCEL_INTEL_EN      : 1;
        uint8_t RSVD                : 6;
    } bits;
    uint8_t byte;
} ACCEL_INTEL_CTRL;

typedef uint8_t ACCEL_WOM_THR;

typedef union {
    struct {
        uint8_t ACCEL_FCHOICE       : 1;
        uint8_t ACCEL_FS_SEL        : 2;
        uint8_t ACCEL_DLPFCFG       : 3;
        uint8_t RSVD                : 2;
    } bits;
    uint8_t byte;
} ACCEL_CONFIG;

typedef union {
    struct {
        uint8_t DEC3_CFG            : 2;
        uint8_t AZ_ST_EN_REG        : 1;
        uint8_t AY_ST_EN_REG        : 1;
        uint8_t AX_ST_EN_REG        : 1;
        uint8_t RSVD                : 3;
    } bits;
    uint8_t byte;
} ACCEL_CONFIG_2;

typedef union {
    struct {
        uint8_t EXT_SYNC_SET        : 4;
        uint8_t WOF_EDGE_INT        : 1;
        uint8_t WOF_DEGLITCH_EN     : 1;
        uint8_t RSVD                : 1;
        uint8_t DELAY_TIME_EN       : 1;
    } bits;
    uint8_t byte;
} FSYNC_CONFIG;

typedef union {
    struct {
        uint8_t TEMP_DLPFCFG        : 3;
        uint8_t RSVD                : 5;
    } bits;
    uint8_t byte;
} TEMP_CONFIG;

typedef union {
    struct {
        uint8_t REG_LP_DMP_EN       : 1;
        uint8_t RSVD                : 7;
    } bits;
    uint8_t byte;
} MOD_CTRL_USR;


// Bank 3


typedef union {
    struct {
        uint8_t I2C_MST_ODR_CONFIG  : 4;
        uint8_t RSVD                : 4;
    } bits;
    uint8_t byte;
} I2C_MST_ODR_CONFIG;

typedef union {
    struct {
        uint8_t I2C_MST_CLK         : 4;
        uint8_t I2C_MST_P_NSR       : 1;
        uint8_t RSVD                : 2;
        uint8_t MULT_MST_EN         : 1;
    } bits;
    uint8_t byte;
} I2C_MST_CTRL;

typedef union {
    struct {
        uint8_t I2C_SLV0_DELAY_EN   : 1;
        uint8_t I2C_SLV1_DELAY_EN   : 1;
        uint8_t I2C_SLV2_DELAY_EN   : 1;
        uint8_t I2C_SLV3_DELAY_EN   : 1;
        uint8_t I2C_SLV4_DELAY_EN   : 1;
        uint8_t RSVD                : 2;
        uint8_t DELAY_ES_SHADOW     : 1;
    } bits;
    uint8_t byte;
} I2C_MST_DELAY_CTRL;

// All I2C Targets are formatted the same
typedef union {
    struct {
        uint8_t I2C_ID             : 7;
        uint8_t I2C_SLV_RNW        : 1;
    } bits;
    uint8_t byte;
} I2C_SLV_ADDR;

typedef uint8_t I2C_SLV_REG;

typedef union {
    struct {
        uint8_t I2C_SLV_LENG       : 4;
        uint8_t I2C_SLV_GRP        : 1;
        uint8_t I2C_SLV_REG_DIS    : 1;
        uint8_t I2C_SLV_BYTE_SW    : 1;
        uint8_t I2C_SLV_EN         : 1;
    } bits;
    uint8_t byte;
} I2C_SLV_CTRL;

typedef uint8_t I2C_SLV_DO;


} // namespace decode

} // namespace imu

#endif // SRC_ICM20948TYPES_HPP