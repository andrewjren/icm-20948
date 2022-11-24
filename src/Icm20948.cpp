// Project Includes
#include "Icm20948.hpp"
#include "CoreTypes.hpp"
#include <iostream> // TODO: Remove iostream logging
#include <unistd.h> // sleep()

imu::Icm20948Interface::Icm20948Interface()
:
m_gyro_smplrt_div(0),
m_gyro_fs_sel(0),
m_gyro_dlpfcfg(0),
m_gyro_fchoice(0),
m_gyro_avgcfg(0),
m_accel_smplrt_div(0),
m_accel_fs_sel(0),
m_accel_dlpfcfg(0),
m_accel_fchoice(0),
m_accel_dec3_cfg(0),
m_mag_enable_single(0),
m_mag_cont_meas_mode(0),
m_gyro_lsb(0),
m_accel_lsb(0),
m_bus(nullptr)
{ }

imu::Icm20948Interface::~Icm20948Interface()
{
    m_bus.reset(); 
}

core::Result imu::Icm20948Interface::Initialize()
{
    // allocate memory for bus writer
    m_bus.reset(new imu::I2cStrategy());

    // Initialize Bus
    if (m_bus->Initialize(1, kIcm20948Addr) < 0) {
        std::cout << "Failed Bus Init" << std::endl;
        return core::kBusFail; // Did not initialize correctly
    }

    // Verify ICM-20948 Who Am I
    if (core::kGood != VerifyWhoAmI()) {
        std::cout << "Failed Who Am I Check" << std::endl;
        return core::kFail;
    }

    // Trigger Reset Chip
    if (core::kGood != ResetChip()) {
        std::cout << "Failed ResetChip" << std::endl;
        return core::kFail;
    }

    // Set Clock Source to Optimal
    if (core::kGood != SetClockSource()) {
        std::cout << "Failed Set Clock Source" << std::endl;
        return core::kFail;
    }

    // Set Full scales for Gyro and Accel
    if (core::kGood != SetAccelGyroModes()) {
        std::cout << "Failed Accel Gyro Modes" << std::endl;
        return core::kFail;
    }

    // SetupMagnetometer
    if (core::kGood != SetupMagnetometer()) {
        std::cout << "Failed Magnetometer Setup" << std::endl;
        return core::kFail;
    }

    // Select User Bank 0
    if (core::kGood != ChangeUserBank(0)) {
        std::cout << "Failed Changing User Bank" << std::endl;
        return core::kFail;
    }
    
    return core::kGood;
}

core::Result imu::Icm20948Interface::ConfigureGyro(
    const uint8_t gyro_smplrt_div, // 0 to 255
    const uint8_t gyro_fs_sel,     // 0 to 3
    const uint8_t gyro_dlpfcfg,    // 0 to 7
    const bool    gyro_fchoice,    // enable filter
    const uint8_t gyro_avgcfg      // 0 to 7
)
{
    if (255 >= gyro_smplrt_div) {
        m_gyro_smplrt_div = gyro_smplrt_div;
    }

    if (3 >= gyro_fs_sel) {
        m_gyro_fs_sel = gyro_fs_sel;
    }

    // set gyro lsb based on fs sel
    switch (m_gyro_fs_sel) {
        case 0:
            m_gyro_lsb = 1.0/131.0;
            break;
        case 1:
            m_gyro_lsb = 1.0/65.5;
            break;
        case 2:
            m_gyro_lsb = 1.0/32.8;
            break;
        case 3:
            m_gyro_lsb = 1.0/16.4;
            break;
        default: break;
    }

    if (7 >= gyro_dlpfcfg) {
        m_gyro_dlpfcfg = gyro_dlpfcfg;
    }

    m_gyro_fchoice = gyro_fchoice;

    if (7 >= gyro_avgcfg) {
        m_gyro_avgcfg = gyro_avgcfg;
    }
    
    return core::kGood;
}

core::Result imu::Icm20948Interface::ConfigureAccel(
    const uint16_t accel_smplrt_div,  // 0 to 4095
    const uint8_t  accel_fs_sel,      // 0 to 3
    const uint8_t  accel_dlpfcfg,     // 0 to 7
    const bool     accel_fchoice,     // enable filter
    const uint8_t  accel_dec3_cfg     // 0 to 3
)
{
    if (4095 >= accel_smplrt_div) {
        m_accel_smplrt_div = accel_smplrt_div;
    }

    if (3 >= accel_fs_sel) {
        m_accel_fs_sel = accel_fs_sel;
    }

    // set accel lsb based on fs sel
    switch (m_accel_fs_sel) {
        case 0:
            m_accel_lsb = 1.0/16384.0;
            break;
        case 1:
            m_accel_lsb = 1.0/8192.0;
            break;
        case 2:
            m_accel_lsb = 1.0/4096.0;
            break;
        case 3:
            m_accel_lsb = 1.0/2048.0;
            break;
        default: break;
    }

    if (7 >= accel_dlpfcfg) {
        m_accel_dlpfcfg = accel_dlpfcfg;
    }

    m_accel_fchoice = accel_fchoice;

    if (3 >= accel_dec3_cfg) {
        m_accel_dec3_cfg = accel_dec3_cfg;
    }

    return core::kGood;
}

