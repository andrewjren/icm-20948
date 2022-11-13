

// Project Includes
#include "Icm20948.hpp"
#include "CoreTypes.hpp"
#include <iostream>
#include <unistd.h>

imu::Icm20948Interface::Icm20948Interface()
:
m_bus(nullptr)
{ }

imu::Icm20948Interface::~Icm20948Interface()
{ }

core::Result imu::Icm20948Interface::Initialize()
{
    // allocate memory for bus writer
    m_bus.reset(new imu::I2cStrategy());

    // initialize bus
    if (m_bus->Initialize(1, kIcm20948Addr) < 0)
    {
        return core::kBusFail; // Did not initialize correctly
    }

    // CheckWhoAmI()
    // Change User Bank to 0
    if (ChangeUserBank(0) < 0)
    {
        return core::kBusFail; 
    }

    // determine WHO_AM_I
    imu::decode::WHO_AM_I who_am_i;
    if (m_bus->Read(static_cast<uint8_t>(imu::UserBank0Registers::kWHO_AM_I), who_am_i) < 0)
    {
        return core::kBusFail;
    }

    // verify WHO_AM_I is correct
    if (kWhoAmIDefault != who_am_i)
    {
        return core::kFail;
    }

    // Set Full scales for Gyro and Accel
    if (core::kGood != SetAccelGyroModes()) {
        return core::kFail;
    }
    std::cout << "Successfully Initialized Accel and Gyro" << std::endl;

    // SetupMagnetometer
    if (core::kGood != SetupMagnetometer()) {
        return core::kFail;
    }

    // Change User Bank to 0
    if (ChangeUserBank(0) < 0)
    {
        return core::kBusFail; 
    }

    // ResetChip()
    imu::decode::PWR_MGMT_1 pwr_mgmt_1;
    pwr_mgmt_1.bits.DEVICE_RESET = 1;

    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kPWR_MGMT_1), pwr_mgmt_1.byte) < 0)
    {
        return core::kBusFail;
    }
    std::cout << "THIS SHOULD BE A SLEEP!!" << std::endl;
    sleep(1);

    // Set Clock Source
    // select auto clock source
    pwr_mgmt_1.bits.CLKSEL = 1; // select best available clock, PLL if available, otherwise 20 MHz clock
    pwr_mgmt_1.bits.DEVICE_RESET = 0; // unset reset
    pwr_mgmt_1.bits.SLEEP = 0; // wake chip
    pwr_mgmt_1.bits.LP_EN = 0; // disable low power mode

    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kPWR_MGMT_1), pwr_mgmt_1.byte) < 0)
    {
        return core::kBusFail;
    }
    std::cout << "Set Clock Source" << std::endl;

    // Select User Bank 0
    if (core::kGood != ChangeUserBank(0)) {
        return core::kFail;
    }
    
    return core::kGood;
}

float imu::Icm20948Interface::GetGyroX()
{
    int16_t data;

    if (core::kGood != ReadWord(UserBank0Registers::kGYRO_XOUT_H, UserBank0Registers::kGYRO_XOUT_L, data)) {
        return core::kBusFail;
    }

    return (float) data * m_gyro_lsb;
}

core::Result imu::Icm20948Interface::SetAccelGyroModes()
{
    // Set Full Scale Range for Gyro and Accel
    // Change User Bank to 2
    if (core::kGood != ChangeUserBank(2))
    {
        return core::kBusFail; 
    }
    std::cout << "Changed User Bank" << std::endl;

    imu::decode::GYRO_SMPLRT_DIV gyro_smplrt_div = 10;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kGYRO_SMPLRT_DIV), gyro_smplrt_div))
    {
        std::cout << "Failed to write gyro sample rate" << std::endl;
        return core::kBusFail;
    }
    std::cout << "set sample rate" << std::endl;

    imu::decode::GYRO_CONFIG_1 gyro_config_1;
    m_gyro_lsb = 1.0/131.0;
    gyro_config_1.bits.GYRO_DLPFCFG = 1; // low pass filter setting   
    gyro_config_1.bits.GYRO_FCHOICE = 1; // enable low pass filter
    gyro_config_1.bits.GYRO_FS_SEL =  0; // dps, informs LSB of Gyro

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kGYRO_CONFIG_1), gyro_config_1.byte))
    {
        return core::kBusFail;
    }
    std::cout << "configured gyro 1" << std::endl;

    imu::decode::GYRO_CONFIG_2 gyro_config_2;
    gyro_config_2.bits.GYRO_AVGCFG = 2;
    gyro_config_2.bits.XGYRO_CTEN = 0;
    gyro_config_2.bits.YGYRO_CTEN = 0;
    gyro_config_2.bits.ZGYRO_CTEN = 0;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kGYRO_CONFIG_2), gyro_config_2.byte))
    {
        return core::kBusFail;
    }
    std::cout << "configured gyro 2" << std::endl;

    imu::decode::ACCEL_SMPLRT_DIV_1 accel_smplrt_div_1;
    accel_smplrt_div_1.bits.RSVD = 0;
    accel_smplrt_div_1.bits.ACCEL_SMPLRT_DIV = 0;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kACCEL_SMPLRT_DIV_1), accel_smplrt_div_1.byte))
    {
        return core::kBusFail;
    }

    imu::decode::ACCEL_SMPLRT_DIV_2 accel_smplrt_div_2 = 10;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kACCEL_SMPLRT_DIV_2), accel_smplrt_div_2))
    {
        return core::kBusFail;
    }


    imu::decode::ACCEL_CONFIG accel_config_1;
    accel_config_1.bits.ACCEL_DLPFCFG = 7;
    accel_config_1.bits.ACCEL_FCHOICE = 1;
    accel_config_1.bits.ACCEL_FS_SEL  = 0;
    m_accel_lsb = 1/16384;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kACCEL_CONFIG), accel_config_1.byte))
    {
        return core::kBusFail;
    }
    std::cout << "configured accel 1" << std::endl;

    imu::decode::ACCEL_CONFIG_2 accel_config_2;
    accel_config_2.bits.DEC3_CFG = 1;
    accel_config_2.bits.AX_ST_EN_REG = 0;
    accel_config_2.bits.AY_ST_EN_REG = 0;
    accel_config_2.bits.AZ_ST_EN_REG = 0;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kACCEL_CONFIG_2), accel_config_2.byte))
    {
        return core::kBusFail;
    }

    if (core::kGood != ChangeUserBank(0))
    {
        return core::kBusFail; 
    }

    return core::kGood;
}

