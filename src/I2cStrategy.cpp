#include "InterfaceStrategy.hpp"

// C Includes
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <cstdio>
#include <fcntl.h>




imu::I2cStrategy::I2cStrategy()
{

}

imu::I2cStrategy::~I2cStrategy()
{

}

/** Initialize I2C Interface
 *  Reference: https://www.kernel.org/doc/Documentation/i2c/dev-interface
 */
int imu::I2cStrategy::Initialize(const int adapter_number, const int i2c_address)
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
        return -1;
    }

    // Check if file descriptor can be controlled
    if (ioctl(m_i2c_file, I2C_SLAVE, m_i2c_address) < 0)
    {
        return -2;
    }

    return 0;
}

/* Use smbus call to read byte data from i2c */
int imu::I2cStrategy::read(const uint8_t addr, uint8_t& data)
{
    int32_t response = i2c_smbus_read_byte_data(m_i2c_file, addr);

    // i2c returns negative value when failed
    if (response < 0)
    {
        return -1;
    }
    
    data = response & 0xFF;
    return 0;
}

/* Use smbus call to write byte data to i2c */
int imu::I2cStrategy::write(const uint8_t addr, const uint8_t data)
{
    int32_t response = i2c_smbus_write_byte_data(m_i2c_file, addr, data);

    // i2c returns negative value when failed
    if (response < 0)
    {
        return -1;
    }

    return 0;
}