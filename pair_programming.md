# 👥 Pair Programming ve Yapay Zeka Destekli Geliştirme Süreci

## Genel Bakış

Bu projede, **pair programming (eşli programlama)** yaklaşımı benimsenmiş ve geliştirme süreci öğrenci ile yapay zeka asistanının birlikte çalışması şeklinde yürütülmüştür.

Projenin amacı; Arduino tabanlı fiziksel programlama bileşenlerini bir araya getirerek RFID kontrollü, load cell ile cihaz var/yok durumunu algılayan, RTC ile saatli kayıt tutan ve OLED, RGB LED, buzzer ile kullanıcıya geri bildirim veren işlevsel bir güvenlik kutusu geliştirmektir.

Pair programming süreci; donanım seçimi, bağlantı planı, kod geliştirme, hata ayıklama, test senaryoları ve dokümantasyon aşamalarında kullanılmıştır.

---

## Pair Programming Yaklaşımı

Geliştirme sürecinde roller şu şekilde uygulanmıştır:

- **Sürücü (Driver):** Devre bağlantılarını yapan, Arduino IDE üzerinde kodu yükleyen, fiziksel montajı gerçekleştiren ve testleri uygulayan öğrenci
- **Gözlemci / Rehber (Navigator):** Sistem akışını planlayan, kod mantığını düzenlemeye yardımcı olan, hata durumlarını yorumlayan ve çözüm önerileri sunan yapay zeka asistanı

Bu yapı sayesinde:

- Proje adım adım ve kontrollü şekilde ilerlemiştir
- Modüller önce ayrı ayrı test edilmiş, daha sonra tek sistemde birleştirilmiştir
- Hatalar daha erken fark edilmiştir
- Final kod daha düzenli ve okunabilir hale getirilmiştir
- Test senaryoları sistemli şekilde hazırlanmıştır

---

## Yapay Zeka Asistanının Rolü

Yapay zeka asistanı aşağıdaki alanlarda aktif olarak kullanılmıştır:

- Proje fikrinin netleştirilmesi
- Kullanılacak donanımların seçilmesi
- Gereksiz sensörlerin projeden çıkarılması
- RFID, RTC, OLED, HX711, servo, buzzer ve RGB LED bağlantılarının planlanması
- Arduino kodunun aşama aşama geliştirilmesi
- Hata mesajlarının yorumlanması
- Load cell eşik değerlerinin belirlenmesi
- Açılışta yanlış ağırlık algılama probleminin çözülmesi
- OLED ekran arayüzünün sadeleştirilmesi
- Serial Monitor kayıt sisteminin düzenlenmesi
- Final test listesinin hazırlanması
- README, rapor, video ve mühendislik tasarım defteri içeriklerinin düzenlenmesi

---

## Somut Katkı Örnekleri

Geliştirme sürecinde yapay zeka asistanı ile birlikte aşağıdaki kararlar alınmıştır:

- Cihaz var/yok ayrımı için **3000 eşik değeri** belirlenmiştir.
- 75000 üstü değerler **ağırlık hatası** olarak kabul edilmiştir.
- Açılışta otomatik tare işlemi yerine **sabit boş referans değeri** kullanılmasına karar verilmiştir.
- Boş ham referans değeri yaklaşık **-268500** olarak belirlenmiştir.
- Serial Monitor’da sürekli veri akışı yerine yalnızca önemli olayların kayıt olarak yazılması sağlanmıştır.
- OLED ekranda karmaşık veriler yerine kısa durum mesajları ve robot yüz ifadeleri kullanılmıştır.
- Yetkili kart, yetkisiz kart, admin kart ve cihaz tagı için ayrı sistem akışları oluşturulmuştur.
- Hata senaryoları ayrı ayrı test edilmiştir.

---

## Sürece Katkılar

Pair programming yaklaşımı sayesinde:

- Proje geliştirme süreci daha planlı ilerlemiştir
- Donanım ve yazılım entegrasyonu daha kontrollü yapılmıştır
- Kodun hata yönetimi güçlendirilmiştir
- Sistem final aşamada daha kararlı hale getirilmiştir
- Dokümantasyon, video ve GitHub içerikleri daha düzenli hazırlanmıştır

---

## Değerlendirme

Bu süreç, BOZ214 Fiziksel Programlama dersi kapsamında beklenen fiziksel sistem geliştirme, sensör/aktüatör entegrasyonu, hata ayıklama ve proje dokümantasyonu hedefleriyle uyumludur.

Yapay zeka asistanı, proje boyunca doğrudan kodu tek başına üreten bir araç olarak değil; fikir geliştirme, teknik rehberlik, hata analizi ve akademik dokümantasyon desteği sağlayan bir geliştirme ortağı olarak kullanılmıştır.
