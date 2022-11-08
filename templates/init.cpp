#include "Icm20948.hpp"
#include <memory>
#include <iostream>

int main()
{
    std::unique_ptr<imu::Icm20948Interface> icm(new imu::Icm20948Interface);

    int i = icm->Initialize();

    std::cout << "Return value: " << i << std::endl;


    return 0;
}