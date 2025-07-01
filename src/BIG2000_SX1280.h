#ifndef BIG2000_SX1280_H
#define BIG2000_SX1280_H

#include <Arduino.h>
#include <SPI.h>

// Hata kodları
typedef enum
{
    SX1280_OK = 0,
    SX1280_ERROR_NOT_INITIALIZED,
    SX1280_ERROR_NULL_PTR,
    SX1280_ERROR_SPI,
    SX1280_ERROR_INVALID_PARAM,
    SX1280_ERROR_UNKNOWN
} SX1280_Status_t;

class BIG2000_SX1280
{
public:
    BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs);

    bool begin();
    bool isInitialized() const { return _initialized; }

    // Tekli register işlemleri
    SX1280_Status_t readRegister(uint16_t addr, uint8_t *value);
    SX1280_Status_t writeRegister(uint16_t addr, uint8_t value);

    // Çoklu (burst) register işlemleri
    SX1280_Status_t burstReadRegister(uint16_t addr, uint8_t *buffer, size_t len);
    SX1280_Status_t burstWriteRegister(uint16_t addr, const uint8_t *buffer, size_t len);

    // Hata yönetimi
    SX1280_Status_t getLastError() const;
    const char *statusToString(SX1280_Status_t status) const;

private:
    uint8_t _sck, _miso, _mosi, _cs;
    bool _initialized;
    SPIClass *_spi;
    SX1280_Status_t _lastError;

    void spiBeginTransaction();
    void spiEndTransaction();
    void csSelect();
    void csDeselect();
};

#endif // BIG2000_SX1280_H