/** 
 * Sets up AK09916 as I2C Slave on secondary I2C Bus
*/
core::Result imu::Icm20948Interface::SetupMagnetometer()
{
    if (core::kGood != DisableI2cPassthrough()) {
        return core::kFail;
    }
    std::cout << "Successfully disabled I2C Passthrough" << std::endl;
    
    if (core::kGood != EnableI2cController()) {
        return core::kFail;
    }
    std::cout << "Successfully Enabled I2C Controller" << std::endl;

    // Setup Magnetometer Register Addresses
    if (core::kGood != SetupRegisterBlock()) {
        return core::kFail;
    }
    std::cout << "Successfully Set Up I2C Proxy Registers" << std::endl;

    // Verify Mag Who Am I
    if (core::kGood != VerifyMagWhoAmI()) {
        return core::kFail;
    }
    std::cout << "Successfully Verified Mag Who Am I" << std::endl;

    // Set Magnetometer rate

    return core::kGood;
}

/**
 * Enables I2C sub-controller on ICM 20948
*/
core::Result imu::Icm20948Interface::EnableI2cController()
{
    // Change to UserBank0
    if (ChangeUserBank(0) < 0)
    {
        return core::kBusFail;
    }

    imu::decode::USER_CTRL user_ctrl;
    user_ctrl.bits.I2C_MST_EN = 1; // enable I2C sub-controller pins
    user_ctrl.bits.DMP_EN = 0;
    user_ctrl.bits.DMP_RST = 0;
    user_ctrl.bits.FIFO_EN = 0;
    user_ctrl.bits.I2C_IF_DS = 0;
    user_ctrl.bits.I2C_MST_RST = 0;
    user_ctrl.bits.SRAM_RST = 0;
    
    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kUSER_CTRL), user_ctrl.byte))
    {
        return core::kBusFail;
    }

    // Change to UserBank3
    if (core::kGood != ChangeUserBank(3))
    {
        return core::kBusFail;
    }

    imu::decode::I2C_MST_CTRL i2c_mst_ctrl;
    i2c_mst_ctrl.bits.I2C_MST_CLK = 7; // 400 Hz
    i2c_mst_ctrl.bits.I2C_MST_P_NSR = 0; // restart between reads
    i2c_mst_ctrl.bits.MULT_MST_EN = 0; // disable multi controller mode

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank3Registers::kI2C_MST_CTRL), i2c_mst_ctrl.byte))
    {
        return core::kBusFail;
    }

    // Change to UserBank0
    if (core::kGood != ChangeUserBank(0))
    {
        return core::kBusFail;
    }

    return core::kGood;
}

/**
 * Disables I2C Passthrough by ensuring SCL/SDA and AUX_CL/AUX_DA are not shorted
*/
core::Result imu::Icm20948Interface::DisableI2cPassthrough()
{
    // Switch to User Bank 0
    if (ChangeUserBank(0) != core::kGood)
    {
        return core::kBusFail;
    }

    // Read current pin config
    imu::decode::INT_PIN_CFG int_pin_cfg;

    if (core::kGood != m_bus->Read(static_cast<uint8_t>(imu::UserBank0Registers::kINT_PIN_CFG), int_pin_cfg.byte))
    {
        return core::kBusFail;
    }

    // Set Disable I2C Passthrough
    int_pin_cfg.bits.BYPASS_EN = 0;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kINT_PIN_CFG), int_pin_cfg.byte))
    {
        return core::kBusFail;
    }

    return core::kGood;
}

