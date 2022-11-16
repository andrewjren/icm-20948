# ICM-20948 Interface Library

This is a C++ based Interface Library for an ICM-20948 breakout that works natively on Raspberry Pi. 

## Features

* 100Hz output data rate, with room to get faster
* Lightweight dependencies ([SMBus](https://docs.kernel.org/i2c/summary.html) and CMake to build)

## _Why?_

There's a lot of other fantastic libraries that do this, which I heavily referenced in this project. 
They're referenced below. So why make my own?

* The python based libraries depend on CircuitPython and had a limited output data rate.
* The arduino based libraries depend on Arduino-specific headers, and I wanted this to 
   work on Raspberry Pi for my application.
* making my own was fun? idk

## ICM-20948 Sensor Configuration

TBC

## References

Thanks to these libraries, much of the initializing and moding the sensor was based on their solution:

* https://github.com/stephendpmurphy/icm20948/tree/main
* https://github.com/adafruit/Adafruit_CircuitPython_ICM20X
* https://github.com/sparkfun/Qwiic_9DoF_IMU_ICM20948_Py