core::Result imu::Icm20948Interface::ConfigureMag(
    const bool    enable_single,
    const uint8_t cont_meas_mode
)
{
    if (enable_single) {
        m_mag_enable_single = enable_single;
        m_mag_cont_meas_mode = 0;
    }
    else {
        m_mag_enable_single = false;
        m_mag_cont_meas_mode = cont_meas_mode;
    }

    return core::kGood;
}

/**
 * Get Gyroscope Sensor measurements in degrees/sec
*/
core::Result imu::Icm20948Interface::GetGyros(float & gyro_x, float & gyro_y, float & gyro_z)
{
    // 16 bit signed int for storing raw data from I2C
    int16_t data;

    if (core::kGood != ReadWord(UserBank0Registers::kGYRO_XOUT_H, UserBank0Registers::kGYRO_XOUT_L, data)) {
        return core::kBusFail;
    }
    gyro_x = (float) data * m_gyro_lsb;

    if (core::kGood != ReadWord(UserBank0Registers::kGYRO_YOUT_H, UserBank0Registers::kGYRO_YOUT_L, data)) {
        return core::kBusFail;
    }
    gyro_y = (float) data * m_gyro_lsb;
    
    if (core::kGood != ReadWord(UserBank0Registers::kGYRO_ZOUT_H, UserBank0Registers::kGYRO_ZOUT_L, data)) {
        return core::kBusFail;
    }
    gyro_z = (float) data * m_gyro_lsb;
    
    return core::kGood;
}

/**
 * Get Accelerometer Sensor measurements in G's
*/
core::Result imu::Icm20948Interface::GetAccels(float & accel_x, float & accel_y, float & accel_z)
{
    // 16 bit signed int for storing raw data from I2C
    int16_t data;

    if (core::kGood != ReadWord(UserBank0Registers::kACCEL_XOUT_H, UserBank0Registers::kACCEL_XOUT_L, data)) {
        return core::kBusFail;
    }
    accel_x = (float) data * m_accel_lsb;

    if (core::kGood != ReadWord(UserBank0Registers::kACCEL_YOUT_H, UserBank0Registers::kACCEL_YOUT_L, data)) {
        return core::kBusFail;
    }
    accel_y = (float) data * m_accel_lsb;
    
    if (core::kGood != ReadWord(UserBank0Registers::kACCEL_ZOUT_H, UserBank0Registers::kACCEL_ZOUT_L, data)) {
        return core::kBusFail;
    }
    accel_z = (float) data * m_accel_lsb;
    
    return core::kGood;
}

/**
 * Get Magnetometer Sensor measurements in uT's
*/
core::Result imu::Icm20948Interface::GetMags(float & mag_x, float & mag_y, float & mag_z)
{
    // 16 bit signed int for storing raw data from I2C
    int16_t data;

    if (core::kGood != ReadWord(UserBank0Registers::kMAG_HXH, UserBank0Registers::kMAG_HXL, data)) {
        return core::kBusFail;
    }
    mag_x = (float) data * m_mag_lsb;

    if (core::kGood != ReadWord(UserBank0Registers::kMAG_HYH, UserBank0Registers::kMAG_HYL, data)) {
        return core::kBusFail;
    }
    mag_y = (float) data * m_mag_lsb;
    
    if (core::kGood != ReadWord(UserBank0Registers::kMAG_HZH, UserBank0Registers::kMAG_HZL, data)) {
        return core::kBusFail;
    }
    mag_z = (float) data * m_mag_lsb;
    
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
 * Reset Chip
*/
core::Result imu::Icm20948Interface::ResetChip()
{
    if (ChangeUserBank(0) < 0) {
        return core::kBusFail; 
    }

    imu::decode::PWR_MGMT_1 pwr_mgmt_1;
    pwr_mgmt_1.bits.DEVICE_RESET = 1;

    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kPWR_MGMT_1), pwr_mgmt_1.byte) < 0)
    {
        return core::kBusFail;
    }

    sleep(1); // wait for chip to reset

    return core::kGood;
}

