#ifndef SRC_INTERFACESTRATEGY_HPP
#define SRC_INTERFACESTRATEGY_HPP

#include <stdint.h>

#include "CoreTypes.hpp"

namespace imu {

/*! BusStrategy implements a Strategy Pattern
 *  BusStrategy is an interface class for a data bus (i2c, SPI) for writing ICM-20948 registers
 */
class BusStrategy {
public:
  /* read should be implemented as read from a register that fills data */
  virtual core::Result Read(const uint8_t addr, uint8_t & data) = 0;

  /* write should be implemented as a write of data to a register */
  virtual core::Result Write(const uint8_t addr, const uint8_t data) = 0;

  /* initialize */
  virtual core::Result Initialize(const int, const int) = 0;

}; // class BusStrategy

/*! I2cStrategy implements BusStrategy
 *
*/
class I2cStrategy : public BusStrategy {
public:
  I2cStrategy();
  ~I2cStrategy();

  // Inherited from BusStrategy
  core::Result Read(const uint8_t addr, uint8_t & data) override;
  core::Result Write(const uint8_t addr, const uint8_t data) override;

  // Initialization
  core::Result Initialize(const int, const int) override;

private:
  // i2c bus management
  int m_i2c_file;
  int m_adapter_number;
  int m_i2c_address;
  char m_filename[20];

}; // class I2cStrategy

}; // namespace imu

#endif // SRC_INTERFACESTRATEGY_HPP