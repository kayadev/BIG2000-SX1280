@echo off
setlocal enabledelayedexpansion

:: BIG2000_SX1280 Arduino Kütüphane Yükleyici (Windows)
:: Bu script projeyi otomatik olarak Arduino libraries klasörüne yükler

title BIG2000_SX1280 Arduino Kütüphane Yükleyici

echo.
echo ████████████████████████████████████████
echo █ BIG2000_SX1280 Arduino Kütüphane     █
echo █ Otomatik Yükleyici v1.0 (Windows)   █
echo ████████████████████████████████████████
echo.

:: Arduino libraries klasörünü belirle
set "ARDUINO_LIBS=%USERPROFILE%\Documents\Arduino\libraries"
set "PROJECT_DIR=%cd%"
set "TARGET_DIR=%ARDUINO_LIBS%\BIG2000_SX1280"
set "LIBRARY_NAME=BIG2000_SX1280"

echo 📂 Proje Klasörü: %PROJECT_DIR%
echo 📂 Hedef Klasör: %TARGET_DIR%
echo.

:: Arduino libraries klasörü var mı kontrol et
if not exist "%ARDUINO_LIBS%" (
    echo ❌ Arduino libraries klasörü bulunamadı: %ARDUINO_LIBS%
    echo 💡 Arduino IDE kurulu mu? Doğru yolda mı?
    pause
    exit /b 1
)

echo ✅ Arduino libraries klasörü bulundu
echo.

:: Mevcut kütüphane var mı kontrol et
if exist "%TARGET_DIR%" (
    echo ⚠️  Mevcut kütüphane bulundu: %TARGET_DIR%
    echo 🗑️  Eski kütüphane siliniyor...
    rmdir /s /q "%TARGET_DIR%" 2>nul
    echo ✅ Eski kütüphane silindi
)

:: Gerekli dosyaları kontrol et
set "error_count=0"

if not exist "%PROJECT_DIR%\src\BIG2000_SX1280.h" (
    echo ❌ Eksik dosya: src\BIG2000_SX1280.h
    set /a error_count+=1
)

if not exist "%PROJECT_DIR%\src\BIG2000_SX1280.cpp" (
    echo ❌ Eksik dosya: src\BIG2000_SX1280.cpp
    set /a error_count+=1
)

if not exist "%PROJECT_DIR%\library.properties" (
    echo ❌ Eksik dosya: library.properties
    set /a error_count+=1
)

if !error_count! gtr 0 (
    echo ❌ %error_count% eksik dosya bulundu!
    pause
    exit /b 1
)

echo ✅ Tüm gerekli dosyalar mevcut
echo.

:: Hedef klasörü oluştur
echo 📁 Hedef klasör oluşturuluyor...
mkdir "%TARGET_DIR%" 2>nul

:: Dosyaları kopyala
echo 📋 Dosyalar kopyalanıyor...

:: src klasörünü kopyala
if exist "%PROJECT_DIR%\src" (
    xcopy "%PROJECT_DIR%\src" "%TARGET_DIR%\src\" /s /i /y >nul
    echo    ✅ src\ klasörü kopyalandı
)

:: examples klasörünü kopyala ve Arduino formatına çevir
if exist "%PROJECT_DIR%\examples" (
    mkdir "%TARGET_DIR%\examples" 2>nul
    
    :: .ino dosyalarını bul ve her biri için klasör oluştur
    for %%f in ("%PROJECT_DIR%\examples\*.ino") do (
        set "filename=%%~nf"
        mkdir "%TARGET_DIR%\examples\!filename!" 2>nul
        copy "%%f" "%TARGET_DIR%\examples\!filename!\" >nul
        echo    ✅ !filename! örneği kopyalandı
    )
    
    :: Alt klasörleri de kopyala
    for /d %%d in ("%PROJECT_DIR%\examples\*") do (
        xcopy "%%d" "%TARGET_DIR%\examples\%%~nd\" /s /i /y >nul
        echo    ✅ %%~nd örneği kopyalandı
    )
)

:: Diğer önemli dosyaları kopyala
if exist "%PROJECT_DIR%\library.properties" (
    copy "%PROJECT_DIR%\library.properties" "%TARGET_DIR%\" >nul
    echo    ✅ library.properties kopyalandı
)

if exist "%PROJECT_DIR%\keywords.txt" (
    copy "%PROJECT_DIR%\keywords.txt" "%TARGET_DIR%\" >nul
    echo    ✅ keywords.txt kopyalandı
)

if exist "%PROJECT_DIR%\LICENSE" (
    copy "%PROJECT_DIR%\LICENSE" "%TARGET_DIR%\" >nul
    echo    ✅ LICENSE kopyalandı
)

if exist "%PROJECT_DIR%\README.md" (
    copy "%PROJECT_DIR%\README.md" "%TARGET_DIR%\" >nul
    echo    ✅ README.md kopyalandı
)

:: extras klasörünü kopyala (varsa)
if exist "%PROJECT_DIR%\extras" (
    xcopy "%PROJECT_DIR%\extras" "%TARGET_DIR%\extras\" /s /i /y >nul
    echo    ✅ extras\ klasörü kopyalandı
)

echo.
echo 🎉 Kütüphane başarıyla yüklendi!
echo.

:: Doğrulama
echo 🔍 Kurulum doğrulanıyor...

if exist "%TARGET_DIR%\src\BIG2000_SX1280.h" (
    echo    ✅ Header dosyası: OK
) else (
    echo    ❌ Header dosyası bulunamadı
)

if exist "%TARGET_DIR%\src\BIG2000_SX1280.cpp" (
    echo    ✅ Implementation dosyası: OK
) else (
    echo    ❌ Implementation dosyası bulunamadı
)

if exist "%TARGET_DIR%\library.properties" (
    echo    ✅ Library properties: OK
) else (
    echo    ❌ Library properties bulunamadı
)

:: Examples kontrolü
set "example_count=0"
for %%f in ("%TARGET_DIR%\examples\*\*.ino") do set /a example_count+=1

if !example_count! gtr 0 (
    echo    ✅ Örnekler: !example_count! adet bulundu
) else (
    echo    ⚠️  Örnek dosya bulunamadı
)

echo.
echo 📋 Kurulum Özeti:
echo    📁 Hedef: %TARGET_DIR%
echo    📝 Library: %LIBRARY_NAME%
echo    🔧 Durum: Hazır
echo.

echo 📱 Sonraki Adımlar:
echo    1. Arduino IDE'yi yeniden başlatın
echo    2. Sketch → Include Library menüsünde 'BIG2000_SX1280' arayın
echo    3. File → Examples → BIG2000_SX1280 altından örnekleri açın
echo.

echo 🚀 Kütüphane kullanıma hazır!
echo.

set /p restart_ide="Arduino IDE'yi şimdi yeniden başlatmak istiyor musunuz? (y/N): "
if /i "!restart_ide!"=="y" (
    echo 🔄 Arduino IDE kapatılıyor...
    taskkill /im arduino.exe /f 2>nul
    timeout /t 2 >nul
    echo 🚀 Arduino IDE başlatılıyor...
    start "" "C:\Program Files (x86)\Arduino\arduino.exe" 2>nul || (
        start "" "C:\Program Files\Arduino\arduino.exe" 2>nul || (
            echo Arduino IDE manuel olarak başlatın
        )
    )
)

echo ✨ İşlem tamamlandı!
pause
