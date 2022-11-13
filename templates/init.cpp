#include "Icm20948.hpp"
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    std::unique_ptr<imu::Icm20948Interface> icm(new imu::Icm20948Interface);

    int i = icm->Initialize();

    std::cout << "Initialize Return value: " << i << std::endl;
    float gyrox;
    while(true)
    {
        gyrox = icm->GetGyroX();
        std::cout << "Gyro X: " << gyrox << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}