#include "BIG2000_SX1280.h"

BIG2000_SX1280::BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs)
    : _sck(sck), _miso(miso), _mosi(mosi), _cs(cs), _initialized(false), _spi(nullptr), _lastError(SX1280_OK)
{
}

bool BIG2000_SX1280::begin()
{
    _spi = new SPIClass(VSPI);
    _spi->begin(_sck, _miso, _mosi, _cs);
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
    _initialized = true;
    _lastError = SX1280_OK;
    return _initialized;
}

void BIG2000_SX1280::spiBeginTransaction()
{
    _spi->beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
}

void BIG2000_SX1280::spiEndTransaction()
{
    _spi->endTransaction();
}

void BIG2000_SX1280::csSelect()
{
    digitalWrite(_cs, LOW);
}

void BIG2000_SX1280::csDeselect()
{
    digitalWrite(_cs, HIGH);
}

SX1280_Status_t BIG2000_SX1280::readRegister(uint16_t addr, uint8_t *value)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }
    if (value == nullptr)
    {
        _lastError = SX1280_ERROR_NULL_PTR;
        return _lastError;
    }
    spiBeginTransaction();
    csSelect();

    _spi->transfer(0x1D); // ReadRegister command
    _spi->transfer((addr >> 8) & 0xFF);
    _spi->transfer(addr & 0xFF);
    _spi->transfer(0x00); // Dummy
    *value = _spi->transfer(0x00);

    csDeselect();
    spiEndTransaction();
    _lastError = SX1280_OK;
    return SX1280_OK;
}

SX1280_Status_t BIG2000_SX1280::writeRegister(uint16_t addr, uint8_t value)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }
    spiBeginTransaction();
    csSelect();

    _spi->transfer(0x0D); // WriteRegister command
    _spi->transfer((addr >> 8) & 0xFF);
    _spi->transfer(addr & 0xFF);
    _spi->transfer(value);

    csDeselect();
    spiEndTransaction();
    _lastError = SX1280_OK;
    return SX1280_OK;
}

SX1280_Status_t BIG2000_SX1280::burstReadRegister(uint16_t addr, uint8_t *buffer, size_t len)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }
    if (buffer == nullptr || len == 0)
    {
        _lastError = SX1280_ERROR_NULL_PTR;
        return _lastError;
    }
    spiBeginTransaction();
    csSelect();

    _spi->transfer(0x1D); // ReadRegister command
    _spi->transfer((addr >> 8) & 0xFF);
    _spi->transfer(addr & 0xFF);
    _spi->transfer(0x00); // Dummy

    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = _spi->transfer(0x00);
    }

    csDeselect();
    spiEndTransaction();
    _lastError = SX1280_OK;
    return SX1280_OK;
}

SX1280_Status_t BIG2000_SX1280::burstWriteRegister(uint16_t addr, const uint8_t *buffer, size_t len)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }
    if (buffer == nullptr || len == 0)
    {
        _lastError = SX1280_ERROR_NULL_PTR;
        return _lastError;
    }
    spiBeginTransaction();
    csSelect();

    _spi->transfer(0x0D); // WriteRegister command
    _spi->transfer((addr >> 8) & 0xFF);
    _spi->transfer(addr & 0xFF);

    for (size_t i = 0; i < len; i++)
    {
        _spi->transfer(buffer[i]);
    }

    csDeselect();
    spiEndTransaction();
    _lastError = SX1280_OK;
    return SX1280_OK;
}

SX1280_Status_t BIG2000_SX1280::getLastError() const
{
    return _lastError;
}

const char *BIG2000_SX1280::statusToString(SX1280_Status_t status) const
{
    switch (status)
    {
    case SX1280_OK:
        return "OK";
    case SX1280_ERROR_NOT_INITIALIZED:
        return "Not initialized";
    case SX1280_ERROR_NULL_PTR:
        return "Null pointer";
    case SX1280_ERROR_SPI:
        return "SPI error";
    case SX1280_ERROR_INVALID_PARAM:
        return "Invalid parameter";
    default:
        return "Unknown error";
    }
}