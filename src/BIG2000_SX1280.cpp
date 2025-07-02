#include "BIG2000_SX1280.h"
#include "SX1280_Definitions.h"

BIG2000_SX1280::BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs)
    : _sck(sck), _miso(miso), _mosi(mosi), _cs(cs), _initialized(false), _spi(nullptr), _lastError(SX1280_OK),
      _rst(-1), _busy(-1), _dio1(-1), _frequency(SX1280_DEFAULT_FREQ), _bandwidth(SX1280_BW_812500),
      _spreadingFactor(SX1280_SF7), _codeRate(SX1280_CR_4_5), _preambleLength(12), _txPower(10)
{
}

BIG2000_SX1280::BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs, int8_t rst, int8_t busy, int8_t dio1)
    : _sck(sck), _miso(miso), _mosi(mosi), _cs(cs), _initialized(false), _spi(nullptr), _lastError(SX1280_OK),
      _rst(rst), _busy(busy), _dio1(dio1), _frequency(SX1280_DEFAULT_FREQ), _bandwidth(SX1280_BW_812500),
      _spreadingFactor(SX1280_SF7), _codeRate(SX1280_CR_4_5), _preambleLength(12), _txPower(10)
{
}

bool BIG2000_SX1280::begin()
{
// Platform uyumlu SPI başlatma
#ifdef ESP32
// ESP32 ve ESP32-S3 için uyumlu SPI başlatma
#ifdef CONFIG_IDF_TARGET_ESP32S3
    // ESP32-S3 için custom SPI
    _spi = new SPIClass(SPI2_HOST);
#elif ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(2, 0, 0)
    // ESP32 Core 2.0+ için HSPI kullan
    _spi = new SPIClass(SPI2_HOST);
#else
    // Eski ESP32 core için VSPI
    _spi = new SPIClass(VSPI);
#endif

    // Custom SPI pinleri ile başlat
    _spi->begin(_sck, _miso, _mosi, _cs);
#else
    // Arduino Uno/Nano için varsayılan SPI
    _spi = &SPI;
    SPI.begin();
#endif

    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);

    // Reset ve diğer pinleri ayarla
    if (_rst >= 0)
    {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(10);
        digitalWrite(_rst, LOW);
        delay(10);
        digitalWrite(_rst, HIGH);
        delay(10);
    }

    if (_busy >= 0)
    {
        pinMode(_busy, INPUT);
    }

    if (_dio1 >= 0)
    {
        pinMode(_dio1, INPUT);
    }

    // Modülün hazır olmasını bekle
    if (_busy >= 0)
    {
        waitOnBusy();
    }

    // Standby moduna geç
    setStandby();

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

// ========== LoRa Konfigürasyon Fonksiyonları ==========

SX1280_Status_t BIG2000_SX1280::setFrequency(uint32_t frequency)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    // Frekans aralığı kontrolü (2.4GHz band)
    if (frequency < 2400000000UL || frequency > 2500000000UL)
    {
        _lastError = SX1280_ERROR_INVALID_PARAM;
        return _lastError;
    }

    _frequency = frequency;
    uint32_t regValue = frequencyToRegValue(frequency);

    uint8_t params[3];
    params[0] = (regValue >> 16) & 0xFF;
    params[1] = (regValue >> 8) & 0xFF;
    params[2] = regValue & 0xFF;

    return executeCommand(0x86, params, 3); // SetRfFrequency
}

SX1280_Status_t BIG2000_SX1280::setBandwidth(uint32_t bandwidth)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    _bandwidth = bandwidth;
    uint8_t bwValue = bandwidthToRegValue(bandwidth);

    // SetModulationParams komutu için parametreler
    uint8_t params[3];
    params[0] = _spreadingFactor;
    params[1] = bwValue;
    params[2] = _codeRate;

    return executeCommand(0x8B, params, 3);
}

