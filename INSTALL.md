# BIG2000_SX1280 - Arduino IDE Kurulum Rehberi

## 🚀 Hızlı Kurulum (GitHub ZIP)

### Adım 1: GitHub'dan İndirin

1. https://github.com/kayadev/BIG2000-SX1280 adresine gidin
2. Yeşil **"Code"** butonuna tıklayın
3. **"Download ZIP"** seçeneğini seçin
4. `BIG2000-SX1280-main.zip` dosyasını indirin

### Adım 2: Arduino IDE'ye Yükleyin

1. Arduino IDE'yi açın
2. **Sketch** → **Include Library** → **Add .ZIP Library...** menüsüne gidin
3. İndirdiğiniz `BIG2000-SX1280-main.zip` dosyasını seçin
4. **"Open"** butonuna tıklayın
5. Arduino IDE otomatik olarak kütüphaneyi kuracak

### Adım 3: Kurulumu Doğrulayın

1. **Sketch** → **Include Library** menüsünde **"BIG2000_SX1280"** görünmeli
2. **File** → **Examples** → **BIG2000_SX1280** altında örnek kodları görebilmelisiniz

## 🔧 Manuel Kurulum (Alternatif)

### Adım 1: Klasör Kurulumu

1. ZIP dosyasını çıkarın
2. `BIG2000-SX1280-main` klasörünü `BIG2000_SX1280` olarak yeniden adlandırın
3. Bu klasörü Arduino libraries dizinine kopyalayın:
   - **Windows**: `C:\Users\{username}\Documents\Arduino\libraries\`
   - **macOS**: `/Users/{username}/Documents/Arduino/libraries/`
   - **Linux**: `/home/{username}/Arduino/libraries/`

### Adım 2: Arduino IDE'yi Yeniden Başlatın

Arduino IDE'yi kapatıp tekrar açın.

## 📝 İlk Kullanım

### Temel Örnek Çalıştırma

1. **File** → **Examples** → **BIG2000_SX1280** → **basic_test**
2. Board'ınızı seçin: **Tools** → **Board** → **ESP32 Dev Module**
3. Port'unuzu seçin: **Tools** → **Port**
4. **Upload** butonuna tıklayın

### Pin Bağlantıları (LilyGO T3-S3)

```
SX1280  -> ESP32
VCC     -> 3.3V
GND     -> GND
SCK     -> GPIO5
MISO    -> GPIO3
MOSI    -> GPIO6
CS      -> GPIO7
RST     -> GPIO8
BUSY    -> GPIO36
DIO1    -> GPIO9
```

## 🎯 Örnek Kod

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

    // Parametreleri ayarla
    lora.setFrequency(2400000000UL);
    lora.setBandwidth(SX1280_BW_812500);
    lora.setSpreadingFactor(SX1280_SF7);

    Serial.println("LoRa hazır!");
}

void loop() {
    String message = "Test: " + String(millis());
    lora.transmit((uint8_t*)message.c_str(), message.length());
    delay(2000);
}
```

## ❗ Önemli Notlar

1. **ESP32 Core** gereklidir: Arduino IDE'de ESP32 desteği olmalı
2. **SPI Library** otomatik olarak dahil edilir
3. **Pin tanımları** board'unuza göre değiştirilebilir
4. **2.4GHz LoRa** için tasarlanmıştır (915MHz değil!)

## 🆘 Sorun Giderme

### "Library not found" Hatası

- Kütüphane klasör adının `BIG2000_SX1280` olduğundan emin olun
- Arduino IDE'yi yeniden başlatın
- Libraries klasörünün doğru konumda olduğunu kontrol edin

### Compilation Hatası

- ESP32 board package'ının yüklü olduğundan emin olun
- **Tools** → **Board** → **Boards Manager** → "ESP32" arayın

### Upload Hatası

- Doğru COM port'u seçtiğinizden emin olun
- Board'unuzun USB bağlantısını kontrol edin
- Gerekirse board'u reset edin

## 📞 Destek

- **GitHub Issues**: https://github.com/kayadev/BIG2000-SX1280/issues
- **Wiki**: https://github.com/kayadev/BIG2000-SX1280/wiki
- **Email**: mustafa@example.com

---

**Başarılı LoRa projeler dileriz!** 🚀📡
