#ifndef BIG2000_SX1280_H
#define BIG2000_SX1280_H

#include <Arduino.h>
#include <SPI.h>

class BIG2000_SX1280
{
public:
    BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs);

    bool begin();
    bool isInitialized() const { return _initialized; }

private:
    uint8_t _sck, _miso, _mosi, _cs;
    bool _initialized;
    SPIClass *_spi;
};

#endif // BIG2000_SX1280_H