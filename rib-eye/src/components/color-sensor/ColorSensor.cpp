#include "ColorSensor.h"
#include "Wire.h"

ColorSensor::ColorSensor() 
    : tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X),
      isInit(false),
      lastReadTime(0),
      lastRGB{0.0f, 0.0f, 0.0f} {}

bool ColorSensor::init() {
    if(tcs.begin(0x29, &Wire)) {
        isInit = true;
        setLed(true);
        return true;
    }
    return false;
}

void ColorSensor::setLed(bool s) {
    if(!isInit) return;
    tcs.setInterrupt(!s);
}

bool ColorSensor::readAsync(RGB &color, unsigned long intervalMS) {
    if(!isInit) return false;
    unsigned long currentMillis = millis();
    if(currentMillis - lastReadTime >= intervalMS) {
        lastReadTime = currentMillis;

        uint16_t c, r, g, b;
        // literalmente lo mismo que el getRGB (normalizazion)
        if(readRegisters(c, r, g, b) && c > 0) {
            lastRGB.r = (float)r / c * 255.0f;
            lastRGB.g = (float)g / c * 255.0f;
            lastRGB.b = (float)b / c * 255.0f;
        }
        tcs.getRGB(&lastRGB.r, &lastRGB.g, &lastRGB.b);

        color = lastRGB;
        return true;
    }

    color = lastRGB;
    return false;
}

// Optimizacion para los registros (para que pese menos)
// Sacado de la datasheet del sensor
bool ColorSensor::readRegisters(uint16_t &c, uint16_t &r, uint16_t &g, uint16_t &b) {
    constexpr uint8_t i2c = 0x29;
    constexpr uint8_t kCmdAutoInc = 0x80 | 0x20; // bit y autoincremento
    constexpr uint8_t kRegCDataL = 0x14; // 2 bytes, LSB primero

    Wire.beginTransmission(i2c);
    Wire.write(kCmdAutoInc | kRegCDataL);
    if(Wire.endTransmission() != 0) return false;
    if(Wire.requestFrom(i2c, (uint8_t)8) != 8) return false;

    uint8_t buf[8];
    for(uint8_t i = 0; i < 8; i++) buf[i] = Wire.read();

    c = (uint16_t)(buf[0] | (buf[1] << 8));
    r = (uint16_t)(buf[2] | (buf[3] << 8));
    g = (uint16_t)(buf[4] | (buf[5] << 8));
    b = (uint16_t)(buf[6] | (buf[7] << 8));
    return true;

}