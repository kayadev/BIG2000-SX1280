# Changelog

Bu dosya BIG2000_SX1280 kütüphanesinin tüm önemli değişikliklerini belgelemektedir.

Biçim [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) temel alınmıştır.

## [1.0.0] - 2025-07-02

### Eklenen

- İlk sürüm yayınlandı
- SX1280 2.4GHz LoRa modül desteği
- ESP32 ve Arduino uyumluluğu
- Tam LoRa API implementasyonu:
  - Frekans, bandwidth, spreading factor ayarları
  - TX/RX modları
  - Veri gönderme/alma fonksiyonları
  - RSSI/SNR okuma
  - Buffer işlemleri
- Kapsamlı hata yönetimi sistemi
- İki constructor seçeneği (temel ve gelişmiş)
- Arduino Library Manager uyumluluğu
- Örnek kodlar:
  - `basic_test.ino` - Temel kullanım
  - `advanced_communication.ino` - Gelişmiş özellikler
- Dokümantasyon ve README
- Keywords.txt Arduino IDE syntax highlighting için
- LilyGO T3-S3 pin tanımları

### Güvenlik

- SPI timeout koruması
- Parametre doğrulama
- Null pointer kontrolleri
- Frekans aralığı kontrolleri

### Teknik Detaylar

- 712 satır C++ kodu
- Tam Arduino platformu uyumluluğu
- Memory efficient implementation
- Non-blocking işlemler için destek

## [Planned] - Gelecek Sürümler

### Planlanmış Özellikler

- Interrupt tabanlı işlemler
- Ranging (mesafe ölçümü) desteği
- FLRC ve BLE modülasyon desteği
- Daha fazla board desteği
- Power management iyileştirmeleri
- OTA (Over-The-Air) güncelleme desteği
