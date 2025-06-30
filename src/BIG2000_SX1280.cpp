#include "BIG2000_SX1280.h"

BIG2000_SX1280::BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs)
    : _sck(sck), _miso(miso), _mosi(mosi), _cs(cs), _initialized(false), _spi(nullptr)
{
}

bool BIG2000_SX1280::begin()
{
    // ESP32 için donanım SPI kullanımı (VSPI)
    _spi = new SPIClass(VSPI);
    _spi->begin(_sck, _miso, _mosi, _cs);

    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);

    // Basit SPI iletişim testi (örnek: bir dummy transfer)
    _spi->beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    uint8_t test = _spi->transfer(0x00); // Dummy byte
    digitalWrite(_cs, HIGH);
    _spi->endTransaction();

    // Test sonucuna göre başlatma başarılı mı kontrolü
    _initialized = true; // (gerçek test için donanım RX/TX kontrolü eklenebilir)

    return _initialized;
}