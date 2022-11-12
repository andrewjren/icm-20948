#ifndef SRC_ICM20948_HPP
#define SRC_ICM20948_HPP

// C++ Includes
#include <memory>
#include <stdint.h>

// Project Includes
#include "Icm20948Types.hpp"
#include "Registers.hpp"
#include "InterfaceStrategy.hpp"
#include "CoreTypes.hpp"

namespace imu {

class Icm20948Interface {
public:
  Icm20948Interface();
  ~Icm20948Interface();

  core::Result Initialize();

private:
  // Utility Methods
  core::Result SetupMagnetometer();
  core::Result EnableI2cMaster();
  core::Result ChangeUserBank(int);
  core::Result DisableI2cMasterPassthrough();
  core::Result ResetChip();
  core::Result StopSleeping();
  core::Result SetPower();
  core::Result VerifyMagWhoAmI();
  core::Result SetupRegisterBlock();
  core::Result SetMagRate();
  core::Result SetAccelGyroModes();

  // Read/Write Methods
  core::Result ReadMagByte(const uint8_t, uint8_t&);
  

  std::unique_ptr<BusStrategy> m_bus;

  // consts
  const uint8_t kWhoAmIDefault = 0xEA; // from Datasheet
  const uint8_t kIcm20948Addr  = 0x69;
  const uint8_t kAk09916Addr   = 0x0C;
  const uint8_t kMagWhoAmIDefault = 0x09;

}; // Icm20948Interface

} // namespace imu

#endif // SRC_ICM20948_HPP