core::Result imu::Icm20948Interface::SetupRegisterBlock()
{
    // Change to User Bank 3
    if (core::kGood != ChangeUserBank(3))
    {
        return core::kBusFail;
    }

    // Set I2C Target 0,1 to be Magnetometer address
    imu::decode::I2C_SLV_ADDR i2c_slv01_addr;
    i2c_slv01_addr.bits.I2C_SLV_RNW = 1; // transfer is a read
    i2c_slv01_addr.bits.I2C_ID = kAk09916Addr; // magnetometer address

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(UserBank3Registers::kI2C_SLV0_ADDR), i2c_slv01_addr.byte))
    {
        return core::kBusFail;
    }

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(UserBank3Registers::kI2C_SLV1_ADDR), i2c_slv01_addr.byte))
    {
        return core::kBusFail;
    }

    // Map WhoAmI to EXT_SENS_0
    // WIA Starts at 0x01
    if(core::kGood != m_bus->Write(static_cast<uint8_t>(UserBank3Registers::kI2C_SLV0_REG), 0x01))
    {
        return core::kBusFail;
    }

    imu::decode::I2C_SLV_CTRL i2c_slv0_ctrl;
    i2c_slv0_ctrl.bits.I2C_SLV_EN = 1; // enable reading
    i2c_slv0_ctrl.bits.I2C_SLV_BYTE_SW = 0; // disable byte swapping
    i2c_slv0_ctrl.bits.I2C_SLV_REG_DIS = 0;
    i2c_slv0_ctrl.bits.I2C_SLV_GRP = 0; 
    i2c_slv0_ctrl.bits.I2C_SLV_LENG = 1; // whoami is 1 byte
    
    if(core::kGood != m_bus->Write(static_cast<uint8_t>(UserBank3Registers::kI2C_SLV0_CTRL), i2c_slv0_ctrl.byte))
    {
        return core::kBusFail;
    }

    // Map Status and Magnetometer readings to EXT_SENS_1-10
    // HXL Starts at 0x10
    if(core::kGood != m_bus->Write(static_cast<uint8_t>(UserBank3Registers::kI2C_SLV1_REG), 0x10))
    {
        return core::kBusFail;
    }

    imu::decode::I2C_SLV_CTRL i2c_slv1_ctrl;
    i2c_slv1_ctrl.bits.I2C_SLV_EN = 1; // enable reading
    i2c_slv1_ctrl.bits.I2C_SLV_BYTE_SW = 0; // disable byte swapping
    i2c_slv1_ctrl.bits.I2C_SLV_REG_DIS = 0;
    i2c_slv1_ctrl.bits.I2C_SLV_GRP = 0; 
    i2c_slv1_ctrl.bits.I2C_SLV_LENG = 9; // 9 bytes for payload

    if(core::kGood != m_bus->Write(static_cast<uint8_t>(UserBank3Registers::kI2C_SLV1_CTRL), i2c_slv1_ctrl.byte))
    {
        return core::kBusFail;
    }

    // Change to User Bank 0
    if (core::kGood != ChangeUserBank(0))
    {
        return core::kBusFail;
    }

    return core::kGood;
}

core::Result imu::Icm20948Interface::VerifyMagWhoAmI()
{
    uint8_t mag_wia2;
    bool mag_verified = false;
    int count = 0;
    
    while (count < 5 && !mag_verified)
    {
        m_bus->Read(static_cast<uint8_t>(imu::UserBank0Registers::kMAG_WIA2), mag_wia2);

        if (kMagWhoAmIDefault == mag_wia2)
        {
            mag_verified = true;
        }
        count++;
    }

    return core::kGood;
}

core::Result imu::Icm20948Interface::SetMagRate()
{
    return core::kGood;
}

core::Result imu::Icm20948Interface::ChangeUserBank(int user_bank)
{
    // range check desired user bank
    if (user_bank < 0 || user_bank > 3)
    {
        return core::kFail;
    }

    // set selected user bank
    imu::decode::REG_BANK_SEL reg_bank_sel;
    reg_bank_sel.bits.USER_BANK = user_bank;

    // User Bank selection is on the same register on User Banks 0-3
    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kREG_BANK_SEL), reg_bank_sel.byte) < 0)
    {
        return core::kBusFail; 
    }

    return core::kGood;
}

/**
 * Read Word from I2C Interface
*/
core::Result imu::Icm20948Interface::ReadWord(const UserBank0Registers reg_h, const UserBank0Registers reg_l, int16_t & data)
{
    // get addresses
    const uint8_t addr_h = static_cast<uint8_t>(reg_h);
    const uint8_t addr_l = static_cast<uint8_t>(reg_l);

    // allocate storage for data
    uint8_t data_h;
    uint8_t data_l;

    if (core::kGood != m_bus->Read(addr_h, data_h)) {
        return core::kBusFail;
    }

    if (core::kGood != m_bus->Read(addr_l, data_l)) {
        return core::kBusFail;
    }

    data = (int16_t) data_h << 8 | data_l;

    return core::kGood;
}