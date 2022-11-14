#include "Icm20948.hpp"
#include "CoreTypes.hpp"
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    std::unique_ptr<imu::Icm20948Interface> icm(new imu::Icm20948Interface);

    int i = icm->Initialize();

    std::cout << "Initialize Return value: " << i << std::endl;
    float gyro_x, gyro_y, gyro_z;
    while(true)
    {
        auto start = std::chrono::system_clock::now();
        if (core::kGood != icm->GetGyros(gyro_x, gyro_y, gyro_z)) {
            std::cout << "Faulty Value!" << std::endl;
            break;
        }
        
        auto end = std::chrono::system_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
        std::cout << "Gyro X: " << gyro_x << "Gyro Y: " << gyro_y << "Gyro Z: " << gyro_z << std::endl;
        std::cout << "duration in us: " << duration_us << std::endl;
    }
    return 0;
}