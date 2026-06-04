#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include "HX711.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SWITCH_PIN 2
#define BUZZER_PIN 4
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 7
#define RFID_RST_PIN 8
#define SERVO_PIN 9
#define RFID_SS_PIN 10

#define HX711_DT A0
#define HX711_SCK A1

// Final esikler
// 3000 ve alti = cihaz yok
// 3000 ustu = cihaz var
// 75000 ustu = agirlik hatasi
#define CIHAZ_VAR_ESIK 3000
#define CIHAZ_YOK_ESIK 3000
#define CIHAZ_MAX_ESIK 75000

// Load cell bos ham referans degeri
// Final montajda bos tabla yaklasik -268500 verdi.
#define BOS_REFERANS -268500L

#define KRITIK_BIP_ON_MS 180
#define KRITIK_BIP_OFF_MS 40
#define ISLEM_TIMEOUT_MS 20000

// UID rolleri
const char USER_UID[] = "D4 FC FA 03";
const char ADMIN_UID[] = "04 E3 6C 71 41 02 89";
const char DEVICE_TAG_UID[] = "F3 1A 69 2D";

RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
Servo kilitServo;
HX711 scale;

char sonUID[24] = "";

long agirlikDegeri = 0;
bool hxHazir = false;
bool cihazVar = false;

unsigned long sonucZamani = 0;
unsigned long islemBaslangicZamani = 0;

bool alTagDogrulandi = false;

bool kritikAlarmAktif = false;
bool kritikAlarmSesAcik = false;
unsigned long kritikAlarmZamani = 0;

enum Durum {
  BEKLE,

  BIRAK_TAG_BEKLE,
  BIRAK_KAPAK_AC,
  BIRAK_KAPAK_KAPAT,
  BIRAK_HATA_CIHAZ_YOK,
  BIRAK_HATA_AGIRLIK_FAZLA,
  BIRAK_HATA_TIMEOUT,

  AL_KAPAK_AC,
  AL_TAG_BEKLE,
  AL_KAPAK_KAPAT,
  AL_HATA_CIHAZ_VAR,
  AL_HATA_TIMEOUT,

  ADMIN_KAPAK_AC,
  ADMIN_KAPAK_KAPAT,

  SONUC_GOSTER
};

enum Sonuc {
  SONUC_YOK,
  SONUC_BIRAKILDI,
  SONUC_ALINDI,
  HATA_YETKISIZ,
  HATA_TAG,
  HATA_BIRAK_CIHAZ_YOK,
  HATA_BIRAK_AGIRLIK_FAZLA,
  HATA_TIMEOUT,
  HATA_AL_CIHAZ_VAR
};

Durum durum = BEKLE;
Durum sonucSonrasiDurum = BEKLE;
Sonuc sonuc = SONUC_YOK;

void rgbKapat() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void rgbMavi() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
}

void rgbYesil() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
}

void rgbKirmizi() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void rgbSari() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
}

void rgbTurkuaz() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void bipAc(int sure) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(sure);
  digitalWrite(BUZZER_PIN, LOW);
}