SX1280_Status_t BIG2000_SX1280::setSpreadingFactor(uint8_t sf)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    if (sf < 5 || sf > 12)
    {
        _lastError = SX1280_ERROR_INVALID_PARAM;
        return _lastError;
    }

    _spreadingFactor = sf;
    uint8_t bwValue = bandwidthToRegValue(_bandwidth);

    uint8_t params[3];
    params[0] = sf;
    params[1] = bwValue;
    params[2] = _codeRate;

    return executeCommand(0x8B, params, 3);
}

SX1280_Status_t BIG2000_SX1280::setCodeRate(uint8_t cr)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    _codeRate = cr;
    uint8_t bwValue = bandwidthToRegValue(_bandwidth);

    uint8_t params[3];
    params[0] = _spreadingFactor;
    params[1] = bwValue;
    params[2] = cr;

    return executeCommand(0x8B, params, 3);
}

SX1280_Status_t BIG2000_SX1280::setPreambleLength(uint16_t length)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    _preambleLength = length;

    uint8_t params[7];
    params[0] = (length >> 8) & 0xFF;
    params[1] = length & 0xFF;
    params[2] = 0x01; // HeaderType: Variable length
    params[3] = 0xFF; // PayloadLength: Max
    params[4] = 0x01; // CRC on
    params[5] = 0x00; // IQ standard
    params[6] = 0x00; // Reserved

    return executeCommand(0x8C, params, 7);
}

SX1280_Status_t BIG2000_SX1280::setTxPower(int8_t power)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    if (power < SX1280_POWER_MIN || power > SX1280_POWER_MAX)
    {
        _lastError = SX1280_ERROR_INVALID_PARAM;
        return _lastError;
    }

    _txPower = power;

    uint8_t params[2];
    params[0] = power;
    params[1] = 0x00; // RampTime: 20us

    return executeCommand(0x8E, params, 2);
}

// ========== Modül Kontrol Fonksiyonları ==========

SX1280_Status_t BIG2000_SX1280::setSleep()
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    uint8_t param = 0x00; // Sleep config
    return executeCommand(SX1280_CMD_SET_SLEEP, &param, 1);
}

SX1280_Status_t BIG2000_SX1280::setStandby()
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    uint8_t param = SX1280_STANDBY_RC;
    return executeCommand(SX1280_CMD_SET_STANDBY, &param, 1);
}

SX1280_Status_t BIG2000_SX1280::setFrequencySynthesis()
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    return executeCommand(SX1280_CMD_SET_FS);
}

SX1280_Status_t BIG2000_SX1280::setTx(uint32_t timeout)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    uint8_t params[3];
    params[0] = 0x00; // PeriodBase: 15.625 us
    params[1] = (timeout >> 8) & 0xFF;
    params[2] = timeout & 0xFF;

    return executeCommand(SX1280_CMD_SET_TX, params, 3);
}

SX1280_Status_t BIG2000_SX1280::setRx(uint32_t timeout)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    uint8_t params[3];
    params[0] = 0x00; // PeriodBase: 15.625 us
    params[1] = (timeout >> 8) & 0xFF;
    params[2] = timeout & 0xFF;

    return executeCommand(SX1280_CMD_SET_RX, params, 3);
}

// ========== Veri İletim Fonksiyonları ==========

SX1280_Status_t BIG2000_SX1280::transmit(const uint8_t *data, size_t length)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    if (data == nullptr || length == 0 || length > SX1280_FIFO_SIZE)
    {
        _lastError = SX1280_ERROR_INVALID_PARAM;
        return _lastError;
    }

    // Buffer'a veri yaz
    SX1280_Status_t status = writeBuffer(0, data, length);
    if (status != SX1280_OK)
    {
        return status;
    }

    // Packet parametrelerini ayarla
    uint8_t params[7];
    params[0] = (_preambleLength >> 8) & 0xFF;
    params[1] = _preambleLength & 0xFF;
    params[2] = 0x01;   // HeaderType: Variable length
    params[3] = length; // PayloadLength
    params[4] = 0x01;   // CRC on
    params[5] = 0x00;   // IQ standard
    params[6] = 0x00;   // Reserved

    status = executeCommand(0x8C, params, 7);
    if (status != SX1280_OK)
    {
        return status;
    }

    // TX moduna geç
    return setTx(0);
}

