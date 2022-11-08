#ifndef SRC_INTERFACESTRATEGY_HPP
#define SRC_INTERFACESTRATEGY_HPP

#include <stdint.h>

namespace imu {

/*! BusStrategy implements a Strategy Pattern
 *  BusStrategy is an interface class for a data bus (i2c, SPI) for writing ICM-20948 registers
 */
class BusStrategy {
public:
  /* read should be implemented as read from a register that fills data */
  virtual int read(const uint8_t addr, uint8_t * data) = 0;

  /* write should be implemented as a write of data to a register */
  virtual int write(const uint8_t addr, const uint8_t data) = 0;

}; // class BusStrategy

/*! I2cStrategy implements BusStrategy
 *
*/
class I2cStrategy : BusStrategy {
public:
  I2cStrategy();
  ~I2cStrategy();

  // Inherited from BusStrategy
  int read(const uint8_t addr, uint8_t * data);
  int write(const uint8_t addr, const uint8_t data);

  // Initialization
  int Initialize(const int, const int);

private:
  // i2c bus management
  int m_i2c_file;
  int m_adapter_number;
  int m_i2c_address;
  char m_filename[20];

}; // class I2cStrategy

}; // namespace imu

#endif // SRC_INTERFACESTRATEGY_HPP