void bipKisa() {
  tone(BUZZER_PIN, 1319, 90);
  delay(130);

  tone(BUZZER_PIN, 1760, 180);
  delay(230);

  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void bipHata() {
  for (int tekrar = 0; tekrar < 2; tekrar++) {
    tone(BUZZER_PIN, 1760, 120);
    delay(150);

    tone(BUZZER_PIN, 1319, 220);
    delay(300);
  }

  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void kritikAlarmBaslat() {
  rgbKirmizi();
  kritikAlarmAktif = true;
  kritikAlarmSesAcik = false;
  kritikAlarmZamani = millis() - KRITIK_BIP_OFF_MS;
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void kritikAlarmDurdur() {
  kritikAlarmAktif = false;
  kritikAlarmSesAcik = false;
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void kritikAlarmGuncelle() {
  rgbKirmizi();

  unsigned long simdi = millis();

  if (kritikAlarmSesAcik) {
    if (simdi - kritikAlarmZamani >= KRITIK_BIP_ON_MS) {
      digitalWrite(BUZZER_PIN, LOW);
      kritikAlarmSesAcik = false;
      kritikAlarmZamani = simdi;
    }
  } else {
    if (simdi - kritikAlarmZamani >= KRITIK_BIP_OFF_MS) {
      digitalWrite(BUZZER_PIN, HIGH);
      kritikAlarmSesAcik = true;
      kritikAlarmZamani = simdi;
    }
  }
}

void servoAc() {
  kilitServo.write(0);
  delay(600);
}

void servoKapat() {
  kilitServo.write(90);
  delay(600);
}

void islemZamaniBaslat() {
  islemBaslangicZamani = millis();
}

bool islemSuresiDolduMu() {
  return millis() - islemBaslangicZamani > ISLEM_TIMEOUT_MS;
}

// Serial Monitor olay kaydi.
// Olay metni F("...") ile flash bellekte tutulur.
void kayitYaz(const __FlashStringHelper* olay, DateTime now) {
  Serial.print(F("KAYIT | "));
  Serial.print(olay);
  Serial.print(F(" | Saat: "));

  if (now.hour() < 10) Serial.print(F("0"));
  Serial.print(now.hour());
  Serial.print(F(":"));

  if (now.minute() < 10) Serial.print(F("0"));
  Serial.print(now.minute());
  Serial.print(F(":"));

  if (now.second() < 10) Serial.print(F("0"));
  Serial.println(now.second());
}

char hexChar(byte v) {
  if (v < 10) return '0' + v;
  return 'A' + (v - 10);
}

void uidHazirla() {
  byte pos = 0;

  for (byte i = 0; i < rfid.uid.size && pos < sizeof(sonUID) - 4; i++) {
    byte b = rfid.uid.uidByte[i];

    sonUID[pos++] = hexChar((b >> 4) & 0x0F);
    sonUID[pos++] = hexChar(b & 0x0F);

    if (i < rfid.uid.size - 1) {
      sonUID[pos++] = ' ';
    }
  }

  sonUID[pos] = '\0';
}

bool rfidOku() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    uidHazirla();

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    return true;
  }

  return false;
}

bool uidEsit(const char* a, const char* b) {
  return strcmp(a, b) == 0;
}

bool yetkiliKullaniciMi(const char* uid) {
  return uidEsit(uid, USER_UID);
}

bool adminKartMi(const char* uid) {
  return uidEsit(uid, ADMIN_UID);
}

bool cihazTagMi(const char* uid) {
  return uidEsit(uid, DEVICE_TAG_UID);
}

void sonucAyarla(Sonuc yeniSonuc, Durum sonrakiDurum, bool basarili) {
  sonuc = yeniSonuc;
  sonucSonrasiDurum = sonrakiDurum;
  sonucZamani = millis();
  durum = SONUC_GOSTER;

  if (basarili) {
    rgbYesil();
    bipKisa();
  } else {
    rgbKirmizi();
    bipHata();
  }
}

void kritikHataBaslat(Durum hataDurumu) {
  durum = hataDurumu;
  kritikAlarmBaslat();
}

void adminAcisBaslat(DateTime now) {
  kritikAlarmDurdur();
  alTagDogrulandi = false;

  kayitYaz(F("Admin kart"), now);

  rgbTurkuaz();
  bipKisa();
  servoAc();

  durum = ADMIN_KAPAK_AC;
}

void agirlikOku() {
  if (scale.is_ready()) {
    long ham = scale.get_value(3);

    if (ham < 0) {
      ham = -ham;
    }

    agirlikDegeri = ham;
    hxHazir = true;

    if (agirlikDegeri > CIHAZ_VAR_ESIK) {
      cihazVar = true;
    } else {
      cihazVar = false;
    }

  } else {
    hxHazir = false;
  }
}

bool agirlikNormalMi() {
  return agirlikDegeri <= CIHAZ_MAX_ESIK;
}

// OLED final arayuz:
// Ust sari alan: kisa sabit durum yazisi
// Alt mavi alan: buyuk robot yuz ifadesi
void oledYaz(const __FlashStringHelper* mesaj,
             const __FlashStringHelper* yuz) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);

  // Ust kisim: kisa durum yazisi
  display.setTextSize(1);
  display.setCursor(2, 3);
  display.print(mesaj);

  // Alt kisim: buyuk robot yuz ifadesi
  display.setTextSize(3);
  display.setCursor(19, 32);
  display.print(yuz);

  display.display();
}

