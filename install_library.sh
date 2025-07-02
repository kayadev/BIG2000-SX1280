#!/bin/bash

# BIG2000_SX1280 Arduino Kütüphane Yükleyici
# Bu script projeyi otomatik olarak Arduino libraries klasörüne yükler

set -e  # Hata durumunda scripti durdur

# Renkli çıktı için ANSI kodları
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logo
echo -e "${BLUE}"
echo "████████████████████████████████████████"
echo "█ BIG2000_SX1280 Arduino Kütüphane     █"
echo "█ Otomatik Yükleyici v1.0              █"
echo "████████████████████████████████████████"
echo -e "${NC}"

# Arduino libraries klasörünü belirle
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    ARDUINO_LIBS="$HOME/Documents/Arduino/libraries"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    ARDUINO_LIBS="$HOME/Arduino/libraries"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    # Windows
    ARDUINO_LIBS="$HOME/Documents/Arduino/libraries"
else
    echo -e "${RED}❌ Desteklenmeyen işletim sistemi!${NC}"
    exit 1
fi

# Proje klasörü
PROJECT_DIR="$(pwd)"
TARGET_DIR="$ARDUINO_LIBS/BIG2000_SX1280"
LIBRARY_NAME="BIG2000_SX1280"

echo -e "${BLUE}📂 Proje Klasörü: ${PROJECT_DIR}${NC}"
echo -e "${BLUE}📂 Hedef Klasör: ${TARGET_DIR}${NC}"
echo ""

# Arduino libraries klasörü var mı kontrol et
if [ ! -d "$ARDUINO_LIBS" ]; then
    echo -e "${RED}❌ Arduino libraries klasörü bulunamadı: $ARDUINO_LIBS${NC}"
    echo -e "${YELLOW}💡 Arduino IDE kurulu mu? Doğru yolda mı?${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Arduino libraries klasörü bulundu${NC}"

# Mevcut kütüphane var mı kontrol et
if [ -d "$TARGET_DIR" ]; then
    echo -e "${YELLOW}⚠️  Mevcut kütüphane bulundu: $TARGET_DIR${NC}"
    echo -e "${YELLOW}🗑️  Eski kütüphane siliniyor...${NC}"
    rm -rf "$TARGET_DIR"
    echo -e "${GREEN}✅ Eski kütüphane silindi${NC}"
fi

# Gerekli dosyaları kontrol et
required_files=("src/BIG2000_SX1280.h" "src/BIG2000_SX1280.cpp" "library.properties")
missing_files=()

for file in "${required_files[@]}"; do
    if [ ! -f "$PROJECT_DIR/$file" ]; then
        missing_files+=("$file")
    fi
done

