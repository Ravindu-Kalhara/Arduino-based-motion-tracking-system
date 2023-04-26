#ifndef Gyroscope_h
#define Gyroscope_h

#include <Wire.h>

class Gyroscope : public TwoWire {
private:
  uint8_t MPU6050_ADDRESS, DLPF_FRQ, FSR;
  float GYRO_LSB_SENSITIVITY;
  float* omega = new float[3];

  float getLSBSensitivity(uint8_t FSR_) {
    // LSB Sensitivity (LSB / degree / s) for gyroscope readings according to FSR (page 31)
    switch (FSR_) {
      case 0x00:
        return 131.0;
      case 0x08:
        return 65.5;
      case 0x10:
        return 32.8;
      case 0x18:
        return 16.4;
      default:
        return 32.8;
    }
  }

public:
  Gyroscope(uint8_t MPU6050_ADDRESS_, uint8_t DLPF_FRQ_, uint8_t FSR_) {
    MPU6050_ADDRESS = MPU6050_ADDRESS_;
    DLPF_FRQ = DLPF_FRQ_;
    FSR = FSR_;
  }

  ~Gyroscope() {
    delete[] omega;
  }

  float* getReadings() {
    TwoWire::beginTransmission(MPU6050_ADDRESS);
    TwoWire::write(0x1A);      // trigger the register for external FSYNC pin sampling and the DLPF setting for both the gyroscope and accelerometer.
    TwoWire::write(DLPF_FRQ);  // set DLPF (page 13)
    TwoWire::endTransmission();

    TwoWire::beginTransmission(MPU6050_ADDRESS);
    TwoWire::write(0x1B);  // trigger the gyroscope self-test and configure the gyroscope FSR
    TwoWire::write(FSR);   // set the gyroscope full scale range (page 14)
    TwoWire::endTransmission();

    TwoWire::beginTransmission(MPU6050_ADDRESS);
    TwoWire::write(0x43);  // trigger registers which store the most recent gyroscope measurements (page 31)
    TwoWire::endTransmission();

    TwoWire::requestFrom((int)MPU6050_ADDRESS, 6);        // get the output of 6 gyroscope registers starting from 43 to 48
    int16_t gx = TwoWire::read() << 8 | TwoWire::read();  // read gyroscope mesurements (page 31)
    int16_t gy = TwoWire::read() << 8 | TwoWire::read();
    int16_t gz = TwoWire::read() << 8 | TwoWire::read();

    // Convert the LSB measurements to degree per seconds (page 31).
    GYRO_LSB_SENSITIVITY = getLSBSensitivity(FSR);
    omega[0] = (float)gx / GYRO_LSB_SENSITIVITY;
    omega[1] = (float)gy / GYRO_LSB_SENSITIVITY;
    omega[2] = (float)gz / GYRO_LSB_SENSITIVITY;

    return omega;
  }
};

#endif