SX1280_Status_t BIG2000_SX1280::receive(uint8_t *buffer, size_t maxLength, size_t *receivedLength)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    if (buffer == nullptr || receivedLength == nullptr)
    {
        _lastError = SX1280_ERROR_NULL_PTR;
        return _lastError;
    }

    // Packet durumunu kontrol et
    uint8_t status = getPacketStatus();

    // RxPayloadLength al
    uint8_t lengthInfo[2];
    SX1280_Status_t result = executeCommand(0x13, nullptr, 0); // GetRxBufferStatus
    if (result != SX1280_OK)
    {
        return result;
    }

    // Basit implementasyon - gerçek uygulamada GetRxBufferStatus kullanılmalı
    *receivedLength = (maxLength < SX1280_FIFO_SIZE) ? maxLength : SX1280_FIFO_SIZE;

    return readBuffer(0, buffer, *receivedLength);
}

bool BIG2000_SX1280::isTransmitComplete()
{
    if (!_initialized)
        return false;

    // DIO1 pinini kontrol et (TX_DONE interrupt)
    if (_dio1 >= 0)
    {
        return digitalRead(_dio1) == HIGH;
    }

    // Status register kontrol et
    uint8_t status = getPacketStatus();
    return (status & 0x01) != 0; // TX_DONE bit
}

bool BIG2000_SX1280::isReceiveComplete()
{
    if (!_initialized)
        return false;

    // DIO1 pinini kontrol et (RX_DONE interrupt)
    if (_dio1 >= 0)
    {
        return digitalRead(_dio1) == HIGH;
    }

    // Status register kontrol et
    uint8_t status = getPacketStatus();
    return (status & 0x02) != 0; // RX_DONE bit
}

// ========== Durum Kontrol Fonksiyonları ==========

uint8_t BIG2000_SX1280::getPacketStatus()
{
    if (!_initialized)
        return 0;

    spiBeginTransaction();
    csSelect();

    _spi->transfer(0x14); // GetPacketStatus
    _spi->transfer(0x00); // Dummy
    uint8_t status = _spi->transfer(0x00);

    csDeselect();
    spiEndTransaction();

    return status;
}

int16_t BIG2000_SX1280::getRSSI()
{
    if (!_initialized)
        return -999;

    spiBeginTransaction();
    csSelect();

    _spi->transfer(0x15); // GetRssiInst
    _spi->transfer(0x00); // Dummy
    uint8_t rssi = _spi->transfer(0x00);

    csDeselect();
    spiEndTransaction();

    return -(int16_t)(rssi / 2); // RSSI = -rssi/2 dBm
}

float BIG2000_SX1280::getSNR()
{
    if (!_initialized)
        return -999.0;

    uint8_t status[5];
    spiBeginTransaction();
    csSelect();

    _spi->transfer(0x14); // GetPacketStatus
    for (int i = 0; i < 5; i++)
    {
        status[i] = _spi->transfer(0x00);
    }

    csDeselect();
    spiEndTransaction();

    int8_t snr = (int8_t)status[1];
    return (float)snr * 0.25; // SNR = snr * 0.25 dB
}

uint8_t BIG2000_SX1280::getChipVersion()
{
    uint8_t version = 0;
    readRegister(SX1280_REG_CHIP_VERSION, &version);
    return version;
}

// ========== Buffer İşlemleri ==========

