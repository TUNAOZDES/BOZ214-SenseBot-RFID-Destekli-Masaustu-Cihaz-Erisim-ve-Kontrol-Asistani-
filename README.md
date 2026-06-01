# SenseBot – RFID Destekli Masaüstü Cihaz Erişim ve Kontrol Asistanı

**Ders:** BOZ214 – Fiziksel Programlama<br>
**Proje Türü:** Final Projesi<br>
**Geliştirici:** Tuna Özdeş<br>
**Üniversite:** Ankara Üniversitesi<br>
**Durum:** Tamamlandı

---

## 📖 Proje Hakkında

SenseBot, Arduino Uno tabanlı RFID kontrollü akıllı bir güvenlik kutusu projesidir. Bu proje, bir cihazın kutuya yetkili şekilde bırakılmasını ve daha sonra yine yetkili şekilde alınmasını kontrol etmek amacıyla geliştirilmiştir.

Sistem; RFID kart/tag doğrulaması, load cell ile ağırlık algılama, RTC ile saatli kayıt tutma, OLED ekran ile kullanıcı bilgilendirme, RGB LED ve buzzer ile görsel/sesli geri bildirim, servo motor ile fiziksel kilit kontrolü gibi birçok fiziksel programlama bileşenini bir araya getirir.

SenseBot yalnızca kutunun açılıp kapanmasını değil, cihazın gerçekten bırakılıp bırakılmadığını veya alınıp alınmadığını da kontrol eder. Bu sayede sistem hem elektronik hem mekanik hem de yazılımsal olarak gerçek dünyayla etkileşim kuran bütünleşik bir fiziksel programlama projesi haline gelmiştir.

---

## 🎯 Projenin Amacı

Bu projenin amacı, Arduino platformu kullanılarak gerçek dünyayla etkileşim kuran, sensörlerden veri alan, aktüatörleri kontrol eden ve kullanıcıya anlık geri bildirim veren işlevsel bir sistem geliştirmektir.

SenseBot ile hedeflenenler:

* Yetkili kullanıcıların RFID kart ile sisteme erişebilmesi
* Cihazın bırakılması ve alınmasının kontrollü şekilde yapılması
* Load cell ile kutuda cihaz olup olmadığının algılanması
* RTC modülü ile işlemlerin saat bilgisiyle kayıt altına alınması
* OLED ekran üzerinden kullanıcıya anlaşılır durum mesajları gösterilmesi
* RGB LED ve buzzer ile görsel/sesli geri bildirim verilmesi
* Hata senaryolarının sistem tarafından algılanıp kullanıcıya bildirilmesi

---

## 🧩 Temel Özellikler

* **RFID Yetkilendirme:** Yetkili kart, yetkisiz kart, admin kart ve cihaz tagı sistemi
* **Cihaz Bırakma Senaryosu:** Kutunun boş olduğu durumda cihaz bırakma işlemi
* **Cihaz Alma Senaryosu:** Kutunun dolu olduğu durumda cihaz alma işlemi
* **Admin Modu:** Admin kart ile özel erişim ve kilit kontrolü
* **Load Cell ile Ağırlık Algılama:** Cihaz var/yok ve ağırlık hatası kontrolü
* **RTC ile Saatli Kayıt:** Önemli olayların saat bilgisiyle Serial Monitor’a yazdırılması
* **OLED Robot Yüz Arayüzü:** Durum mesajları ve robot yüz ifadeleri
* **RGB LED Geri Bildirimi:** Mavi, yeşil, kırmızı, sarı ve turkuaz durum renkleri
* **Buzzer Uyarıları:** Başarılı işlem, hata ve alarm sesleri
* **Servo Kilit Mekanizması:** Fiziksel kilit açma ve kapatma
* **Micro Switch:** Kapak açık/kapalı durumunun algılanması
* **Hata Yönetimi:** Yetkisiz kart/tag, cihaz konmadı, cihaz alınmadı, ağırlık hatası ve süre doldu senaryoları

---

## 🛠️ Kullanılan Donanımlar

| Donanım             | Kullanım Amacı                                 |
| ------------------- | ---------------------------------------------- |
| **Arduino Uno**     | Sistemin ana kontrol kartı                     |
| **RFID RC522**      | Kart ve tag okuma                              |
| **RFID Kartları**   | Yetkili, yetkisiz ve admin kullanıcı tanımlama |
| **RFID Tag**        | Cihaz doğrulama                                |
| **RTC DS3231**      | Gerçek zamanlı saat bilgisi                    |
| **OLED Ekran**      | Durum mesajı ve robot yüz ifadeleri            |
| **Load Cell**       | Ağırlık algılama                               |
| **HX711 Modülü**    | Load cell verisini Arduino’ya aktarma          |
| **Servo Motor**     | Kilit mekanizmasını açma/kapatma               |
| **Micro Switch**    | Kapak açık/kapalı algılama                     |
| **RGB LED**         | Görsel durum bildirimi                         |
| **Buzzer**          | Sesli uyarı                                    |
| **Breadboard**      | Devre bağlantıları                             |
| **Jumper Kablolar** | Modül bağlantıları                             |
| **Ahşap Kutu**      | Projenin fiziksel gövdesi                      |