/**
 * Set Clock Source
*/
core::Result imu::Icm20948Interface::SetClockSource()
{
    if (ChangeUserBank(0) < 0) {
        return core::kBusFail; 
    }

    // Set Clock Source
    // select auto clock source
    imu::decode::PWR_MGMT_1 pwr_mgmt_1;
    pwr_mgmt_1.bits.CLKSEL = 1; // select best available clock, PLL if available, otherwise 20 MHz clock
    pwr_mgmt_1.bits.DEVICE_RESET = 0; // unset reset
    pwr_mgmt_1.bits.SLEEP = 0; // wake chip
    pwr_mgmt_1.bits.LP_EN = 0; // disable low power mode

    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kPWR_MGMT_1), pwr_mgmt_1.byte) < 0)
    {
        return core::kBusFail;
    }

    sleep(1);

    return core::kGood;
}

core::Result imu::Icm20948Interface::VerifyWhoAmI()
{
    // Change User Bank to 0
    if (ChangeUserBank(0) < 0) {
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

    return core::kGood;
}

core::Result imu::Icm20948Interface::SetAccelGyroModes()
{
    // Set Full Scale Range for Gyro and Accel
    // Change User Bank to 2
    if (core::kGood != ChangeUserBank(2))
    {
        return core::kBusFail; 
    }

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kGYRO_SMPLRT_DIV), m_gyro_smplrt_div))
    {
        std::cout << "Failed to write gyro sample rate" << std::endl;
        return core::kBusFail;
    }

    imu::decode::GYRO_CONFIG_1 gyro_config_1;
    gyro_config_1.bits.GYRO_DLPFCFG = m_gyro_dlpfcfg; // low pass filter setting   
    gyro_config_1.bits.GYRO_FCHOICE = m_gyro_fchoice; // enable low pass filter
    gyro_config_1.bits.GYRO_FS_SEL =  m_gyro_fs_sel; // dps, informs LSB of Gyro

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kGYRO_CONFIG_1), gyro_config_1.byte))
    {
        return core::kBusFail;
    }

    imu::decode::GYRO_CONFIG_2 gyro_config_2;
    gyro_config_2.bits.GYRO_AVGCFG = m_gyro_avgcfg;
    gyro_config_2.bits.XGYRO_CTEN = 0; // disable self test
    gyro_config_2.bits.YGYRO_CTEN = 0;
    gyro_config_2.bits.ZGYRO_CTEN = 0;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kGYRO_CONFIG_2), gyro_config_2.byte))
    {
        return core::kBusFail;
    }

    imu::decode::ACCEL_SMPLRT_DIV_1 accel_smplrt_div_1;
    accel_smplrt_div_1.bits.RSVD = 0;
    accel_smplrt_div_1.bits.ACCEL_SMPLRT_DIV = (0xFF00 & m_accel_smplrt_div) >> 8;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kACCEL_SMPLRT_DIV_1), accel_smplrt_div_1.byte))
    {
        return core::kBusFail;
    }

    imu::decode::ACCEL_SMPLRT_DIV_2 accel_smplrt_div_2 = 0xFF & m_accel_smplrt_div;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kACCEL_SMPLRT_DIV_2), accel_smplrt_div_2))
    {
        return core::kBusFail;
    }


    imu::decode::ACCEL_CONFIG accel_config_1;
    accel_config_1.bits.ACCEL_DLPFCFG = m_accel_dlpfcfg;
    accel_config_1.bits.ACCEL_FCHOICE = m_accel_fchoice;
    accel_config_1.bits.ACCEL_FS_SEL  = m_accel_fs_sel;

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank2Registers::kACCEL_CONFIG), accel_config_1.byte))
    {
        return core::kBusFail;
    }

    imu::decode::ACCEL_CONFIG_2 accel_config_2;
    accel_config_2.bits.DEC3_CFG = m_accel_dec3_cfg;
    accel_config_2.bits.AX_ST_EN_REG = 0; // disable self test
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
 * Sets up AK09916 as I2C Target on secondary I2C Bus