if [ ${#missing_files[@]} -ne 0 ]; then
    echo -e "${RED}❌ Eksik dosyalar:${NC}"
    for file in "${missing_files[@]}"; do
        echo -e "${RED}   - $file${NC}"
    done
    exit 1
fi

echo -e "${GREEN}✅ Tüm gerekli dosyalar mevcut${NC}"

# Hedef klasörü oluştur
echo -e "${BLUE}📁 Hedef klasör oluşturuluyor...${NC}"
mkdir -p "$TARGET_DIR"

# Ana dosyaları kopyala
echo -e "${BLUE}📋 Dosyalar kopyalanıyor...${NC}"

# src klasörünü kopyala
if [ -d "$PROJECT_DIR/src" ]; then
    cp -r "$PROJECT_DIR/src" "$TARGET_DIR/"
    echo -e "${GREEN}   ✅ src/ klasörü kopyalandı${NC}"
fi

# examples klasörünü kopyala ve Arduino formatına çevir
if [ -d "$PROJECT_DIR/examples" ]; then
    mkdir -p "$TARGET_DIR/examples"
    
    echo -e "${BLUE}   📁 Examples kopyalanıyor ve formatlanıyor...${NC}"
    
    # Mevcut examples alt klasörlerini direkt kopyala (zaten doğru formatta)
    for example_dir in "$PROJECT_DIR/examples"/*/; do
        if [ -d "$example_dir" ]; then
            example_name=$(basename "$example_dir")
            # Sadece klasörün içeriğini kopyala, klasörün kendisini değil
            mkdir -p "$TARGET_DIR/examples/$example_name"
            cp -r "$example_dir"* "$TARGET_DIR/examples/$example_name/"
            echo -e "${GREEN}   ✅ $example_name örneği kopyalandı${NC}"
        fi
    done
    
    # Eğer examples klasöründe direkt .ino dosyaları varsa onları da klasörlere koy
    # Ama sadece alt klasör yoksa yapalım
    direct_ino_files=($(find "$PROJECT_DIR/examples" -maxdepth 1 -name "*.ino"))
    if [ ${#direct_ino_files[@]} -gt 0 ]; then
        echo -e "${YELLOW}   ⚠️  Düz .ino dosyaları bulundu, klasörlere taşınıyor...${NC}"
        for ino_file in "${direct_ino_files[@]}"; do
            filename=$(basename "$ino_file" .ino)
            # Sadece aynı isimde klasör yoksa oluştur
            if [ ! -d "$TARGET_DIR/examples/$filename" ]; then
                mkdir -p "$TARGET_DIR/examples/$filename"
                cp "$ino_file" "$TARGET_DIR/examples/$filename/"
                echo -e "${GREEN}   ✅ $filename örneği kopyalandı (klasör formatında)${NC}"
            fi
        done
    fi
fi

# Diğer önemli dosyaları kopyala
important_files=("library.properties" "keywords.txt" "LICENSE" "README.md")

for file in "${important_files[@]}"; do
    if [ -f "$PROJECT_DIR/$file" ]; then
        cp "$PROJECT_DIR/$file" "$TARGET_DIR/"
        echo -e "${GREEN}   ✅ $file kopyalandı${NC}"
    fi
done

# extras klasörünü kopyala (varsa)
if [ -d "$PROJECT_DIR/extras" ]; then
    cp -r "$PROJECT_DIR/extras" "$TARGET_DIR/"
    echo -e "${GREEN}   ✅ extras/ klasörü kopyalandı${NC}"
fi

echo ""
echo -e "${GREEN}🎉 Kütüphane başarıyla yüklendi!${NC}"
echo ""

# Doğrulama
echo -e "${BLUE}🔍 Kurulum doğrulanıyor...${NC}"

if [ -f "$TARGET_DIR/src/BIG2000_SX1280.h" ]; then
    echo -e "${GREEN}   ✅ Header dosyası: OK${NC}"
else
    echo -e "${RED}   ❌ Header dosyası bulunamadı${NC}"
fi

if [ -f "$TARGET_DIR/src/BIG2000_SX1280.cpp" ]; then
    echo -e "${GREEN}   ✅ Implementation dosyası: OK${NC}"
else
    echo -e "${RED}   ❌ Implementation dosyası bulunamadı${NC}"
fi

if [ -f "$TARGET_DIR/library.properties" ]; then
    echo -e "${GREEN}   ✅ Library properties: OK${NC}"
else
    echo -e "${RED}   ❌ Library properties bulunamadı${NC}"
fi

# Examples kontrolü
example_count=$(find "$TARGET_DIR/examples" -name "*.ino" 2>/dev/null | wc -l)
if [ $example_count -gt 0 ]; then
    echo -e "${GREEN}   ✅ Örnekler: $example_count adet bulundu${NC}"
else
    echo -e "${YELLOW}   ⚠️  Örnek dosya bulunamadı${NC}"
fi

echo ""
echo -e "${GREEN}📋 Kurulum Özeti:${NC}"
echo -e "${GREEN}   📁 Hedef: $TARGET_DIR${NC}"
echo -e "${GREEN}   📝 Library: $LIBRARY_NAME${NC}"
echo -e "${GREEN}   🔧 Durum: Hazır${NC}"
echo ""

echo -e "${YELLOW}📱 Sonraki Adımlar:${NC}"
echo -e "${YELLOW}   1. Arduino IDE'yi yeniden başlatın${NC}"
echo -e "${YELLOW}   2. Sketch → Include Library menüsünde 'BIG2000_SX1280' arayın${NC}"
echo -e "${YELLOW}   3. File → Examples → BIG2000_SX1280 altından örnekleri açın${NC}"
echo ""

echo -e "${BLUE}🚀 Kütüphane kullanıma hazır!${NC}"
echo ""

# Arduino IDE'yi yeniden başlatma önerisi
read -p "Arduino IDE'yi şimdi yeniden başlatmak istiyor musunuz? (y/N): " restart_ide
if [[ $restart_ide =~ ^[Yy]$ ]]; then
    echo -e "${BLUE}🔄 Arduino IDE kapatılıyor...${NC}"
    # macOS için Arduino IDE'yi kapat
    if [[ "$OSTYPE" == "darwin"* ]]; then
        osascript -e 'quit app "Arduino"' 2>/dev/null || true
        sleep 2
        echo -e "${BLUE}🚀 Arduino IDE başlatılıyor...${NC}"
        open -a Arduino 2>/dev/null || echo -e "${YELLOW}Arduino IDE manuel olarak başlatın${NC}"
    else
        echo -e "${YELLOW}Arduino IDE'yi manuel olarak yeniden başlatın${NC}"
    fi
fi

echo -e "${GREEN}✨ İşlem tamamlandı!${NC}"