---

## ⚙️ Kullanılan Kütüphaneler

Projede aşağıdaki Arduino kütüphaneleri kullanılmıştır:

| Kütüphane              | Kullanım Amacı                 |
| ---------------------- | ------------------------------ |
| **Wire.h**             | I2C haberleşmesi               |
| **SPI.h**              | SPI haberleşmesi               |
| **MFRC522.h**          | RFID RC522 kart/tag okuma      |
| **RTClib.h**           | RTC DS3231 saat modülü         |
| **Adafruit_GFX.h**     | OLED grafik altyapısı          |
| **Adafruit_SSD1306.h** | OLED ekran kontrolü            |
| **Servo.h**            | Servo motor kontrolü           |
| **HX711.h**            | Load cell ağırlık verisi okuma |

---

## 📁 Proje Dosya Yapısı

```text
BOZ214-SenseBot-RFID-Destekli-Masaustu-Cihaz-Erisim-ve-Kontrol-Asistani/
│
├── Tuna_Ozdes_SenseBot_Final_Proje.ino
│   └── Arduino IDE üzerinde çalışan final proje kodu
│
└── README.md
    └── Proje açıklaması, kullanılan donanımlar, çalışma mantığı ve test bilgileri
```

---

## 🔌 Final Pin Bağlantıları

| Arduino Pin | Bağlantı            |
| ----------- | ------------------- |
| **D2**      | Micro switch        |
| **D4**      | Buzzer              |
| **D5**      | RGB LED kırmızı     |
| **D6**      | RGB LED yeşil       |
| **D7**      | RGB LED mavi        |
| **D8**      | RFID RC522 RST      |
| **D9**      | Servo sinyal        |
| **D10**     | RFID RC522 SDA / SS |
| **D11**     | RFID RC522 MOSI     |
| **D12**     | RFID RC522 MISO     |
| **D13**     | RFID RC522 SCK      |
| **A0**      | HX711 DT / DOUT     |
| **A1**      | HX711 SCK           |
| **A4**      | OLED SDA + RTC SDA  |
| **A5**      | OLED SCL + RTC SCL  |

> ⚠️ RFID RC522 modülü 3.3V ile çalıştırılmıştır.
> Tüm modüllerin GND bağlantıları ortak yapılmıştır.

---

## ▶️ Çalışma Mantığı

SenseBot, sistem durumlarına göre çalışan bir akış yapısına sahiptir.

### 1. Bekleme Durumu

Sistem açıldığında OLED ekranda “Kart okutunuz” mesajı ve robot yüz ifadesi görünür. RGB LED mavi renkte yanar. Sistem RFID kart bekler.

### 2. Cihaz Bırakma Senaryosu

Kutuda cihaz yokken yetkili kart okutulursa sistem cihaz bırakma moduna geçer.

İşlem sırası:

1. Yetkili kart okutulur.
2. Cihaz tagı okutulur.
3. Servo kilidi açar.
4. Kullanıcı kapağı açar.
5. Cihaz load cell platformuna bırakılır.
6. Kapak kapatılır.
7. Sistem ağırlığı kontrol eder.
8. Uygun ağırlık algılanırsa kilit kapanır.
9. OLED ekranda “Cihaz birakildi” mesajı görünür.
10. Serial Monitor’a saatli kayıt yazılır.

### 3. Cihaz Alma Senaryosu

Kutuda cihaz varken yetkili kart okutulursa sistem cihaz alma moduna geçer.

İşlem sırası:

1. Yetkili kart okutulur.
2. Servo kilidi açar.
3. Kullanıcı kapağı açar.
4. Cihaz platformdan alınır.
5. Cihaz tagı okutulur.
6. Kapak kapatılır.
7. Sistem load cell üzerinden cihazın alındığını doğrular.
8. Kilit kapanır.
9. OLED ekranda “Cihaz alindi” mesajı görünür.
10. Serial Monitor’a saatli kayıt yazılır.

### 4. Admin Modu

Admin kart okutulduğunda sistem özel erişim moduna geçer.

