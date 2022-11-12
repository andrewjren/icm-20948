

// Project Includes
#include "Icm20948.hpp"
#include "CoreTypes.hpp"

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

    // ResetChip()

    // StopSleeping()

    // SetPower()

    // Set Clock Source
    // select auto clock source
    imu::decode::PWR_MGMT_1 pwr_mgmt_1;
    pwr_mgmt_1.bits.CLKSEL = 1; // select best available clock, PLL if available, otherwise 20 MHz clock
    pwr_mgmt_1.bits.DEVICE_RESET = 0;
    pwr_mgmt_1.bits.SLEEP = 0;

    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kPWR_MGMT_1), pwr_mgmt_1.byte) < 0)
    {
        return core::kBusFail;
    }

    // Set Sample Mode for Gyro and Accel

    // Set Full Scale Range for Gyro and Accel

    // Set Low Pass Filter for Gyro and Accell

    // SetupMagnetometer
    
    return core::kGood;
}

/** 
 * Sets up AK09916 as I2C Slave on secondary I2C Bus
*/
core::Result imu::Icm20948Interface::SetupMagnetometer()
{
    if (core::kGood != DisableI2cMasterPassthrough()) {
        return core::kFail;
    }
    
    if (core::kGood != EnableI2cMaster()) {
        return core::kFail;
    }

    // Setup Magnetometer Register Addresses
    if (core::kGood != SetupRegisterBlock()) {
        return core::kFail;
    }

    // Verify Mag Who Am I
    if (core::kGood != VerifyMagWhoAmI()) {
        return core::kFail;
    }

    // Set Magnetometer rate

    return core::kGood;
}

/**
 * Enables I2C Master on ICM 20948
*/
core::Result imu::Icm20948Interface::EnableI2cMaster()
{
    // Change to UserBank0
    if (ChangeUserBank(0) < 0)
    {
        return core::kBusFail;
    }

    imu::decode::USER_CTRL user_ctrl;
    user_ctrl.bits.I2C_MST_EN = 1; // enable I2C master pins
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
    i2c_mst_ctrl.bits.MULT_MST_EN = 0; // disable multi master mode

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
 * Disables I2C Master Passthrough by ensuring SCL/SDA and AUX_CL/AUX_DA are not shorted
*/
core::Result imu::Icm20948Interface::DisableI2cMasterPassthrough()
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

    // Set Disable I2C Master Passthrough
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

    return core::kGood;
}

core::Result imu::Icm20948Interface::VerifyMagWhoAmI()
{
    uint8_t mag_wia2;

    if (core::kGood != m_bus->Read(static_cast<uint8_t>(imu::UserBank0Registers::kMAG_WIA2), mag_wia2))
    {
        return core::kBusFail;
    }

    if (kMagWhoAmIDefault != mag_wia2)
    {
        return core::kLogicFail;
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