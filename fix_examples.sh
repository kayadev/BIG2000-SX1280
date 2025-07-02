#!/bin/bash

# Arduino Examples Klasör Düzeltici
# Bu script .ino dosyalarını Arduino IDE standardına uygun klasörlere taşır

set -e

PROJECT_DIR="$(pwd)"
EXAMPLES_DIR="$PROJECT_DIR/examples"

echo "🔧 Arduino Examples Klasör Düzenleyici"
echo "======================================"
echo

if [ ! -d "$EXAMPLES_DIR" ]; then
    echo "❌ Examples klasörü bulunamadı: $EXAMPLES_DIR"
    exit 1
fi

echo "📂 Examples klasörü: $EXAMPLES_DIR"
echo

# .ino dosyalarını bul
ino_files=($(find "$EXAMPLES_DIR" -maxdepth 1 -name "*.ino"))

if [ ${#ino_files[@]} -eq 0 ]; then
    echo "✅ Tüm örnekler zaten doğru formatta!"
    exit 0
fi

echo "🔍 Düzeltilmesi gereken .ino dosyaları:"
for file in "${ino_files[@]}"; do
    filename=$(basename "$file" .ino)
    echo "  - $filename.ino"
done

echo

# Her .ino dosyası için klasör oluştur ve taşı
for file in "${ino_files[@]}"; do
    filename=$(basename "$file" .ino)
    target_dir="$EXAMPLES_DIR/$filename"
    
    echo "📁 $filename için klasör oluşturuluyor..."
    mkdir -p "$target_dir"
    
    echo "📋 $filename.ino taşınıyor..."
    mv "$file" "$target_dir/"
    
    echo "✅ $filename tamamlandı"
    echo
done

echo "🎉 Tüm örnekler Arduino IDE formatına çevrildi!"
echo
echo "📋 Sonuç:"
for dir in "$EXAMPLES_DIR"/*/; do
    if [ -d "$dir" ]; then
        dirname=$(basename "$dir")
        if [ -f "$dir/$dirname.ino" ]; then
            echo "  ✅ $dirname/"
            echo "     └── $dirname.ino"
        fi
    fi
done

echo
echo "🚀 Arduino IDE'de File → Examples → BIG2000_SX1280 altında görünecek!"
