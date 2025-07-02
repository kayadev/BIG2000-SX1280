#include "BIG2000_SX1280.h"
#include "lilygo-t3s3.h"

// BIG2000_SX1280 modül örneği (tüm pinlerle)
BIG2000_SX1280 lora(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS, PIN_RST, PIN_BUSY, PIN_DIO1);

// İletişim parametreleri
struct LoRaConfig
{
    uint32_t frequency = 2450000000UL;     // 2.45 GHz
    uint32_t bandwidth = SX1280_BW_812500; // 812.5 kHz
    uint8_t spreadingFactor = SX1280_SF9;  // SF9 (daha uzun menzil)
    uint8_t codeRate = SX1280_CR_4_5;      // 4/5 (hızlı)
    uint16_t preambleLength = 16;          // 16 sembol
    int8_t txPower = 13;                   // Maksimum güç
};

LoRaConfig config;
unsigned long lastTransmit = 0;
unsigned long lastReceiveCheck = 0;
uint32_t messageCounter = 0;
bool isReceiving = false;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("=== BIG2000 SX1280 Gelişmiş Test ===");
    Serial.println("Başlatılıyor...");

    // LED pinini ayarla
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // LoRa modülünü başlat
    if (!lora.begin())
    {
        Serial.println("❌ LoRa modülü başlatılamadı!");
        blinkError();
    }

    Serial.println("✅ LoRa modülü başarıyla başlatıldı!");
    digitalWrite(PIN_LED, HIGH);
    delay(500);
    digitalWrite(PIN_LED, LOW);

    // Chip versiyonunu kontrol et
    uint8_t version = lora.getChipVersion();
    Serial.printf("📡 Chip Version: 0x%02X\n", version);

    // Parametreleri yapılandır
    setupLoRaParameters();

    // İlk alma moduna geç
    startReceiveMode();

    Serial.println("🚀 Sistem hazır!");
    Serial.println("===========================");
    printHelp();
}

void loop()
{
    // Seri port komutlarını kontrol et
    handleSerialCommands();

    // Otomatik mesaj gönderimi (10 saniyede bir)
    if (millis() - lastTransmit > 10000)
    {
        sendTestMessage();
        lastTransmit = millis();
    }

    // Alma kontrolü (500ms'de bir)
    if (millis() - lastReceiveCheck > 500)
    {
        checkForReceive();
        lastReceiveCheck = millis();
    }

    delay(100);
}

void setupLoRaParameters()
{
    Serial.println("⚙️  LoRa parametreleri ayarlanıyor...");

    // Frekans ayarla
    if (lora.setFrequency(config.frequency) == SX1280_OK)
    {
        Serial.printf("  ✅ Frekans: %.1f MHz\n", config.frequency / 1000000.0);
    }
    else
    {
        Serial.println("  ❌ Frekans ayarlanamadı!");
    }

    // Bandwidth ayarla
    if (lora.setBandwidth(config.bandwidth) == SX1280_OK)
    {
        Serial.printf("  ✅ Bandwidth: %.1f kHz\n", config.bandwidth / 1000.0);
    }
    else
    {
        Serial.println("  ❌ Bandwidth ayarlanamadı!");
    }

    // Spreading Factor ayarla
    if (lora.setSpreadingFactor(config.spreadingFactor) == SX1280_OK)
    {
        Serial.printf("  ✅ Spreading Factor: SF%d\n", config.spreadingFactor);
    }
    else
    {
        Serial.println("  ❌ Spreading Factor ayarlanamadı!");
    }

    // Code Rate ayarla
    if (lora.setCodeRate(config.codeRate) == SX1280_OK)
    {
        Serial.println("  ✅ Code Rate: 4/5");
    }
    else
    {
        Serial.println("  ❌ Code Rate ayarlanamadı!");
    }

    // Preamble Length ayarla
    if (lora.setPreambleLength(config.preambleLength) == SX1280_OK)
    {
        Serial.printf("  ✅ Preamble: %d sembol\n", config.preambleLength);
    }
    else
    {
        Serial.println("  ❌ Preamble Length ayarlanamadı!");
    }

    // TX Power ayarla
    if (lora.setTxPower(config.txPower) == SX1280_OK)
    {
        Serial.printf("  ✅ TX Power: %d dBm\n", config.txPower);
    }
    else
    {
        Serial.println("  ❌ TX Power ayarlanamadı!");
    }

    Serial.println("⚙️  Parametreler ayarlandı!");
}

void sendTestMessage()
{
    // LED'i yak
    digitalWrite(PIN_LED, HIGH);

    // Mesaj oluştur
    String message = String("BIG2000-Test-") + String(messageCounter++) +
                     String("-Time:") + String(millis()) +
                     String("-Uptime:") + String(millis() / 1000) + String("s");

    Serial.println("📤 Veri gönderiliyor: " + message);

    SX1280_Status_t status = lora.transmit((uint8_t *)message.c_str(), message.length());

    if (status == SX1280_OK)
    {
        // TX tamamlanmasını bekle
        uint32_t startTime = millis();
        while (!lora.isTransmitComplete() && (millis() - startTime) < 5000)
        {
            delay(10);
        }

        if (lora.isTransmitComplete())
        {
            Serial.println("  ✅ Veri başarıyla gönderildi!");
        }
        else
        {
            Serial.println("  ⚠️  TX timeout!");
        }
    }
    else
    {
        Serial.println("  ❌ Gönderme hatası: " + String(lora.statusToString(status)));
    }

    // LED'i söndür
    digitalWrite(PIN_LED, LOW);

    // Tekrar alma moduna geç
    startReceiveMode();
}

