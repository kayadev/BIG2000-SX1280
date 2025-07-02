# BIG2000_SX1280 Arduino Library Extras

Bu klasör Arduino Library Manager tarafından kullanılan ek dosyaları içerir.

## İçerik

- Test kodları
- Schematic dosyaları
- Ek dokümantasyon
- Platform spesifik notlar

## SX1280 Pin Bağlantıları

### ESP32 (LilyGO T3-S3)

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

### Arduino Uno/Nano

```
SX1280  -> Arduino
VCC     -> 3.3V
GND     -> GND
SCK     -> 13
MISO    -> 12
MOSI    -> 11
CS      -> 10
RST     -> 9
BUSY    -> 8
DIO1    -> 2 (Interrupt pin)
```

## Gereksinimler

- Arduino IDE 1.8.19+
- ESP32 Core 2.0.0+
- SPI Library (Arduino Core ile birlikte gelir)

## Test Edilmiş Platformlar

- ✅ ESP32-S3 (LilyGO T3-S3)
- ✅ ESP32 DevKit
- ⚠️ Arduino Uno (test edilmedi)
- ⚠️ Arduino Nano (test edilmedi)

## Performans Notları

- SPI Clock: 8MHz (varsayılan)
- Maximum payload: 256 bytes
- Range: ~1-5km (açık alan, optimize edilmiş parametrelerle)