* Servo kilidi açar.
* RGB LED turkuaz renkte yanar.
* Kapak kapatıldığında sistem tekrar kilitlenir.
* Serial Monitor’a “Admin kart” ve “Admin kapandi” kayıtları yazılır.

---

## 🚨 Hata Senaryoları

Sistem yalnızca başarılı işlemleri değil, hatalı durumları da algılar.

| Hata Durumu        | Sistem Tepkisi                                   |
| ------------------ | ------------------------------------------------ |
| **Yetkisiz kart**  | Kilit açılmaz, kırmızı LED ve hata sesi verir    |
| **Yetkisiz tag**   | İşlem devam etmez, hata bildirimi yapılır        |
| **Cihaz konmadı**  | Kilit kapanmaz, alarm devam eder                 |
| **Cihaz alınmadı** | Kilit kapanmaz, alarm devam eder                 |
| **Ağırlık hatası** | 75000 üstü değerlerde sistem hata verir          |
| **Süre doldu**     | İşlem belirlenen sürede tamamlanmazsa hata verir |

---

## ⚖️ Load Cell Eşik Değerleri

Final testlerde load cell için aşağıdaki değerler kullanılmıştır:

| Durum            | Gözlenen Değer | Karar          |
| ---------------- | -------------- | -------------- |
| Boş platform     | 0–200          | Cihaz yok      |
| USB bellek + tag | 8100–8300      | Cihaz var      |
| Kulaklık         | Yaklaşık 18000 | Cihaz var      |
| Ağır nesne       | 75000 üstü     | Ağırlık hatası |

Final eşikler:

```text
3000 ve altı  -> Cihaz yok
3000 üstü     -> Cihaz var
75000 üstü    -> Ağırlık hatası
```

Ayrıca açılışta otomatik tare yapılmamıştır. Bunun yerine sabit boş ham referans değeri kullanılmıştır:

```text
Boş referans ham değeri: -268500
```

Bu sayede Arduino, cihaz kutudayken açılırsa da cihazı doğru şekilde “var” olarak algılayabilir.

---

## 🖥️ OLED Ekran Arayüzü

OLED ekranda kullanıcıya kısa durum mesajları ve robot yüz ifadeleri gösterilir.

| Yüz İfadesi | Anlamı             |
| ----------- | ------------------ |
| `(-_-)`     | Bekleme            |
| `(o_o)`     | İşlem devam ediyor |
| `(^_^)`     | Başarılı işlem     |
| `(>_<)`     | Hata               |
| `(X_X)`     | Ağırlık hatası     |

Örnek OLED mesajları:

* Kart okutunuz
* Cihaz tag okut
* Kapagi aciniz
* Cihazi koy kapat
* Cihazi al tag okut
* Cihaz birakildi
* Cihaz alindi
* Yetkisiz kart
* Yetkisiz tag
* Cihaz konmadi
* Cihaz alinmadi
* Agirlik hatasi
* Sure doldu
* Admin modu ac
* Admin kapat

---

## 📝 Serial Monitor Kayıtları

RTC modülü sayesinde önemli olaylar saat bilgisiyle Serial Monitor’a yazdırılır.

Örnek kayıtlar:

```text
KAYIT | Yetkili kart | Saat: 18:44:58
KAYIT | Yetkili tag | Saat: 18:45:02
KAYIT | Cihaz birakildi | Saat: 18:45:12
KAYIT | Cihaz alindi | Saat: 18:48:30
KAYIT | Admin kart | Saat: 18:50:05
KAYIT | Admin kapandi | Saat: 18:50:30
KAYIT | Yetkisiz kart | Saat: 18:51:10
KAYIT | Yetkisiz tag | Saat: 18:52:03
KAYIT | Cihaz konmadi | Saat: 18:53:20
KAYIT | Cihaz alinmadi | Saat: 18:54:10
KAYIT | Agirlik hatasi | Saat: 18:55:02
KAYIT | Sure doldu | Saat: 18:56:30
```

---

## 🧠 Sistem Mimarisi

SenseBot çoklu modül entegrasyonu ile çalışır.

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

Sistem; giriş birimleri, işlem birimi ve çıkış birimleri olarak üç ana bölümde düşünülebilir.

### Giriş Birimleri

* RFID RC522
* Load cell
* Micro switch

### İşlem Birimi

* Arduino Uno

### Çıkış Birimleri

* Servo motor
* OLED ekran
* RGB LED
* Buzzer
* Serial Monitor kayıtları

---

## 🧪 Test Edilen Senaryolar

Final aşamada aşağıdaki senaryolar test edilmiştir:

| Test                    | Sonuç    |
| ----------------------- | -------- |
| Servo aç/kapat testi    | Başarılı |
| Micro switch testi      | Başarılı |
| Buzzer testi            | Başarılı |
| RGB LED testi           | Başarılı |
| OLED ekran testi        | Başarılı |
| RTC saat testi          | Başarılı |
| RFID kart testi         | Başarılı |
| RFID tag testi          | Başarılı |
| Load cell testi         | Başarılı |
| Cihaz bırakma testi     | Başarılı |
| Cihaz alma testi        | Başarılı |
| Admin kart testi        | Başarılı |
| Yetkisiz kart/tag testi | Başarılı |
| Cihaz konmadı hatası    | Başarılı |
| Cihaz alınmadı hatası   | Başarılı |
| Ağırlık hatası          | Başarılı |
| Süre doldu hatası       | Başarılı |

---

## 🏗️ Fiziksel Tasarım

Proje ahşap bir kutu üzerine kurulmuştur.

* Ön yüzde OLED ekran, buzzer, RGB LED ve RFID okuma alanı bulunur.
* RFID modülü kutunun iç tarafına monte edilmiştir.
* Kutunun dışına RFID okuma alanını göstermek için sembol yerleştirilmiştir.
* İç bölümde Arduino, breadboard, RTC, HX711 ve bağlantı kabloları bulunur.
* Cihaz yerleştirme alanında load cell destekli platform kullanılmıştır.
* Kapakta servo motorlu kilit mekanizması bulunur.
* Micro switch, kapağın açık veya kapalı olduğunu algılar.

---

## ⚙️ Kurulum ve Kullanım

Bu proje fiziksel donanım üzerinde çalışmaktadır. Kodun çalıştırılması için Arduino IDE kullanılmalıdır.

### 1. Arduino IDE ile Açma

Arduino IDE üzerinden aşağıdaki `.ino` dosyası açılır:

```text
Tuna_Ozdes_SenseBot_Final_Proje.ino
```

### 2. Gerekli Kütüphaneleri Kurma

Arduino IDE Library Manager üzerinden aşağıdaki kütüphaneler kurulmalıdır:

```text
MFRC522
RTClib
Adafruit GFX Library
Adafruit SSD1306
HX711
Servo
```

### 3. Kart Seçimi

Arduino IDE içinde:

```text
Tools > Board > Arduino Uno
```

seçilir.

### 4. Port Seçimi

Arduino bağlıyken uygun COM portu seçilir.

```text
Tools > Port
```

### 5. Kodu Yükleme

Arduino IDE üzerinden **Upload** butonuna basılarak kod Arduino Uno’ya yüklenir.

---

## 📌 Arduino Konuları ile İlişki

Bu proje BOZ214 Fiziksel Programlama dersi kapsamında birçok Arduino konusunu içermektedir:

* Temel Arduino programlama
* Dijital giriş/çıkış kullanımı
* Sensör okuma
* Aktüatör kontrolü
* Servo motor kontrolü
* Buzzer ile ses üretimi
* RGB LED kontrolü
* Serial Monitor kullanımı
* I2C haberleşmesi
* SPI haberleşmesi
* RTC modülü kullanımı
* OLED ekran kullanımı
* RFID modülü kullanımı
* Load cell kalibrasyonu
* Çoklu modül entegrasyonu
* Hata yönetimi ve durum tabanlı programlama

---

## 🧠 Pair Programming ve Yapay Zeka Asistanı Kullanımı

Bu proje geliştirilirken pair programming yaklaşımı kullanılmıştır. Geliştirme sürecinde öğrenci ve yapay zeka asistanı birlikte çalışmıştır.

Bu süreçte:

* Proje fikri ve sistem akışı planlanmıştır.
* Kullanılacak donanımlar belirlenmiştir.
* Arduino kodu aşama aşama geliştirilmiştir.
* Modüller tek tek test edilmiştir.
* Hata senaryoları oluşturulmuştur.
* OLED ekran arayüzü iyileştirilmiştir.
* Load cell eşik değerleri test edilmiştir.
* Açılışta yanlış ağırlık algılama problemi çözülmüştür.
* Final test listesi hazırlanmıştır.
* Rapor, video ve mühendislik tasarım defteri hazırlıkları yapılmıştır.

---

## 📄 Lisans ve Telif Hakkı

Bu projede **Tüm Haklar Saklıdır**.

Kaynak kodları yalnızca inceleme ve eğitim amaçlı erişime sahiptir. İzin alınmadan ticari amaçla kullanılması, kopyalanması veya dağıtılması yasaktır.

**Copyright © 2026 Tuna Özdeş**

*Not: Bu proje Ankara Üniversitesi BOZ214 Fiziksel Programlama dersi kapsamında geliştirilmiştir.*
