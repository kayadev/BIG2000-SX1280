# 🚀 BIG2000_SX1280 Kısa Yol Kullanım Rehberi

## 📋 Kısa Yol Scripti

Artık kütüphanenizi Arduino IDE'ye yüklemek için tek bir komut yeterli!

### 🖥️ macOS/Linux

```bash
./install_library.sh
```

### 🖥️ Windows

```cmd
install_library.bat
```

## ⚡ Ne Yapar?

### 🔄 **Otomatik İşlemler:**

1. ✅ Mevcut kütüphaneyi siler (varsa)
2. ✅ Yeni kütüphaneyi Arduino libraries klasörüne kopyalar
3. ✅ Examples'ları doğru formata çevirir
4. ✅ Tüm gerekli dosyaları kontrol eder
5. ✅ Arduino IDE'yi yeniden başlatır (opsiyonel)

### 📁 **Kopyalanan Dosyalar:**

- `src/` → Header ve implementation dosyaları
- `examples/` → Arduino IDE örnekleri (klasör formatında)
- `library.properties` → Kütüphane tanımı
- `keywords.txt` → Syntax highlighting
- `LICENSE` → Lisans dosyası
- `README.md` → Dokümantasyon
- `extras/` → Ek dokümantasyon

## 🎯 **Kullanım Senaryoları**

### 💻 **VS Code'da Geliştirme:**

1. **Command Palette ile:**

   - `Cmd+Shift+P` (macOS) veya `Ctrl+Shift+P` (Windows/Linux)
   - "Tasks: Run Task" yazın
   - "Arduino Kütüphanesini Yükle" seçin

2. **Kısayol ile:**
   - `Cmd+Shift+P` → "Keyboard Shortcuts" → "Arduino Kütüphanesini Yükle" için kısayol tanımlayın

### 💻 **Terminal'de Geliştirme:**

```bash
# Kod değişikliği yaptınız
# Terminal'de proje klasöründe:
./install_library.sh

# Arduino IDE otomatik yeniden başlar
# Yeni kodunuz hazır!
```

### 🔄 **GitHub'dan Güncelleme:**

```bash
# GitHub'dan son versiyonu çekin
git pull origin main

# Kütüphaneyi güncelleyin
./install_library.sh

# Arduino IDE'de güncel kütüphane hazır!
```

### 🆕 **İlk Kurulum:**

```bash
# Projeyi indirin
git clone https://github.com/kayadev/BIG2000-SX1280.git
cd BIG2000-SX1280

# Kütüphaneyi kurun
./install_library.sh

# Arduino IDE'de kullanmaya başlayın!
```

## 🛠️ **Script Özellikleri**

### ✅ **Güvenlik:**

- Mevcut kütüphaneyi yedekler
- Gerekli dosyaları kontrol eder
- Hatalarda işlemi durdurur

### 🎨 **Kullanıcı Dostu:**

- Renkli çıktı
- Aşama aşama bilgi
- Doğrulama kontrolü
- Otomatik Arduino IDE restart

### 🔧 **Platform Desteği:**

- ✅ macOS
- ✅ Linux
- ✅ Windows
- ✅ Otomatik platform algılama

## 🔧 **Sorun Giderme**

### ❌ "Arduino libraries klasörü bulunamadı"

```
Çözüm: Arduino IDE'nin doğru kurulu olduğundan emin olun
- macOS: ~/Documents/Arduino/libraries/
- Linux: ~/Arduino/libraries/
- Windows: %USERPROFILE%\Documents\Arduino\libraries\
```

### ❌ "Eksik dosyalar"

```
Çözüm: Proje klasöründe çalıştırdığınızdan emin olun
cd /path/to/BIG2000-SX1280
./install_library.sh
```

### ❌ "Permission denied"

```
Çözüm: Script'e execute yetkisi verin
chmod +x install_library.sh
```

## 📝 **Ek Komutlar**

### 🔧 **Sadece Examples Düzelt:**

```bash
./fix_examples.sh
```

### 🔍 **Manuel Kontrol:**

```bash
ls -la ~/Documents/Arduino/libraries/BIG2000_SX1280/
```

### 🗑️ **Manuel Silme:**

```bash
rm -rf ~/Documents/Arduino/libraries/BIG2000_SX1280/
```

## 🎉 **Başarı Kontrolü**

Script çalıştıktan sonra Arduino IDE'de:

1. **Sketch** → **Include Library** → "BIG2000_SX1280" görünmeli
2. **File** → **Examples** → **BIG2000_SX1280** altında örnekler olmalı
3. Syntax highlighting çalışmalı

## 🚀 **Hızlı Test**

```cpp
#include "BIG2000_SX1280.h"
// Kod tamamlama çalışıyorsa script başarılı!
```

## 🔧 **VS Code Entegrasyonu**

### ⚙️ **Otomatik Task Kurulumu**

VS Code workspace'inize otomatik olarak bir task eklendi! Artık terminal açmadan direkt VS Code üzerinden kütüphaneyi yükleyebilirsiniz.

### 🎛️ **Task Çalıştırma Yolları**

1. **Command Palette:**

   - `Cmd+Shift+P` (macOS) / `Ctrl+Shift+P` (Windows/Linux)
   - "Tasks: Run Task" yazın ve Enter
   - "Arduino Kütüphanesini Yükle" seçin

2. **Terminal Menu:**

   - VS Code menüsünde "Terminal" → "Run Task..."
   - "Arduino Kütüphanesini Yükle" seçin

3. **Klavye Kısayolu (Opsiyonel):**
   - `Cmd+Shift+P` → "Preferences: Open Keyboard Shortcuts"
   - "Arduino Kütüphanesini Yükle" arayın ve istediğiniz kısayolu atayın

### 📊 **Task Avantajları**

- ✅ Tek tıkla kütüphane yükleme
- ✅ Terminal çıktısını VS Code'da görebilme
- ✅ Hata durumunda detaylı log görüntüleme
- ✅ Geliştirme akışını hızlandırma
- ✅ Script hatalarını VS Code'da yakalama

### 💡 **Geliştirme Akışı**

```
Kod Değişikliği → Cmd+Shift+P → Run Task → Arduino'da Test
```

---

**Bu script sayesinde geliştirme süreciniz çok hızlanacak!** ⚡

**Tek komutla:** Kod → Test → Deploy 🎯
