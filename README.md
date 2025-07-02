# BIG2000 SX1280 LoRa Kütüphanesi

ESP32 tabanlı sistemler için geliştirilmiş SX1280 LoRa modül kütüphanesi.

## 🚀 Özellikler

- ✅ SX1280 2.4GHz LoRa modül desteği
- ✅ Kolay kullanılabilir API
- ✅ Kapsamlı hata yönetimi
- ✅ Burst read/write işlemleri
- ✅ Çoklu constructor desteği
- ✅ LilyGO T3-S3 uyumlu pin tanımları
- ✅ Örnek kodlar

## 📦 Kurulum

1. Bu kütüphaneyi Arduino IDE'nin `libraries` klasörüne kopyalayın
2. Arduino IDE'yi yeniden başlatın
3. `#include "BIG2000_SX1280.h"` ile kullanmaya başlayın

## 🔧 Temel Kullanım

```cpp
#include "BIG2000_SX1280.h"
#include "lilygo-t3s3.h"

// Temel kullanım
BIG2000_SX1280 lora(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

// Gelişmiş kullanım (tüm pinler)
BIG2000_SX1280 lora(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS, PIN_RST, PIN_BUSY, PIN_DIO1);

void setup() {
    Serial.begin(115200);

    // Modülü başlat
    if (!lora.begin()) {
        Serial.println("LoRa modülü başlatılamadı!");
        return;
    }

    // Parametreleri ayarla
    lora.setFrequency(2400000000UL);      // 2.4 GHz
    lora.setBandwidth(SX1280_BW_812500);  // 812.5 kHz
    lora.setSpreadingFactor(SX1280_SF7);  // SF7
    lora.setTxPower(10);                  // 10 dBm
}

void loop() {
    // Veri gönder
    String msg = "Merhaba LoRa!";
    lora.transmit((uint8_t*)msg.c_str(), msg.length());

    delay(1000);
}
```

## 📋 API Referansı

### Başlatma

```cpp
bool begin();                    // Modülü başlat
bool isInitialized();           // Başlatma durumunu kontrol et
```

### Yapılandırma

```cpp
SX1280_Status_t setFrequency(uint32_t frequency);        // Frekans ayarla
SX1280_Status_t setBandwidth(uint32_t bandwidth);        // Bant genişliği
SX1280_Status_t setSpreadingFactor(uint8_t sf);         // Spreading factor
SX1280_Status_t setCodeRate(uint8_t cr);                // Code rate
SX1280_Status_t setPreambleLength(uint16_t length);     // Preamble uzunluğu
SX1280_Status_t setTxPower(int8_t power);               // Güç seviyesi
```

### Veri İletimi

```cpp
SX1280_Status_t transmit(const uint8_t *data, size_t length);
SX1280_Status_t receive(uint8_t *buffer, size_t maxLength, size_t *receivedLength);
bool isTransmitComplete();
bool isReceiveComplete();
```

### Durum Kontrolü

```cpp
int16_t getRSSI();              // RSSI değeri
float getSNR();                 // SNR değeri
uint8_t getChipVersion();       // Chip versiyonu
SX1280_Status_t getLastError(); // Son hata
```

## 🎯 Desteklenen Parametreler

### Frekans

- 2.4 GHz ISM bandı (2400-2500 MHz)

### Bandwidth

- 203.125 kHz (`SX1280_BW_203125`)
- 406.250 kHz (`SX1280_BW_406250`)
- 812.500 kHz (`SX1280_BW_812500`)
- 1.625 MHz (`SX1280_BW_1625000`)

### Spreading Factor

- SF5 - SF12 (`SX1280_SF5` - `SX1280_SF12`)

### Code Rate

- 4/5 (`SX1280_CR_4_5`)
- 4/6 (`SX1280_CR_4_6`)
- 4/7 (`SX1280_CR_4_7`)
- 4/8 (`SX1280_CR_4_8`)

### Güç Seviyeleri

- -18 dBm ile +13 dBm arası

## 🔗 Pin Bağlantıları (LilyGO T3-S3)

| SX1280 | ESP32  | Pin |
| ------ | ------ | --- |
| SCK    | GPIO5  | 5   |
| MISO   | GPIO3  | 3   |
| MOSI   | GPIO6  | 6   |
| CS     | GPIO7  | 7   |
| RST    | GPIO8  | 8   |
| BUSY   | GPIO36 | 36  |
| DIO1   | GPIO9  | 9   |

## 📁 Dosya Yapısı

```
src/
├── BIG2000_SX1280.h          # Ana header dosyası
├── BIG2000_SX1280.cpp        # Implementation
├── SX1280_Definitions.h      # Register ve sabit tanımları
└── lilygo-t3s3.h            # Pin tanımları

examples/
└── basic_test.ino            # Temel kullanım örneği
```

## 🔍 Hata Kodları

```cpp
SX1280_OK                     // Başarılı
SX1280_ERROR_NOT_INITIALIZED  // Modül başlatılmamış
SX1280_ERROR_NULL_PTR         // Null pointer
SX1280_ERROR_SPI              // SPI hatası
SX1280_ERROR_INVALID_PARAM    // Geçersiz parametre
SX1280_ERROR_UNKNOWN          // Bilinmeyen hata
```

## 🤝 Katkıda Bulunma

1. Bu repoyu fork edin
2. Yeni bir branch oluşturun (`git checkout -b feature/yeni-ozellik`)
3. Değişikliklerinizi commit edin (`git commit -am 'Yeni özellik eklendi'`)
4. Branch'ınızı push edin (`git push origin feature/yeni-ozellik`)
5. Pull Request oluşturun

## 📄 Lisans

Bu proje MIT lisansı altında lisanslanmıştır.

## 📞 İletişim

Sorularınız için issue açabilir veya e-posta gönderebilirsiniz.

---

**BIG2000 SX1280** - Güvenilir LoRa iletişimi için! 🚀