SX1280_Status_t BIG2000_SX1280::writeBuffer(uint8_t offset, const uint8_t *data, size_t length)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    if (data == nullptr || length == 0)
    {
        _lastError = SX1280_ERROR_NULL_PTR;
        return _lastError;
    }

    spiBeginTransaction();
    csSelect();

    _spi->transfer(SX1280_CMD_WRITE_BUFFER);
    _spi->transfer(offset);

    for (size_t i = 0; i < length; i++)
    {
        _spi->transfer(data[i]);
    }

    csDeselect();
    spiEndTransaction();

    _lastError = SX1280_OK;
    return SX1280_OK;
}

SX1280_Status_t BIG2000_SX1280::readBuffer(uint8_t offset, uint8_t *buffer, size_t length)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    if (buffer == nullptr || length == 0)
    {
        _lastError = SX1280_ERROR_NULL_PTR;
        return _lastError;
    }

    spiBeginTransaction();
    csSelect();

    _spi->transfer(SX1280_CMD_READ_BUFFER);
    _spi->transfer(offset);
    _spi->transfer(0x00); // Dummy

    for (size_t i = 0; i < length; i++)
    {
        buffer[i] = _spi->transfer(0x00);
    }

    csDeselect();
    spiEndTransaction();

    _lastError = SX1280_OK;
    return SX1280_OK;
}

// ========== Private Yardımcı Fonksiyonlar ==========

SX1280_Status_t BIG2000_SX1280::executeCommand(uint8_t cmd, const uint8_t *params, size_t paramLength)
{
    if (!_initialized)
    {
        _lastError = SX1280_ERROR_NOT_INITIALIZED;
        return _lastError;
    }

    waitOnBusy();

    spiBeginTransaction();
    csSelect();

    _spi->transfer(cmd);

    if (params != nullptr && paramLength > 0)
    {
        for (size_t i = 0; i < paramLength; i++)
        {
            _spi->transfer(params[i]);
        }
    }

    csDeselect();
    spiEndTransaction();

    waitOnBusy();

    _lastError = SX1280_OK;
    return SX1280_OK;
}

SX1280_Status_t BIG2000_SX1280::waitOnBusy()
{
    if (_busy < 0)
    {
        delay(1); // BUSY pin yoksa kısa gecikme
        return SX1280_OK;
    }

    unsigned long start = millis();
    while (digitalRead(_busy) == HIGH)
    {
        if (millis() - start > 1000)
        { // 1 saniye timeout
            _lastError = SX1280_ERROR_SPI;
            return _lastError;
        }
#ifdef ESP32
        delayMicroseconds(100); // ESP32 için mikrosaniye gecikme
#else
        delay(1); // Diğer platformlar için milisaniye
#endif
    }

    return SX1280_OK;
}

bool BIG2000_SX1280::validateParameters(uint32_t frequency, uint32_t bandwidth, uint8_t sf)
{
    if (frequency < 2400000000UL || frequency > 2500000000UL)
        return false;
    if (sf < 5 || sf > 12)
        return false;

    switch (bandwidth)
    {
    case SX1280_BW_203125:
    case SX1280_BW_406250:
    case SX1280_BW_812500:
    case SX1280_BW_1625000:
        return true;
    default:
        return false;
    }
}

uint32_t BIG2000_SX1280::frequencyToRegValue(uint32_t frequency)
{
    // SX1280 frekans hesaplaması: FreqReg = Freq * 2^16 / Fxtal
    // Fxtal = 52MHz for SX1280
    return (uint32_t)((float)frequency * 65536.0 / 52000000.0);
}

uint8_t BIG2000_SX1280::bandwidthToRegValue(uint32_t bandwidth)
{
    switch (bandwidth)
    {
    case SX1280_BW_203125:
        return 0x18;
    case SX1280_BW_406250:
        return 0x26;
    case SX1280_BW_812500:
        return 0x39;
    case SX1280_BW_1625000:
        return 0x56;
    default:
        return 0x39; // Default to 812.5kHz
    }
}

void BIG2000_SX1280::resetModule()
{
    if (_rst >= 0)
    {
        digitalWrite(_rst, LOW);
        delay(10);
        digitalWrite(_rst, HIGH);
        delay(10);
    }
}