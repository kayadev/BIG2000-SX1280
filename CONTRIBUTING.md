# Katkıda Bulunma Rehberi

BIG2000_SX1280 kütüphanesine katkıda bulunduğunuz için teşekkürler! 🎉

## 🤝 Nasıl Katkıda Bulunabilirsiniz

### Hata Raporlama

1. Mevcut [Issues](https://github.com/kayadev/BIG2000-SX1280/issues) sayfasını kontrol edin
2. Hata zaten rapor edilmediyse yeni bir Issue açın
3. Şu bilgileri dahil edin:
   - Arduino IDE versiyonu
   - Board tipi (ESP32, Arduino Uno, vb.)
   - Kütüphane versiyonu
   - Hatayı yeniden oluşturmak için gerekli kod
   - Beklenen davranış vs gerçek davranış

### Özellik Önerisi

1. Yeni özellik için Issue açın
2. Özelliğin neden gerekli olduğunu açıklayın
3. Mümkünse implementasyon önerilerinizi paylaşın

### Kod Katkısı

#### Kurulum

```bash
git clone https://github.com/kayadev/BIG2000-SX1280.git
cd BIG2000-SX1280
```

#### Geliştirme Ortamı

- Arduino IDE 1.8.19+ veya PlatformIO
- ESP32 development board (test için)
- SX1280 modül

#### Pull Request Süreci

1. Bu repoyu fork edin
2. Feature branch oluşturun:
   ```bash
   git checkout -b feature/yeni-ozellik
   ```
3. Değişikliklerinizi yapın
4. Test edin
5. Commit edin:
   ```bash
   git commit -m "feat: yeni özellik eklendi"
   ```
6. Branch'ınızı push edin:
   ```bash
   git push origin feature/yeni-ozellik
   ```
7. Pull Request açın

## 📋 Kodlama Standartları

### C++ Kodlama Kuralları

- **Indentation**: 4 boşluk (tab değil)
- **Naming Convention**:
  - Fonksiyonlar: `camelCase`
  - Değişkenler: `camelCase`
  - Private üyeler: `_underscorePrefix`
  - Sabitler: `UPPER_CASE`
  - Sınıflar: `PascalCase`

### Örnek Kod Formatı

```cpp
class BIG2000_SX1280
{
public:
    SX1280_Status_t setFrequency(uint32_t frequency);

private:
    uint8_t _sck;
    bool _initialized;
    static const uint8_t MAX_RETRIES = 3;
};
```

### Dokümantasyon

- Tüm public fonksiyonlar comment edilmeli
- README.md güncel tutulmalı
- Örnek kodlar eklenebilir

## 🧪 Test Etme

### Manuel Test

- Tüm örnek kodları çalıştırın
- Farklı LoRa parametreleriyle test edin
- Farklı ESP32 boardlarda test edin

### Test Checklist

- [ ] Compilation hatası yok
- [ ] Temel send/receive çalışıyor
- [ ] Error handling doğru çalışıyor
- [ ] Memory leak yok
- [ ] Örnek kodlar güncel

## 📝 Commit Message Formatı

Conventional Commits kullanıyoruz:

```
<type>(<scope>): <description>

<body>

<footer>
```

### Types

- `feat`: Yeni özellik
- `fix`: Hata düzeltmesi
- `docs`: Dokümantasyon değişikliği
- `style`: Kod formatı (mantık değişikliği yok)
- `refactor`: Code refactoring
- `test`: Test ekleme/düzeltme
- `chore`: Build, CI, vb.

### Örnekler

```
feat(lora): add ranging support

docs: update README with new examples

fix(spi): resolve timeout issue on ESP32
```

## 🏷️ Versiyonlama

[Semantic Versioning](https://semver.org/) kullanıyoruz:

- `MAJOR.MINOR.PATCH`
- MAJOR: Breaking changes
- MINOR: Yeni özellikler (backward compatible)
- PATCH: Bug fixes

## 📞 İletişim

- **Issues**: GitHub Issues
- **Discussions**: GitHub Discussions
- **Email**: mustafa@example.com

## 📄 Lisans

Bu projeye katkıda bulunarak, katkılarınızın MIT lisansı altında lisanslanacağını kabul ediyorsunuz.

---

**Teşekkürler!** 🙏
Katkılarınız open source topluluğunu güçlendiriyor!
