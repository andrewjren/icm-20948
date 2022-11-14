#include "InterfaceStrategy.hpp"

// C Includes
#include <cstdio>
#include <fcntl.h>
#include <sys/ioctl.h>

// Project Includes
#include "CoreTypes.hpp"

// https://stackoverflow.com/questions/50154296/undefined-reference-to-i2c-smbus-read-word-dataint-unsigned-char
// needs to be wrapped in extern C statement because it is not C++ ready
extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

imu::I2cStrategy::I2cStrategy()
{

}

imu::I2cStrategy::~I2cStrategy()
{

}

/** Initialize I2C Interface
 *  Reference: https://www.kernel.org/doc/Documentation/i2c/dev-interface
 */
core::Result imu::I2cStrategy::Initialize(const int adapter_number, const int i2c_address)
{
    // Set I2C Config values
    m_adapter_number = adapter_number;
    m_i2c_address = i2c_address;

    // Initialize file descriptor for i2c
    sprintf(m_filename, "/dev/i2c-%d", m_adapter_number);
    m_i2c_file = open(m_filename, O_RDWR);
    
    // Check if file descriptor is valid
    if (m_i2c_file < 0)
    {
        return core::kFail;
    }

    // Check if file descriptor can be controlled
    if (ioctl(m_i2c_file, I2C_SLAVE, m_i2c_address) < 0)
    {
        return core::kFail;
    }

    return core::kGood;
}

/* Use smbus call to read byte data from i2c */
core::Result imu::I2cStrategy::Read(const uint8_t addr, uint8_t& data)
{
    int32_t response = i2c_smbus_read_byte_data(m_i2c_file, addr);

    // i2c returns negative value when failed
    if (response < 0)
    {
        return core::kBusFail;
    }
    
    data = response & 0xFF;
    return core::kGood;
}

/* Use smbus call to write byte data to i2c */
core::Result imu::I2cStrategy::Write(const uint8_t addr, const uint8_t data)
{
    int32_t response = i2c_smbus_write_byte_data(m_i2c_file, addr, data);

    // i2c returns negative value when failed
    if (response < 0)
    {
        return core::kBusFail;
    }

    return core::kGood;
}