# BIG2000_SX1280 v1.0.0 Release Notes

🎉 **İlk Stabil Sürüm Yayınlandı!**

Bu release SX1280 2.4GHz LoRa modülleri için tam özellikli Arduino kütüphanesini içerir.

## ✨ Ana Özellikler

### 📡 LoRa İletişimi

- ✅ 2.4GHz frekans bandı desteği (2400-2500 MHz)
- ✅ Ayarlanabilir bandwidth (203.125 kHz - 1.625 MHz)
- ✅ Spreading Factor SF5-SF12 desteği
- ✅ Code Rate 4/5, 4/6, 4/7, 4/8 seçenekleri
- ✅ TX Power ayarı (-18 dBm ile +13 dBm)
- ✅ RSSI ve SNR okuması

### 🔧 Kolay Kullanım

- ✅ İki constructor seçeneği (temel/gelişmiş)
- ✅ Kapsamlı hata yönetimi sistemi
- ✅ Non-blocking işlemler
- ✅ Buffer tabanlı veri transfer
- ✅ Örnek kodlar dahil

### 🎯 Platform Desteği

- ✅ ESP32 (test edilmiş)
- ✅ Arduino Uno/Nano uyumlu
- ✅ Arduino IDE Library Manager uyumlu
- ✅ PlatformIO desteği

## 📦 Kurulum

### Arduino IDE (Önerilen)

1. **Sketch** → **Include Library** → **Add .ZIP Library...**
2. İndirdiğiniz ZIP dosyasını seçin
3. Kurulum tamamlandı!

### Manuel Kurulum

```bash
# Arduino libraries klasörüne çıkarın
~/Documents/Arduino/libraries/BIG2000_SX1280/
```

## 🚀 Hızlı Başlangıç

```cpp
#include "BIG2000_SX1280.h"
#include "lilygo-t3s3.h"

BIG2000_SX1280 lora(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

void setup() {
    Serial.begin(115200);

    if (!lora.begin()) {
        Serial.println("LoRa başlatılamadı!");
        return;
    }

    // Temel konfigürasyon
    lora.setFrequency(2400000000UL);  // 2.4 GHz
    lora.setBandwidth(SX1280_BW_812500);
    lora.setSpreadingFactor(SX1280_SF7);

    Serial.println("LoRa hazır!");
}

void loop() {
    String msg = "Merhaba LoRa: " + String(millis());
    lora.transmit((uint8_t*)msg.c_str(), msg.length());
    delay(2000);
}
```

## 📋 Örnekler

### Dahil Edilen Örnekler

1. **basic_test.ino** - Temel gönderme/alma
2. **advanced_communication.ino** - Gelişmiş özellikler, seri komutlar

### Örnek Çalıştırma

- **File** → **Examples** → **BIG2000_SX1280** → **basic_test**

## 🔌 Donanım Gereksinimleri

### Desteklenen Modüller

- SX1280 tabanlı LoRa modüller
- 2.4GHz frekans bandı
- SPI arayüzü

### Pin Bağlantıları (LilyGO T3-S3)

```
SX1280 -> ESP32
SCK    -> GPIO5
MISO   -> GPIO3
MOSI   -> GPIO6
CS     -> GPIO7
RST    -> GPIO8  (opsiyonel)
BUSY   -> GPIO36 (opsiyonel)
DIO1   -> GPIO9  (opsiyonel)
```

## 📊 Performans

- **SPI Hızı**: 8 MHz
- **Max Payload**: 256 bytes
- **Range**: ~1-5km (açık alan, optimize parametreler)
- **Memory Usage**: ~2KB RAM

## 🐛 Bilinen Sorunlar

- Henüz bilinen kritik sorun yok
- Test feedback'leri beklenmekte

## 🔄 Değişiklik Geçmişi

### [1.0.0] - 2025-07-02

#### Eklenen

- İlk stabil release
- Tam SX1280 API implementasyonu
- ESP32 ve Arduino uyumluluğu
- Kapsamlı örnek kodlar
- Arduino Library Manager uyumluluğu
- CI/CD pipeline (GitHub Actions)
- Profesyonel dokümantasyon

## 🤝 Katkıda Bulunma

- **Issues**: [GitHub Issues](https://github.com/kayadev/BIG2000-SX1280/issues)
- **Pull Requests**: Hoş geldin!
- **Dokümantasyon**: İyileştirmeler memnuniyetle karşılanır

## 📄 Lisans

MIT License - Detaylar için [LICENSE](LICENSE) dosyasına bakın.

## 🙏 Teşekkürler

- Semtech SX1280 datasheet
- Arduino ve ESP32 topluluğu
- Test eden tüm kullanıcılar

---

**Bu sürümü test edin ve feedback gönderin!** 🚀

**Download**: [BIG2000-SX1280-v1.0.0.zip](../../archive/refs/tags/v1.0.0.zip)
