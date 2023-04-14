#ifndef Accelerometer_h
#define Accelerometer_h

#include <Wire.h>

class Accelerometer : public TwoWire {
private:
  uint8_t MPU6050_ADDRESS, DLPF_FRQ, FSR;
  uint16_t ACCEL_LSB_SENSITIVITY;
  float* acc = new float[3];

  uint16_t getLSBSensitivity(uint8_t FSR_) {
    // LSB Sensitivity (LSB / g) for accelerometer readings according to FSR (page 29)
    switch (FSR_) {
      case 0x00:
        return 16384;
      case 0x08:
        return 8192;
      case 0x10:
        return 4096;
      case 0x18:
        return 2048;
      default:
        return 4096;
    }
  }

public:
  Accelerometer(uint8_t MPU6050_ADDRESS_, uint8_t DLPF_FRQ_, uint8_t FSR_) {
    MPU6050_ADDRESS = MPU6050_ADDRESS_;
    DLPF_FRQ = DLPF_FRQ_;
    FSR = FSR_;
  }

  ~Accelerometer() {
    delete[] acc;
  }

  float* getReadings() {
    TwoWire::beginTransmission(MPU6050_ADDRESS);
    TwoWire::write(0x1A);      // trigger the register for external FSYNC pin sampling and the DLPF setting for both the gyroscope and accelerometer.
    TwoWire::write(DLPF_FRQ);  // set DLPF (page 13)
    TwoWire::endTransmission();

    TwoWire::beginTransmission(MPU6050_ADDRESS);
    TwoWire::write(0x1C);  // trigger accelerometer to configure the accelerometer FSR (page 15).
    TwoWire::write(FSR);   // set the accelerometer FSR
    TwoWire::endTransmission();

    TwoWire::beginTransmission(MPU6050_ADDRESS);
    TwoWire::write(0x3B);  // trigger registers which store the most recent accelerometer measurements (page 29).
    TwoWire::endTransmission();

    TwoWire::requestFrom((int)MPU6050_ADDRESS, 6);  // get the output from 6 accelerometer registers starting from 3B to 40 (page 29)
    int16_t accLSBX = TwoWire::read() << 8 | TwoWire::read();
    int16_t accLSBY = TwoWire::read() << 8 | TwoWire::read();
    int16_t accLSBZ = TwoWire::read() << 8 | TwoWire::read();

    // convert LSB to g (page 29).
    ACCEL_LSB_SENSITIVITY = getLSBSensitivity(FSR);
    acc[0] = (float)accLSBX / ACCEL_LSB_SENSITIVITY;
    acc[1] = (float)accLSBY / ACCEL_LSB_SENSITIVITY;
    acc[2] = (float)accLSBZ / ACCEL_LSB_SENSITIVITY;

    return acc;
  }
};

#endif