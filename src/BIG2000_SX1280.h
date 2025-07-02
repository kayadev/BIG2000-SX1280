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
    // Temel constructor (sadece SPI pinleri)
    BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs);

    // Gelişmiş constructor (tüm pinler dahil)
    BIG2000_SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs,
                   int8_t rst = -1, int8_t busy = -1, int8_t dio1 = -1);

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

    // LoRa Konfigürasyon
    SX1280_Status_t setFrequency(uint32_t frequency);
    SX1280_Status_t setBandwidth(uint32_t bandwidth);
    SX1280_Status_t setSpreadingFactor(uint8_t sf);
    SX1280_Status_t setCodeRate(uint8_t cr);
    SX1280_Status_t setPreambleLength(uint16_t length);
    SX1280_Status_t setTxPower(int8_t power);

    // Modul Kontrol
    SX1280_Status_t setSleep();
    SX1280_Status_t setStandby();
    SX1280_Status_t setFrequencySynthesis();
    SX1280_Status_t setTx(uint32_t timeout = 0);
    SX1280_Status_t setRx(uint32_t timeout = 0);

    // Veri İletimi
    SX1280_Status_t transmit(const uint8_t *data, size_t length);
    SX1280_Status_t receive(uint8_t *buffer, size_t maxLength, size_t *receivedLength);
    bool isTransmitComplete();
    bool isReceiveComplete();

    // Durum Kontrol
    uint8_t getPacketStatus();
    int16_t getRSSI();
    float getSNR();
    uint8_t getChipVersion();

    // Buffer İşlemleri
    SX1280_Status_t writeBuffer(uint8_t offset, const uint8_t *data, size_t length);
    SX1280_Status_t readBuffer(uint8_t offset, uint8_t *buffer, size_t length);

private:
    uint8_t _sck, _miso, _mosi, _cs;
    bool _initialized;
    SPIClass *_spi;
    SX1280_Status_t _lastError;

    // Pin tanımları (opsiyonel)
    int8_t _rst;
    int8_t _busy;
    int8_t _dio1;

    // LoRa Parametreleri
    uint32_t _frequency;
    uint32_t _bandwidth;
    uint8_t _spreadingFactor;
    uint8_t _codeRate;
    uint16_t _preambleLength;
    int8_t _txPower;

    // SPI İşlemleri
    void spiBeginTransaction();
    void spiEndTransaction();
    void csSelect();
    void csDeselect();

    // Alt seviye komutlar
    SX1280_Status_t executeCommand(uint8_t cmd, const uint8_t *params = nullptr, size_t paramLength = 0);
    SX1280_Status_t waitOnBusy();
    bool validateParameters(uint32_t frequency, uint32_t bandwidth, uint8_t sf);

    // Yardımcı fonksiyonlar
    uint32_t frequencyToRegValue(uint32_t frequency);
    uint8_t bandwidthToRegValue(uint32_t bandwidth);
    void resetModule();
};

#endif // BIG2000_SX1280_H