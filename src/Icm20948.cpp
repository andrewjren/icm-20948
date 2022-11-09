#include "Icm20948.hpp"

imu::Icm20948Interface::Icm20948Interface()
:
m_bus(nullptr)
{ }

imu::Icm20948Interface::~Icm20948Interface()
{ }

int imu::Icm20948Interface::Initialize()
{
    // allocate memory for bus writer
    m_bus.reset(new imu::I2cStrategy());

    // initialize bus
    if (m_bus->Initialize(1, kIcm20948Addr) < 0)
    {
        return -1; // Did not initialize correctly
    }

    // CheckWhoAmI()
    // Change User Bank to 0
    if (ChangeUserBank(0) < 0)
    {
        return -2; 
    }

    // determine WHO_AM_I
    imu::decode::WHO_AM_I who_am_i;
    if (m_bus->Read(static_cast<uint8_t>(imu::UserBank0Registers::kWHO_AM_I), who_am_i) < 0)
    {
        return -3;
    }

    // verify WHO_AM_I is correct
    if (kWhoAmIDefault != who_am_i)
    {
        return -4;
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
        return -5;
    }

    // Set Sample Mode for Gyro and Accel

    // Set Full Scale Range for Gyro and Accel

    // Set Low Pass Filter for Gyro and Accell

    // SetupMagnetometer
    
    return 0;
}

/** SetupMagnetometer sets up AK09916
 * 
*/
int imu::Icm20948Interface::SetupMagnetometer()
{
    // Disable I2c Master Passthrough

    // EnableI2cMaster

    // Verify Mag Who Am I

    // Set Magnetometer rate

    // configure i2c master to read magnetometer

}

/**
 * 
*/
int imu::Icm20948Interface::EnableI2cMaster()
{
    // Change to UserBank0
    if (ChangeUserBank(0) < 0)
    {
        return -1;
    }

    imu::decode::USER_CTRL user_ctrl;
    user_ctrl.bits.I2C_MST_EN = 1; // enable I2C master pins
    user_ctrl.bits.DMP_EN = 0;
    user_ctrl.bits.DMP_RST = 0;
    user_ctrl.bits.FIFO_EN = 0;
    user_ctrl.bits.I2C_IF_DS = 0;
    user_ctrl.bits.I2C_MST_RST = 0;
    user_ctrl.bits.SRAM_RST = 0;
    
    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kUSER_CTRL), user_ctrl.byte) < 0)
    {
        return -1;
    }

    // Change to UserBank3
    if (ChangeUserBank(3) < 0)
    {
        return -1;
    }

    imu::decode::I2C_MST_CTRL i2c_mst_ctrl;
    i2c_mst_ctrl.bits.I2C_MST_CLK = 7; // 400 Hz
    i2c_mst_ctrl.bits.I2C_MST_P_NSR = 0;
    i2c_mst_ctrl.bits.MULT_MST_EN = 0;

    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank3Registers::kI2C_MST_CTRL), i2c_mst_ctrl.byte) < 0)
    {
        return -1;
    }

    // Change to UserBank0
    if (ChangeUserBank(0) < 0)
    {
        return -1;
    }

    return 0;
}

int imu::Icm20948Interface::ChangeUserBank(int user_bank)
{
    // range check desired user bank
    if (user_bank < 0 || user_bank > 3)
    {
        return -1;
    }

    // set selected user bank
    imu::decode::REG_BANK_SEL reg_bank_sel;
    reg_bank_sel.bits.USER_BANK = user_bank;

    // User Bank selection is on the same register on User Banks 0-3
    if (m_bus->Write(static_cast<uint8_t>(imu::UserBank0Registers::kREG_BANK_SEL), reg_bank_sel.byte) < 0)
    {
        return -2; 
    }

    return 0;
}