*/
core::Result imu::Icm20948Interface::SetupMagnetometer()
{
    if (core::kGood != DisableI2cPassthrough()) {
        std::cout << "Failed to disable I2C Passthrough" << std::endl;
        return core::kFail;
    }
    
    if (core::kGood != EnableI2cController()) {
        std::cout << "Failed to Enable I2C Controller" << std::endl;
        return core::kFail;
    }

    // Setup Magnetometer Register Addresses
    if (core::kGood != SetupRegisterBlock()) {
        std::cout << "Failed to set up Register Block" << std::endl;
        return core::kFail;
    }

    // Verify Mag Who Am I
    if (core::kGood != VerifyMagWhoAmI()) {
        std::cout << "Failed to Verify Mag Who Am I" << std::endl;
        return core::kFail;
    }

    // Set Magnetometer rate
    if (core::kGood != SetMagRate()) {
        std::cout << "Failed Setting up Mag Rate" << std::endl;
        return core::kFail;
    }

    return core::kGood;
}

/**
 * Disables I2C Passthrough by ensuring SCL/SDA and AUX_CL/AUX_DA are not shorted
*/
core::Result imu::Icm20948Interface::DisableI2cPassthrough()
{
    // Switch to User Bank 0
    if (core::kGood != ChangeUserBank(0))
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

/**
 * Enables I2C sub-controller on ICM 20948
*/
core::Result imu::Icm20948Interface::EnableI2cController()
{
    // Change to UserBank3
    if (core::kGood != ChangeUserBank(3))
    {
        return core::kBusFail;
    }

    imu::decode::I2C_MST_CTRL i2c_mst_ctrl;
    i2c_mst_ctrl.bits.I2C_MST_CLK = 7; // 400 Hz
    i2c_mst_ctrl.bits.I2C_MST_P_NSR = 1; // restart between reads
    i2c_mst_ctrl.bits.MULT_MST_EN = 0; // disable multi controller mode

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank3Registers::kI2C_MST_CTRL), i2c_mst_ctrl.byte))
    {
        return core::kBusFail;
    }

    // Change to UserBank0
    if (ChangeUserBank(0) < 0)
    {
        return core::kBusFail;
    }

    // Read current USER_CTRL register
    imu::decode::USER_CTRL user_ctrl;
    if (core::kGood != m_bus->Read(static_cast<uint8_t>(imu::UserBank0Registers::kUSER_CTRL), user_ctrl.byte))
    {
        return core::kBusFail;
    }

    user_ctrl.bits.I2C_MST_EN = 1; // enable I2C sub-controller pins
    
    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kUSER_CTRL), user_ctrl.byte))
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
    // Change to User Bank 0
    if (core::kGood != ChangeUserBank(0))
    {
        return core::kBusFail;
    }

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

    if (!mag_verified) {
        return core::kFail;
    }
    
    return core::kGood;
}

core::Result imu::Icm20948Interface::SetMagRate()
{
    // Set Mag Rate to 100 Hz
    imu::decode::MAG_CNTL2 mag_cntl2;
    mag_cntl2.bits.MODE = m_mag_cont_meas_mode; 
    mag_cntl2.bits.MODE_SINGLE = m_mag_enable_single;
    mag_cntl2.bits.SELF_TEST = 0; // disable self test

    if (core::kGood != WriteMagByte(static_cast<uint8_t>(MagRegisters::kCNTL2), mag_cntl2.byte)) {
        return core::kFail;
    }

    return core::kGood;
}

core::Result imu::Icm20948Interface::WriteMagByte(const uint8_t addr, const uint8_t data)
{
    // Use Target 4 to write 
    if (core::kGood != ChangeUserBank(3)) {
        return core::kBusFail; 
    }

    // Configure Target 4 for write
    imu::decode::I2C_SLV_ADDR i2c_slv04_addr;
    i2c_slv04_addr.bits.I2C_SLV_RNW = 0; // transfer is a write
    i2c_slv04_addr.bits.I2C_ID = kAk09916Addr; // magnetometer address

    if (core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank3Registers::kI2C_SLV4_ADDR), i2c_slv04_addr.byte))
    {
        return core::kBusFail;
    }

    // set Mag register to write to
    if(core::kGood != m_bus->Write(static_cast<uint8_t>(imu::UserBank3Registers::kI2C_SLV4_REG), addr))
    {
        return core::kBusFail;
    }

    // set data out for Target 4
    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank3Registers::kI2C_SLV4_DO), data))
    {
        return core::kBusFail;
    }

    // send write
    imu::decode::I2C_SLV04_CTRL i2c_slv04_ctrl;
    i2c_slv04_ctrl.bits.I2C_SLV_EN = 1;
    i2c_slv04_ctrl.bits.I2C_SLV_INT_EN = 0;
    i2c_slv04_ctrl.bits.I2C_SLV_REG_DIS = 0;
    i2c_slv04_ctrl.bits.I2C_SLV_DLY = 0;

    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank3Registers::kI2C_SLV4_CTRL), i2c_slv04_ctrl.byte))
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