void startReceiveMode()
{
    if (lora.setRx(0) == SX1280_OK)
    { // Sürekli alma modu
        isReceiving = true;
        Serial.println("📥 Alma modu aktif");
    }
    else
    {
        Serial.println("❌ Alma modu başlatılamadı!");
        isReceiving = false;
    }
}

void checkForReceive()
{
    if (!isReceiving)
        return;

    if (lora.isReceiveComplete())
    {
        uint8_t rxBuffer[256];
        size_t receivedLength;

        SX1280_Status_t status = lora.receive(rxBuffer, sizeof(rxBuffer), &receivedLength);

        if (status == SX1280_OK && receivedLength > 0)
        {
            // LED'i yanıp söndür
            digitalWrite(PIN_LED, HIGH);
            delay(100);
            digitalWrite(PIN_LED, LOW);

            Serial.println("📨 Veri alındı!");
            Serial.printf("  📊 RSSI: %d dBm\n", lora.getRSSI());
            Serial.printf("  📊 SNR: %.2f dB\n", lora.getSNR());
            Serial.printf("  📏 Uzunluk: %d byte\n", receivedLength);
            Serial.print("  📝 Veri: ");

            // Veriyi yazdır
            for (size_t i = 0; i < receivedLength; i++)
            {
                if (rxBuffer[i] >= 32 && rxBuffer[i] <= 126)
                {
                    Serial.print((char)rxBuffer[i]);
                }
                else
                {
                    Serial.printf("[0x%02X]", rxBuffer[i]);
                }
            }
            Serial.println();
            Serial.println("  ────────────────────");
        }
        else
        {
            Serial.println("❌ Alma hatası: " + String(lora.statusToString(status)));
        }

        // Tekrar alma moduna geç
        startReceiveMode();
    }
}

void handleSerialCommands()
{
    if (Serial.available())
    {
        String command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase();

        if (command == "help" || command == "h")
        {
            printHelp();
        }
        else if (command == "send" || command == "s")
        {
            sendTestMessage();
        }
        else if (command == "status" || command == "st")
        {
            printStatus();
        }
        else if (command == "reset" || command == "r")
        {
            resetSystem();
        }
        else if (command.startsWith("power "))
        {
            int power = command.substring(6).toInt();
            setPower(power);
        }
        else if (command.startsWith("freq "))
        {
            uint32_t freq = command.substring(5).toInt();
            setFrequency(freq);
        }
        else
        {
            Serial.println("❓ Bilinmeyen komut. 'help' yazın.");
        }
    }
}

void printHelp()
{
    Serial.println("\n📋 Mevcut Komutlar:");
    Serial.println("  help/h     - Bu yardım menüsü");
    Serial.println("  send/s     - Test mesajı gönder");
    Serial.println("  status/st  - Sistem durumu");
    Serial.println("  reset/r    - Sistemi resetle");
    Serial.println("  power <n>  - TX gücünü ayarla (-18 to 13)");
    Serial.println("  freq <n>   - Frekans ayarla (MHz)");
    Serial.println();
}

void printStatus()
{
    Serial.println("\n📊 Sistem Durumu:");
    Serial.printf("  🔧 Başlatıldı: %s\n", lora.isInitialized() ? "Evet" : "Hayır");
    Serial.printf("  📡 Chip Version: 0x%02X\n", lora.getChipVersion());
    Serial.printf("  📻 Frekans: %.1f MHz\n", config.frequency / 1000000.0);
    Serial.printf("  📏 Bandwidth: %.1f kHz\n", config.bandwidth / 1000.0);
    Serial.printf("  📊 Spreading Factor: SF%d\n", config.spreadingFactor);
    Serial.printf("  🔋 TX Power: %d dBm\n", config.txPower);
    Serial.printf("  📨 Gönderilen Mesaj: %d\n", messageCounter);
    Serial.printf("  ⏱️  Çalışma Süresi: %lu saniye\n", millis() / 1000);
    Serial.printf("  🔄 Alma Modu: %s\n", isReceiving ? "Aktif" : "Pasif");
    Serial.println();
}

void resetSystem()
{
    Serial.println("🔄 Sistem resetleniyor...");
    messageCounter = 0;
    ESP.restart();
}

void setPower(int power)
{
    if (power < -18 || power > 13)
    {
        Serial.println("❌ Güç değeri -18 ile 13 arasında olmalı!");
        return;
    }

    config.txPower = power;
    if (lora.setTxPower(power) == SX1280_OK)
    {
        Serial.printf("✅ TX Power %d dBm olarak ayarlandı\n", power);
    }
    else
    {
        Serial.println("❌ TX Power ayarlanamadı!");
    }
}

void setFrequency(uint32_t freqMHz)
{
    uint32_t frequency = freqMHz * 1000000UL;

    if (frequency < 2400000000UL || frequency > 2500000000UL)
    {
        Serial.println("❌ Frekans 2400-2500 MHz arasında olmalı!");
        return;
    }

    config.frequency = frequency;
    if (lora.setFrequency(frequency) == SX1280_OK)
    {
        Serial.printf("✅ Frekans %.1f MHz olarak ayarlandı\n", frequency / 1000000.0);
        startReceiveMode(); // Alma modunu yeniden başlat
    }
    else
    {
        Serial.println("❌ Frekans ayarlanamadı!");
    }
}

void blinkError()
{
    while (true)
    {
        digitalWrite(PIN_LED, HIGH);
        delay(200);
        digitalWrite(PIN_LED, LOW);
        delay(200);
    }
}
