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
        auto start = std::chrono::system_clock::now();
        gyrox = icm->GetGyroX();
        //std::cout << "Gyro X: " << gyrox << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto end = std::chrono::system_clock::now();

        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "duration in us: " << duration_us << std::endl;
    }
    return 0;
}