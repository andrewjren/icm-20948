# ICM-20948 Interface Library

This is a C++ based Interface Library for an ICM-20948 breakout that works natively on Raspberry Pi. 

## Features

* 100Hz output data rate, with room to get faster
* Lightweight dependencies ([SMBus](https://docs.kernel.org/i2c/summary.html) and CMake to build)

## _Why?_

There's a lot of other libraries that do this, which I heavily referenced in this project. 
They're listed below. So why make my own?

* The python based libraries depend on CircuitPython and had a limited output data rate.
* The arduino based libraries depend on Arduino-specific headers, and I wanted this to 
   work on Raspberry Pi for my application.
* making my own was more fun? idk

## ICM-20948 Sensor Configuration

The magnetometer on this package is the AK09916, which is on an isolated I2C bus. The ICM-20948 is configured to use its 
auxiliary I2C interface to get access to the magnetometer registers, where the ICM-20948 is the controller and the AK09916 is 
the target. Magnetometer registers are then mapped to `kEXT_SLV_SENS_DATA_00` through `kEXT_SLV_SENS_DATA_08`, and accessed 
through the controller interface on the ICM-20948. 

## A Note on I2C Terminology

This library uses the term "I2C Controller" in favor of "I2C Master," and "I2C Target" in favor of "I2C Slave."
However, the register names are taken directly from the ICM-20948 Datasheet, so `MST` and `SLV` are found throughout
the source. 

## Todo:

* Add callback to logging function

## References

Thanks to these libraries, much of the initializing and moding the sensor was based on their solution:

* https://github.com/stephendpmurphy/icm20948/tree/main
* https://github.com/adafruit/Adafruit_CircuitPython_ICM20X
* https://github.com/sparkfun/Qwiic_9DoF_IMU_ICM20948_Py
