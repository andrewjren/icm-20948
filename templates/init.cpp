#include "Icm20948.hpp"
#include "CoreTypes.hpp"
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    std::unique_ptr<imu::Icm20948Interface> icm(new imu::Icm20948Interface);

    if (core::kGood != icm->Initialize())
    {
        std::cout << "Failed to Initialize Properly!" << std::endl;
        return -1;
    }

    float gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, mag_x, mag_y, mag_z;
    while(true)
    {
        auto start = std::chrono::system_clock::now();
        if (core::kGood != icm->GetGyros(gyro_x, gyro_y, gyro_z)) {
            std::cout << "Gyro Faulty Value!" << std::endl;
            break;
        }
        if (core::kGood != icm->GetAccels(accel_x, accel_y, accel_z)) {
            std::cout << "Accel Faulty Value!" << std::endl;
            break;
        }
        if (core::kGood != icm->GetMags(mag_x, mag_y, mag_z)) {
            std::cout << "Mag Faulty Value!" << std::endl;
            break;
        }
        
        auto end = std::chrono::system_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
        std::cout << "Gyro: " << gyro_x << ", " << gyro_y << ", " << gyro_z << std::endl;
        std::cout << "Accel: " << accel_x << ", " << accel_y << ", " << accel_z << std::endl;
        std::cout << "Mag: " << mag_x << ", " << mag_y << ", " << mag_z << std::endl;
        std::cout << "duration in us: " << duration_us << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}