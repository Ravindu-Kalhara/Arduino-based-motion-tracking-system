#include <Wire.h>
#include "Magnetometer.h"

Magnetometer Mag(0x1E, 0x14, 0x20, 0x00);
float *mag;

void setup(){
    Serial.begin(115200);
    Wire.setClock(400000); // set I2C clock speed (page 5)
    Wire.begin();
}

void loop(){
    mag = Mag.getReadings();
    Serial.print(mag[0], 4);
    Serial.print(',');
    Serial.print(mag[1], 4);
    Serial.print(',');
    Serial.println(mag[2], 4);

    delay(30);
}