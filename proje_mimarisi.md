# 🏗️ Proje Mimarisi

Bu doküman, **SenseBot – RFID Destekli Masaüstü Cihaz Erişim ve Kontrol Asistanı** projesinin genel sistem mimarisini, donanım bileşenlerini ve yazılım akışını açıklamak amacıyla hazırlanmıştır.

SenseBot, Arduino Uno tabanlı, gerçek dünyayla etkileşim kuran fiziksel programlama projesidir. Sistem; RFID doğrulama, load cell ile cihaz algılama, RTC ile saatli kayıt, OLED ekran, RGB LED, buzzer ve servo kilit mekanizmasını birlikte kullanır.

---

## Genel Mimari Yaklaşım

Proje, tek bir basit aç/kapat sisteminden daha kapsamlı olacak şekilde tasarlanmıştır. Sistem yalnızca kapağı açmaz; cihazın gerçekten bırakılıp bırakılmadığını veya alınıp alınmadığını da kontrol eder.

Genel mimari üç ana bölümden oluşur:

1. **Giriş Birimleri**
   - RFID RC522
   - Load cell + HX711
   - Micro switch

2. **İşlem Birimi**
   - Arduino Uno

3. **Çıkış Birimleri**
   - Servo motor
   - OLED ekran
   - RGB LED
   - Buzzer
   - Serial Monitor kayıtları

---

## Sistem Blok Diyagramı

```text
RFID Kart / Tag
      ↓
Arduino Uno
      ↓
Yetki Kontrolü
      ↓
Load Cell + HX711 ile Cihaz Kontrolü
      ↓
Servo Kilit Mekanizması
      ↓
OLED + RGB LED + Buzzer Geri Bildirimi
      ↓
RTC Saat Bilgisi ile Serial Monitor Kaydı
```

---

## Donanım Bileşenleri ve Görevleri

### Arduino Uno

Sistemin ana kontrol kartıdır. Tüm sensörlerden gelen verileri okur ve çıkış birimlerini kontrol eder.

### RFID RC522

Kart ve tag okuma için kullanılır. Yetkili kart, yetkisiz kart, admin kart ve cihaz tagı ayrımı bu modül üzerinden yapılır. SPI haberleşmesi kullanır.

### Load Cell + HX711

Cihazın kutuda olup olmadığını algılamak için kullanılır. Load cell fiziksel ağırlığı algılar, HX711 ise bu veriyi Arduino’nun okuyabileceği hale getirir.

### RTC DS3231

Gerçek zamanlı saat bilgisi sağlar. Serial Monitor kayıtlarında işlemlerin saat bilgisiyle yazdırılması için kullanılır.

### OLED Ekran

Kullanıcıya durum mesajları ve robot yüz ifadeleri gösterir. I2C haberleşmesi kullanır.

### Servo Motor

Fiziksel kilit mekanizmasını açıp kapatır. Yetkili işlem başladığında kilit açılır, işlem başarıyla tamamlandığında tekrar kilitlenir.

### Micro Switch

Kapağın açık veya kapalı olduğunu algılar. Kodda INPUT_PULLUP mantığı ile çalıştırılmıştır.

### RGB LED

Sistem durumunu renklerle gösterir.

- Mavi: Bekleme
- Yeşil: Başarılı işlem
- Kırmızı: Hata
- Sarı: İşlem devam ediyor
- Turkuaz: Admin modu

### Buzzer

Başarılı işlem, hata ve alarm durumlarında sesli uyarı verir.

---

## Yazılım Mimarisi

Kod, durum tabanlı bir sistem mantığıyla çalışır. Her işlem aşaması ayrı bir durum olarak ele alınmıştır.

Başlıca durumlar:

- Bekleme durumu
- Cihaz bırakma akışı
- Cihaz alma akışı
- Admin modu
- Sonuç gösterme durumu
- Hata durumları

Bu yapı sayesinde sistemin farklı senaryolarda nasıl davranacağı daha düzenli şekilde yönetilmiştir.

---

## Ana Çalışma Akışları

### 1. Cihaz Bırakma Akışı

1. Sistem bekleme modundadır.
2. Yetkili kart okutulur.
3. Kutuda cihaz yoksa cihaz bırakma senaryosu başlar.
4. Cihaz tagı okutulur.
5. Servo kilidi açar.
6. Kullanıcı kapağı açar.
7. Cihaz load cell platformuna bırakılır.
8. Kapak kapatılır.
9. Sistem ağırlığı kontrol eder.
10. Uygun ağırlık varsa kilit kapanır ve işlem kaydedilir.

### 2. Cihaz Alma Akışı

1. Kutuda cihaz varken yetkili kart okutulur.
2. Sistem cihaz alma moduna geçer.
3. Servo kilidi açar.
4. Kullanıcı kapağı açar.
5. Cihaz platformdan alınır.
6. Cihaz tagı okutulur.
7. Kapak kapatılır.
8. Load cell cihazın alındığını doğrular.
9. Kilit kapanır ve işlem kaydedilir.

### 3. Admin Modu

1. Admin kart okutulur.
2. Sistem admin moduna geçer.
3. Kilit açılır.
4. RGB LED turkuaz yanar.
5. Kapak kapatıldığında kilit tekrar kapanır.
6. Admin işlemi Serial Monitor’a kaydedilir.

---

## Hata Yönetimi

Sistem aşağıdaki hata durumlarını algılayacak şekilde tasarlanmıştır:

- Yetkisiz kart
- Yetkisiz tag
- Cihaz konmadı
- Cihaz alınmadı
- Ağırlık hatası
- Süre doldu

Hata durumlarında sistem kırmızı LED, hata sesi ve OLED mesajı ile kullanıcıyı uyarır. Bazı kritik hatalarda kilit kapanmaz ve kullanıcı doğru işlemi yapana kadar alarm devam eder.

---

## Haberleşme Protokolleri

| Protokol / Yöntem | Kullanılan Modül |
|---|---|
| I2C | OLED ekran, RTC DS3231 |
| SPI | RFID RC522 |
| Dijital Giriş | Micro switch |
| Dijital/PWM Çıkış | RGB LED, buzzer, servo |
| Analog/Dijital Veri Okuma | HX711 üzerinden load cell |
| UART / Serial | Serial Monitor kayıtları |

---

## Sonuç

SenseBot mimarisi; sensör, aktüatör, ekran, sesli uyarı, saatli kayıt ve fiziksel kilit kontrolünü bir araya getiren bütünleşik bir yapıya sahiptir.

Bu mimari sayesinde proje:

- Modüler
- Test edilebilir
- Gerçek donanım üzerinde çalışabilir
- Hata senaryolarını yönetebilir
- BOZ214 Fiziksel Programlama dersi kapsamındaki birçok Arduino konusunu içerebilir

bir sistem haline getirilmiştir.