void ekranYaz(DateTime now, bool kapakKapali) {
  // now ve kapakKapali sistemde kullanilmaya devam eder.
  // OLED final arayuzunde saat/kapak/agirlik gosterilmez.

  if (durum == BEKLE) {
    oledYaz(F("Kart okutunuz"), F("(-_-)"));
  }

  else if (durum == BIRAK_TAG_BEKLE) {
    oledYaz(F("Cihaz tag okut"), F("(o_o)"));
  }

  else if (durum == BIRAK_KAPAK_AC) {
    oledYaz(F("Kapagi aciniz"), F("(o_o)"));
  }

  else if (durum == BIRAK_KAPAK_KAPAT) {
    oledYaz(F("Cihazi koy kapat"), F("(^_^)"));
  }

  else if (durum == BIRAK_HATA_CIHAZ_YOK) {
    oledYaz(F("Cihaz konmadi"), F("(>_<)"));
  }

  else if (durum == BIRAK_HATA_AGIRLIK_FAZLA) {
    oledYaz(F("Agirlik hatasi"), F("(X_X)"));
  }

  else if (durum == BIRAK_HATA_TIMEOUT) {
    oledYaz(F("Sure doldu"), F("(>_<)"));
  }

  else if (durum == AL_KAPAK_AC) {
    oledYaz(F("Kapagi aciniz"), F("(o_o)"));
  }

  else if (durum == AL_TAG_BEKLE) {
    oledYaz(F("Cihazi al tag okut"), F("(o_o)"));
  }

  else if (durum == AL_KAPAK_KAPAT) {
    oledYaz(F("Kapagi kapatin"), F("(^_^)"));
  }

  else if (durum == AL_HATA_CIHAZ_VAR) {
    oledYaz(F("Cihaz alinmadi"), F("(>_<)"));
  }

  else if (durum == AL_HATA_TIMEOUT) {
    oledYaz(F("Sure doldu"), F("(>_<)"));
  }

  else if (durum == ADMIN_KAPAK_AC) {
    oledYaz(F("Admin modu ac"), F("(o_o)"));
  }

  else if (durum == ADMIN_KAPAK_KAPAT) {
    oledYaz(F("Admin kapat"), F("(o_o)"));
  }

  else if (durum == SONUC_GOSTER) {
    if (sonuc == SONUC_BIRAKILDI) {
      oledYaz(F("Cihaz birakildi"), F("(^_^)"));
    }

    else if (sonuc == SONUC_ALINDI) {
      oledYaz(F("Cihaz alindi"), F("(^_^)"));
    }

    else if (sonuc == HATA_YETKISIZ) {
      oledYaz(F("Yetkisiz kart"), F("(>_<)"));
    }

    else if (sonuc == HATA_TAG) {
      oledYaz(F("Yetkisiz tag"), F("(>_<)"));
    }

    else if (sonuc == HATA_BIRAK_CIHAZ_YOK) {
      oledYaz(F("Cihaz yok"), F("(>_<)"));
    }

    else if (sonuc == HATA_BIRAK_AGIRLIK_FAZLA) {
      oledYaz(F("Agirlik hatasi"), F("(X_X)"));
    }

    else if (sonuc == HATA_TIMEOUT) {
      oledYaz(F("Sure doldu"), F("(>_<)"));
    }

    else if (sonuc == HATA_AL_CIHAZ_VAR) {
      oledYaz(F("Cihaz hala var"), F("(>_<)"));
    }

    else {
      oledYaz(F("Islem tamam"), F("(o_o)"));
    }
  }

  else {
    oledYaz(F("SenseBot hazir"), F("(o_o)"));
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(SWITCH_PIN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  rgbKapat();

  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED bulunamadi"));
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 3);
  display.println(F("SenseBot basliyor"));
  display.display();

  if (!rtc.begin()) {
    Serial.println(F("RTC bulunamadi"));
    while (1);
  }

  SPI.begin();
  rfid.PCD_Init();

  kilitServo.attach(SERVO_PIN);
  servoKapat();

  scale.begin(HX711_DT, HX711_SCK);

  delay(1000);

  // Artik acilista otomatik tare yapmiyoruz.
  // Bos referans sabit kullaniliyor.
  scale.set_offset(BOS_REFERANS);

  if (scale.is_ready()) {
    hxHazir = true;
  } else {
    hxHazir = false;
  }

  rgbMavi();
}

void loop() {
  DateTime now = rtc.now();
  bool kapakKapali = (digitalRead(SWITCH_PIN) == LOW);

  agirlikOku();

  bool kartOkundu = rfidOku();

  // Admin kart her durumda onceliklidir.
  if (kartOkundu && adminKartMi(sonUID)) {
    adminAcisBaslat(now);
    ekranYaz(now, kapakKapali);
    delay(150);
    return;
  }

  // Admin acis modu: once kapagin acilmasini bekler.
  if (durum == ADMIN_KAPAK_AC) {
    rgbTurkuaz();

    if (!kapakKapali) {
      durum = ADMIN_KAPAK_KAPAT;
    }

    ekranYaz(now, kapakKapali);
    delay(150);
    return;
  }

  // Admin kapatma modu: kapak kapaninca kilitler, yesil onay verir ve beklemeye doner.
  if (durum == ADMIN_KAPAK_KAPAT) {
    rgbTurkuaz();

    if (kapakKapali) {
      servoKapat();

      kayitYaz(F("Admin kapandi"), now);

      rgbYesil();
      ekranYaz(now, kapakKapali);
      bipKisa();
      delay(1000);

      rgbMavi();
      durum = BEKLE;
    }

    ekranYaz(now, kapakKapali);
    delay(150);
    return;
  }

  // Birakma: cihaz konmadi hatasi
  if (durum == BIRAK_HATA_CIHAZ_YOK) {
    if (kapakKapali && cihazVar && agirlikNormalMi()) {
      kritikAlarmDurdur();

      rgbSari();
      ekranYaz(now, kapakKapali);
      delay(600);

      servoKapat();
      kayitYaz(F("Cihaz birakildi"), now);
      sonucAyarla(SONUC_BIRAKILDI, BEKLE, true);
    } else if (kapakKapali && cihazVar && !agirlikNormalMi()) {
      kayitYaz(F("Agirlik hatasi"), now);
      durum = BIRAK_HATA_AGIRLIK_FAZLA;
    } else {
      kritikAlarmGuncelle();
    }

    ekranYaz(now, kapakKapali);
    delay(20);
    return;
  }

  // Birakma: agirlik fazla hatasi
  if (durum == BIRAK_HATA_AGIRLIK_FAZLA) {
    if (kapakKapali && cihazVar && agirlikNormalMi()) {
      kritikAlarmDurdur();

      rgbSari();
      ekranYaz(now, kapakKapali);
      delay(600);

      servoKapat();
      kayitYaz(F("Cihaz birakildi"), now);
      sonucAyarla(SONUC_BIRAKILDI, BEKLE, true);
    } 
    else if (kapakKapali && !cihazVar) {
      kayitYaz(F("Cihaz konmadi"), now);
      durum = BIRAK_HATA_CIHAZ_YOK;
    } 
    else {
      kritikAlarmGuncelle();
    }

    ekranYaz(now, kapakKapali);
    delay(20);
    return;
  }

  // Birakma: zaman asimi hatasi
  if (durum == BIRAK_HATA_TIMEOUT) {
    if (kapakKapali && cihazVar && agirlikNormalMi()) {
      kritikAlarmDurdur();

      rgbSari();
      ekranYaz(now, kapakKapali);
      delay(600);

      servoKapat();
      kayitYaz(F("Cihaz birakildi"), now);
      sonucAyarla(SONUC_BIRAKILDI, BEKLE, true);
    } 
    else if (kapakKapali && cihazVar && !agirlikNormalMi()) {
      kayitYaz(F("Agirlik hatasi"), now);
      durum = BIRAK_HATA_AGIRLIK_FAZLA;
    } 
    else if (kapakKapali && !cihazVar) {
      kayitYaz(F("Cihaz konmadi"), now);
      durum = BIRAK_HATA_CIHAZ_YOK;
    } 
    else {
      kritikAlarmGuncelle();
    }

    ekranYaz(now, kapakKapali);
    delay(20);
    return;
  }

  // Alma: cihaz hala var hatasi
  if (durum == AL_HATA_CIHAZ_VAR) {
    if (kapakKapali && !cihazVar) {
      kritikAlarmDurdur();

      rgbSari();
      ekranYaz(now, kapakKapali);
      delay(600);

      servoKapat();
      kayitYaz(F("Cihaz alindi"), now);
      sonucAyarla(SONUC_ALINDI, BEKLE, true);
    } else {
      kritikAlarmGuncelle();
    }

    ekranYaz(now, kapakKapali);
    delay(20);
    return;
  }

  // Alma: zaman asimi hatasi
  if (durum == AL_HATA_TIMEOUT) {
    if (!alTagDogrulandi && kartOkundu) {
      if (cihazTagMi(sonUID)) {
        kayitYaz(F("Yetkili tag"), now);
        alTagDogrulandi = true;
      } else {
        kayitYaz(F("Yetkisiz tag"), now);
      }
    }

    if (kapakKapali && !cihazVar && alTagDogrulandi) {
      kritikAlarmDurdur();

      rgbSari();
      ekranYaz(now, kapakKapali);
      delay(600);

      servoKapat();
      kayitYaz(F("Cihaz alindi"), now);
      sonucAyarla(SONUC_ALINDI, BEKLE, true);
    } else if (kapakKapali && cihazVar && alTagDogrulandi) {
      kayitYaz(F("Cihaz alinmadi"), now);
      durum = AL_HATA_CIHAZ_VAR;
    } else {
      kritikAlarmGuncelle();
    }

    ekranYaz(now, kapakKapali);
    delay(20);
    return;
  }

  // Sonuc ekrani
  if (durum == SONUC_GOSTER) {
    if (millis() - sonucZamani > 2500) {
      durum = sonucSonrasiDurum;

      if (durum == BEKLE) {
        rgbMavi();
      } else {
        rgbSari();
      }
    }

    ekranYaz(now, kapakKapali);
    delay(150);
    return;
  }

  // Normal RFID islemleri
  if (kartOkundu) {
    if (durum == BEKLE) {
      if (yetkiliKullaniciMi(sonUID)) {
        kayitYaz(F("Yetkili kart"), now);

        if (cihazVar) {
          rgbYesil();
          bipKisa();
          servoAc();
          alTagDogrulandi = false;
          islemZamaniBaslat();
          durum = AL_KAPAK_AC;
        } else {
          rgbSari();
          bipKisa();
          durum = BIRAK_TAG_BEKLE;
        }

      } else {
        kayitYaz(F("Yetkisiz kart"), now);
        sonucAyarla(HATA_YETKISIZ, BEKLE, false);
      }
    }

    else if (durum == BIRAK_TAG_BEKLE) {
      if (cihazTagMi(sonUID)) {
        kayitYaz(F("Yetkili tag"), now);

        rgbYesil();
        bipKisa();
        servoAc();
        islemZamaniBaslat();
        durum = BIRAK_KAPAK_AC;
      } else {
        kayitYaz(F("Yetkisiz tag"), now);
        sonucAyarla(HATA_TAG, BIRAK_TAG_BEKLE, false);
      }
    }

    else if (durum == AL_TAG_BEKLE) {
      if (cihazTagMi(sonUID)) {
        kayitYaz(F("Yetkili tag"), now);

        rgbSari();
        bipKisa();
        alTagDogrulandi = true;
        durum = AL_KAPAK_KAPAT;
      } else {
        kayitYaz(F("Yetkisiz tag"), now);
        sonucAyarla(HATA_TAG, AL_TAG_BEKLE, false);
      }
    }
  }

  // Birakma akisi
  if (durum == BIRAK_KAPAK_AC) {
    if (!kapakKapali) {
      durum = BIRAK_KAPAK_KAPAT;
      rgbSari();
    } else if (islemSuresiDolduMu()) {
      kayitYaz(F("Sure doldu"), now);
      kritikHataBaslat(BIRAK_HATA_TIMEOUT);
    }
  }

  else if (durum == BIRAK_KAPAK_KAPAT) {
    if (kapakKapali) {
      if (cihazVar && agirlikNormalMi()) {
        servoKapat();
        kayitYaz(F("Cihaz birakildi"), now);
        sonucAyarla(SONUC_BIRAKILDI, BEKLE, true);
      } else if (cihazVar && !agirlikNormalMi()) {
        kayitYaz(F("Agirlik hatasi"), now);
        servoAc();
        kritikHataBaslat(BIRAK_HATA_AGIRLIK_FAZLA);
      } else {
        kayitYaz(F("Cihaz konmadi"), now);
        servoAc();
        kritikHataBaslat(BIRAK_HATA_CIHAZ_YOK);
      }
    } else if (islemSuresiDolduMu()) {
      kayitYaz(F("Sure doldu"), now);
      kritikHataBaslat(BIRAK_HATA_TIMEOUT);
    }
  }

  // Alma akisi
  else if (durum == AL_KAPAK_AC) {
    if (!kapakKapali) {
      durum = AL_TAG_BEKLE;
      rgbSari();
    } else if (islemSuresiDolduMu()) {
      kayitYaz(F("Sure doldu"), now);
      kritikHataBaslat(AL_HATA_TIMEOUT);
    }
  }

  else if (durum == AL_TAG_BEKLE) {
    if (islemSuresiDolduMu()) {
      kayitYaz(F("Sure doldu"), now);
      kritikHataBaslat(AL_HATA_TIMEOUT);
    }
  }

  else if (durum == AL_KAPAK_KAPAT) {
    if (kapakKapali) {
      if (!cihazVar) {
        servoKapat();
        kayitYaz(F("Cihaz alindi"), now);
        sonucAyarla(SONUC_ALINDI, BEKLE, true);
      } else {
        kayitYaz(F("Cihaz alinmadi"), now);
        servoAc();
        kritikHataBaslat(AL_HATA_CIHAZ_VAR);
      }
    } else if (islemSuresiDolduMu()) {
      kayitYaz(F("Sure doldu"), now);
      kritikHataBaslat(AL_HATA_TIMEOUT);
    }
  }

  if (durum == BEKLE) {
    rgbMavi();
  }

  ekranYaz(now, kapakKapali);

  delay(150);
}
