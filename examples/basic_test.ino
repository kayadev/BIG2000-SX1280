#include "BIG2000_SX1280.h"
#include "lilygo-t3s3.h"

// BIG2000_SX1280 modül örneği
BIG2000_SX1280 lora(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS, PIN_RST, PIN_BUSY, PIN_DIO1);

void setup()
{
    Serial.begin(115200);
    Serial.println("BIG2000 SX1280 Test Başlatılıyor...");

    // LoRa modülünü başlat
    if (!lora.begin())
    {
        Serial.println("LoRa modülü başlatılamadı!");
        while (1)
            ;
    }

    Serial.println("LoRa modülü başarıyla başlatıldı!");

    // Temel yapılandırma
    lora.setFrequency(2400000000UL);     // 2.4 GHz
    lora.setBandwidth(SX1280_BW_812500); // 812.5 kHz
    lora.setSpreadingFactor(SX1280_SF7); // SF7
    lora.setCodeRate(SX1280_CR_4_5);     // 4/5
    lora.setPreambleLength(12);          // 12 sembol
    lora.setTxPower(10);                 // 10 dBm

    Serial.println("LoRa parametreleri ayarlandı");

    // Chip versiyonunu oku
    uint8_t version = lora.getChipVersion();
    Serial.printf("Chip Version: 0x%02X\n", version);
}

void loop()
{
    // Veri gönderme örneği
    String message = "Merhaba LoRa! Zaman: " + String(millis());

    Serial.println("Veri gönderiliyor: " + message);

    SX1280_Status_t status = lora.transmit((uint8_t *)message.c_str(), message.length());

    if (status == SX1280_OK)
    {
        Serial.println("Veri başarıyla gönderildi!");
    }
    else
    {
        Serial.println("Veri gönderme hatası: " + String(lora.statusToString(status)));
    }

    // Veri alma kontrolü (non-blocking)
    uint8_t rxBuffer[256];
    size_t receivedLength;

    lora.setRx(5000); // 5 saniye alma modu

    if (lora.isReceiveComplete())
    {
        status = lora.receive(rxBuffer, sizeof(rxBuffer), &receivedLength);

        if (status == SX1280_OK && receivedLength > 0)
        {
            Serial.println("Veri alındı:");
            Serial.printf("RSSI: %d dBm\n", lora.getRSSI());
            Serial.printf("SNR: %.2f dB\n", lora.getSNR());
            Serial.printf("Veri: ");

            for (size_t i = 0; i < receivedLength; i++)
            {
                Serial.print((char)rxBuffer[i]);
            }
            Serial.println();
        }
    }

    delay(2000);
}
