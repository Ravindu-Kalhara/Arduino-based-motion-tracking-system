#ifndef Magnetometer_h
#define Magnetometer_h

#include <Wire.h>

class Magnetometer : public TwoWire {
private:
  uint8_t HMC5883_ADDRESS, DOUTPUT_RATE, GAIN, MEASUREMENT_MODE;
  uint16_t GAIN_VAL;
  float* mag = new float[3];

  uint16_t getGain(uint8_t GAIN_) {
    // gain (LSb / Gauss) (page 16)
    switch (GAIN_) {
      case 0x00:
        return 1370;
      case 0x20:
        return 1090;
      case 0x40:
        return 820;
      case 0x60:
        return 660;
      case 0x80:
        return 440;
      case 0xA0:
        return 390;
      case 0xC0:
        return 330;
      case 0xE0:
        return 230;
      default:
        return 1090;
    }
  }

public:
  Magnetometer(uint8_t HMC5883_ADDRESS_, uint8_t DOUTPUT_RATE_, uint8_t GAIN_, uint8_t MEASUREMENT_MODE_) {
    HMC5883_ADDRESS = HMC5883_ADDRESS_;
    DOUTPUT_RATE = DOUTPUT_RATE_;
    GAIN = GAIN_;
    MEASUREMENT_MODE = MEASUREMENT_MODE_;
  }

  ~Magnetometer() {
    delete[] mag;
  }

  float* getReadings() {
    TwoWire::beginTransmission(HMC5883_ADDRESS);
    TwoWire::write(0x00);          // trigger the data output rate and measurement configuration register (page 14)
    TwoWire::write(DOUTPUT_RATE);  // set data output rate
    TwoWire::endTransmission();

    TwoWire::beginTransmission(HMC5883_ADDRESS);
    TwoWire::write(0x01);  // trigger the configuration register B for setting the device gain (page 15)
    TwoWire::write(GAIN);  // set the gain
    TwoWire::endTransmission();

    TwoWire::beginTransmission(HMC5883_ADDRESS);
    TwoWire::write(0x02);              // trigger the register used to select the operating mode of the device. (page 17)
    TwoWire::write(MEASUREMENT_MODE);  // set mesurement mode
    TwoWire::endTransmission();

    TwoWire::beginTransmission(HMC5883_ADDRESS);
    TwoWire::write(0x03);  // trigger registers which store the most recent magnetometer measurements (page 18)
    TwoWire::endTransmission();

    TwoWire::requestFrom((int)HMC5883_ADDRESS, 6);  // get the magnetometer readings (page 18 , 19)
    int16_t mx, my, mz;
    if (TwoWire::available() >= 6) {
      mx = TwoWire::read() << 8 | TwoWire::read();
      my = TwoWire::read() << 8 | TwoWire::read();
      mz = TwoWire::read() << 8 | TwoWire::read();
    }

    // 1G = 0.0001 T
    // field strength = 41046.3 nT = 41.0463 uT
    GAIN_VAL = getGain(GAIN);
    mag[0] = ((float)mx / GAIN_VAL) * 100.0;  // get the readings in G -> uT (page 16)
    mag[1] = ((float)my / GAIN_VAL) * 100.0;
    mag[2] = ((float)mz / GAIN_VAL) * 100.0;

    return mag;
  }
};

#endif