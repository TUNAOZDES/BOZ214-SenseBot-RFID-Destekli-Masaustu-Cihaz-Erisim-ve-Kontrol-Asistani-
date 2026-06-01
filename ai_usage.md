# 🧠 Yapay Zeka Asistanı Kullanımı

Bu projede yapay zeka asistanı, SenseBot’un geliştirme sürecinin farklı aşamalarında aktif olarak kullanılmıştır. Asistan yalnızca soru-cevap aracı olarak değil; planlama, hata ayıklama, kod düzenleme, test senaryosu hazırlama ve dokümantasyon desteği sağlayan bir geliştirme yardımcısı olarak değerlendirilmiştir.

---

## Kullanım Alanları

Yapay zeka asistanı aşağıdaki alanlarda etkin şekilde kullanılmıştır:

- Proje fikrinin netleştirilmesi
- Kullanılacak donanımların seçilmesi
- Gereksiz sensörlerin projeden çıkarılması
- Devre bağlantılarının planlanması
- Arduino kodunun aşama aşama geliştirilmesi
- Modüllerin ayrı ayrı test edilmesi
- Kod entegrasyonu ve hata ayıklama
- Load cell eşik değerlerinin belirlenmesi
- RFID kart/tag senaryolarının düzenlenmesi
- Admin kart akışının oluşturulması
- OLED ekran tasarımının iyileştirilmesi
- Serial Monitor kayıt sisteminin sadeleştirilmesi
- Final test listesinin hazırlanması
- GitHub README dosyasının hazırlanması
- Proje raporu, video metni ve mühendislik tasarım defteri içeriklerinin düzenlenmesi

---

## Somut Katkı Örnekleri

Yapay zeka asistanı ile yapılan çalışmalar sonucunda aşağıdaki somut geliştirmeler yapılmıştır:

### 1. Sistem Akışının Planlanması

Başlangıçta proje yalnızca RFID ile açılan bir kutu fikrine daha yakındı. Daha sonra cihazın gerçekten bırakılıp bırakılmadığını kontrol etmek için load cell kullanılması planlandı.

### 2. Sensör Seçiminin Netleştirilmesi

PIR, LDR ve sıcaklık sensörü gibi parçalar ilk aşamada düşünülmüş, ancak projenin temel amacına doğrudan katkı sağlamadığı için sistemden çıkarılmıştır.

### 3. RFID Kart ve Tag Mantığı

Yetkili kart, yetkisiz kart, admin kart ve cihaz tagı için ayrı roller belirlenmiştir. Böylece sistem yalnızca kullanıcıyı değil, cihazı da doğrulayacak hale getirilmiştir.

### 4. Load Cell Eşiklerinin Belirlenmesi

Testlerde boş platform, hafif cihaz ve ağır nesneler için değerler gözlenmiştir. Bu ölçümlere göre:

```text
3000 ve altı  -> Cihaz yok
3000 üstü     -> Cihaz var
75000 üstü    -> Ağırlık hatası
```

eşikleri belirlenmiştir.

### 5. Açılışta Yanlış Algılama Probleminin Çözülmesi

Arduino açılırken otomatik tare yapılması, cihaz platformdayken sistemin yanlış başlangıç yapmasına sebep olmuştur. Bu sorun, sabit boş referans değeri kullanılarak çözülmüştür.

Final boş referans değeri:

```text
-268500
```

### 6. OLED Arayüzünün Geliştirilmesi

İlk denemelerde OLED’de saat, kapak durumu, ağırlık ve cihaz bilgisi birlikte gösterilmiştir. Daha sonra ekranın sadeleşmesi için kısa durum mesajları ve robot yüz ifadeleri kullanılmasına karar verilmiştir.

### 7. Serial Monitor Kayıt Sistemi

İlk denemelerde sürekli teknik veri yazdırılırken, finalde yalnızca önemli olayların saat bilgisiyle kayıt olarak yazdırılması sağlanmıştır.

Örnek:

```text
KAYIT | Cihaz birakildi | Saat: 18:45:12
KAYIT | Cihaz alindi | Saat: 18:48:30
KAYIT | Yetkisiz kart | Saat: 18:51:10
```

---

## Geliştirme Sürecine Etkisi

Yapay zeka asistanı ile kurulan etkileşim sayesinde:

- Kod geliştirme süreci daha planlı ilerlemiştir
- Hatalar daha hızlı yorumlanmıştır
- Testler sistematik hale getirilmiştir
- Donanım ve yazılım entegrasyonu daha kontrollü yapılmıştır
- Proje dokümantasyonu daha kapsamlı hazırlanmıştır
- GitHub sayfası ve README dosyası daha düzenli hale getirilmiştir

---

## Sınırlılıklar

Yapay zeka asistanı fiziksel devreyi doğrudan test etmemiştir. Tüm bağlantılar, ölçümler, montaj işlemleri ve gerçek donanım testleri öğrenci tarafından yapılmıştır.

Asistanın önerileri, gerçek test sonuçlarına göre değerlendirilmiş ve gerekli durumlarda değiştirilmiştir. Bu nedenle proje sürecinde son kararlar fiziksel testlere göre verilmiştir.

---

## Sonuç

Yapay zeka asistanı, SenseBot projesinde aktif bir geliştirme ortağı olarak kullanılmıştır. Projenin planlama, kodlama, hata çözme, test ve dokümantasyon aşamalarında katkı sağlamıştır.

Bu kullanım, BOZ214 Fiziksel Programlama dersi kapsamında geliştirilen projenin teknik ve akademik kalitesini artırmıştır.
