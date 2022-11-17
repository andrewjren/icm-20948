#ifndef SRC_ICM20948_HPP
#define SRC_ICM20948_HPP

// C++ Includes
#include <memory>
#include <stdint.h>

// Project Includes
#include "Icm20948Types.hpp"
#include "Registers.hpp"
#include "BusStrategy.hpp"
#include "CoreTypes.hpp"

namespace imu {

class Icm20948Interface {
public:
  Icm20948Interface();
  ~Icm20948Interface();

  core::Result Initialize();
  core::Result ConfigureGyro(const uint8_t, const uint8_t, const uint8_t, const bool, const uint8_t);
  core::Result ConfigureAccel(const uint16_t, const uint8_t, const uint8_t, const bool, const uint8_t);
  core::Result ConfigureMag(const bool, const uint8_t);

  // Get Sensor Data
  core::Result GetGyros(float &, float &, float &);
  core::Result GetAccels(float &, float &, float &);
  core::Result GetMags(float &, float &, float &);

private:
  // Utility Methods
  core::Result ChangeUserBank(int);
  core::Result ResetChip();

  // Initialization Methods
  core::Result SetClockSource();
  core::Result VerifyWhoAmI();
  core::Result SetAccelGyroModes();

  // Magnetometer Initialization Methods
  core::Result SetupMagnetometer();
  core::Result DisableI2cPassthrough();
  core::Result EnableI2cController();
  core::Result SetupRegisterBlock();
  core::Result VerifyMagWhoAmI();
  core::Result SetMagRate();

  // Read/Write Methods
  core::Result WriteMagByte(const uint8_t, const uint8_t);
  core::Result ReadWord(const UserBank0Registers, const UserBank0Registers, int16_t&);
  
  /* Local Reference for Bus Interface */
  std::unique_ptr<BusStrategy> m_bus;

  // consts
  const uint8_t kWhoAmIDefault = 0xEA; // from Datasheet
  const uint8_t kIcm20948Addr  = 0x69;
  const uint8_t kAk09916Addr   = 0x0C;
  const uint8_t kMagWhoAmIDefault = 0x09;

  // Sensor LSB Values
  double m_gyro_lsb;  // gyro and accel lsbs change with 
  double m_accel_lsb; // settings
  const double m_mag_lsb = 0.15; // uT/LSB, no change 

  
  // Gyro Configuration Values
  uint8_t m_gyro_smplrt_div;
  uint8_t m_gyro_fs_sel;
  uint8_t m_gyro_dlpfcfg;
  bool    m_gyro_fchoice;
  uint8_t m_gyro_avgcfg;

  // Accel Configuration Values
  uint16_t m_accel_smplrt_div;
  uint8_t  m_accel_fs_sel;
  uint8_t  m_accel_dlpfcfg;
  bool     m_accel_fchoice;
  uint8_t  m_accel_dec3_cfg;

  // Mag Configuration Values
  bool    m_mag_enable_single;
  uint8_t m_mag_cont_meas_mode;
  

}; // Icm20948Interface

} // namespace imu

#endif // SRC_ICM20948_HPP