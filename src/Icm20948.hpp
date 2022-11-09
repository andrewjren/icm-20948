#ifndef SRC_ICM20948_HPP
#define SRC_ICM20948_HPP

// C++ Includes
#include <memory>
#include <stdint.h>

// Project Includes
#include "Icm20948Types.hpp"
#include "Registers.hpp"
#include "InterfaceStrategy.hpp"

namespace imu {

class Icm20948Interface {
public:
  Icm20948Interface();
  ~Icm20948Interface();

  int Initialize();

private:
  // Initialization Methods
  int SetupMagnetometer();
  int EnableI2cMaster();
  int ChangeUserBank(int);

  std::unique_ptr<BusStrategy> m_bus;

  // consts
  const uint8_t kWhoAmIDefault = 0xEA; // from Datasheet
  const uint8_t kIcm20948Addr  = 0x69;
  const uint8_t kAk09916Addr   = 0x0C;
  const uint8_t kMagWhoAmIDefault = 0x09;

}; // Icm20948Interface

} // namespace imu

#endif // SRC_ICM20948_HPP