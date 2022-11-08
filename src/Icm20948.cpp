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
    if (m_bus->Initialize(1, 0x69) < 0)
    {
        return -1; // Did not initialize correctly
    }
    
    return 0;
}