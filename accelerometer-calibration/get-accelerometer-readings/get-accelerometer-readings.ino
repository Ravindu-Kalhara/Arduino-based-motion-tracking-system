#include <Wire.h>
#include "Accelerometer.h"

#define MPU6050_ADDRESS 0x68
Accelerometer Acc(MPU6050_ADDRESS, 0x05, 0x10);
float *acc;

void setup()
{
    Serial.begin(115200);
    Wire.setClock(400000); // set I2C clock speed (page 5)
    Wire.begin();

    Wire.beginTransmission(MPU6050_ADDRESS); // start the I2C communication (page 45)
    Wire.write(0x6B);                        // start the gyro power register in power mode (page 40)
    Wire.write(0x00);                        // command to maintain the power mode (page 40)
    Wire.endTransmission();                  // terminate I2C communication
}

void loop()
{
    acc = Acc.getReadings();
    Serial.print(acc[0], 8);
    Serial.print(',');
    Serial.print(acc[1], 8);
    Serial.print(',');
    Serial.println(acc[2], 8);

    delay(30);
}