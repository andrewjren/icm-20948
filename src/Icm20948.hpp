#ifndef SRC_ICM20948_HPP
#define SRC_ICM20948_HPP

// C++ Includes
#include <memory>

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
  std::unique_ptr<BusStrategy> m_bus;

}; // Icm20948Interface

} // namespace imu

#endif // SRC_ICM20948_HPP