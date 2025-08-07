# Mesafe-Gostergesi
# Arduino Mesafe Sensörü ve 8x8 LED Matrix Projesi

Arduino Uno, ultrasonik sensör (HC-SR04) ve 8x8 LED Matrix kullanarak gerçek zamanlı mesafe görselleştirme sistemi. Robotik projeler, yakınlık algılama ve eğitim demonstrasyonları için mükemmel.

## 🌟 Özellikler

- **Gerçek Zamanlı Mesafe Görselleştirmesi**: LED sütunları mesafe seviyelerini temsil eder (20cm - 5cm arası)
- **Yumuşak Seviye Geçişleri**: Titreşim yok, stabil LED görüntüsü
- **Yüksek Parlaklık**: Video kayıt ve demonstrasyon için optimize edilmiş
- **Seri Port İzleme**: Gerçek zamanlı mesafe okumaları ve sütun sayısı gösterimi
- **Anti-Flicker Teknolojisi**: Akıllı güncelleme algoritması LED titreşimini önler
- **Stabil Ölçümler**: Doğru sonuçlar için 3 okumadan medyan filtreleme

## 🎬 Demo

![Mesafe Sensörü Demo](demo.gif)

*Nesneler sensöre yaklaştıkça LED sütunları azalır*

## 🛠 Donanım Gereksinimleri

### Bileşenler
- **Arduino Uno** (veya uyumlu)
- **HC-SR04 Ultrasonik Sensör**
- **MAX7219 8x8 LED Matrix Modülü**
- **Breadboard ve jumper kablolar**
- **Programlama ve güç için USB Kablosu**

### Opsiyonel Bileşenler
- **5V Harici Güç Kaynağı** (stabil çalışma için önerilen)
- **Buzzer** (ses geri bildirimi için - kod hazır)

## 📋 Devre Şeması

### MAX7219 LED Matrix Bağlantıları
| Matrix Pin | Arduino Pin | Açıklama |
|------------|-------------|----------|
| VCC        | 5V          | Güç Beslemesi |
| GND        | GND         | Toprak |
| DIN        | 11          | Veri Girişi |
| CS         | 10          | Chip Seçici |
| CLK        | 13          | Saat |

### HC-SR04 Ultrasonik Sensör Bağlantıları
| Sensör Pin | Arduino Pin | Açıklama |
|------------|-------------|----------|
| VCC        | 5V          | Güç Beslemesi |
| GND        | GND         | Toprak |
| Trig       | 7           | Tetik |
| Echo       | 8           | Yankı |

## 🔧 Kurulum

### 1. Arduino IDE Kurulumu
```bash
# Arduino IDE'yi https://www.arduino.cc/en/software adresinden indirin
```

### 2. Kütüphane Kurulumu
1. Arduino IDE'yi açın
2. **Sketch → Include Library → Manage Libraries** menüsüne gidin
3. "**LedControl**" arayın ve kurun
4. Bu kütüphane MAX7219 LED matrix iletişimini yönetir

### 3. Kodu Yükleme
1. Arduino Uno'yu USB ile bilgisayara bağlayın
2. `distance_sensor.ino` dosyasını Arduino IDE'de açın
3. **Board: Arduino Uno** ve doğru **Port**'u seçin
4. **Upload** butonuna tıklayın

## 📊 Nasıl Çalışır

### Mesafe Eşleştirmesi
| Mesafe Aralığı | LED Sütunları | Görsel Temsil |
|----------------|---------------|---------------|
| 20cm ve üzeri  | 8 sütun       | `████████` |
| 17.5cm         | 7 sütun       | `███████░` |
| 15.0cm         | 6 sütun       | `██████░░` |
| 12.5cm         | 5 sütun       | `█████░░░` |
| 10.0cm         | 4 sütun       | `████░░░░` |
| 7.5cm          | 3 sütun       | `███░░░░░` |
| 5.0cm          | 2 sütun       | `██░░░░░░` |
| 2.5cm          | 1 sütun       | `█░░░░░░░` |
| 2.5cm altı     | 0 sütun       | `░░░░░░░░` |

### Teknik Detaylar
- **Ölçüm Aralığı**: 2cm - 400cm (HC-SR04 spesifikasyonu)
- **Görüntü Aralığı**: 2.5cm - 20cm için optimize edilmiş
- **Güncelleme Hızı**: Stabil görüntü için 5Hz (200ms aralıklar)
- **Doğruluk**: Yakın mesafede ±3mm
- **Anti-Flicker**: Akıllı LED güncelleme - sadece değişen sütunları günceller

## 🚀 Kullanım

### Temel İşletim
1. Arduino'yu açın
2. **Serial Monitor**'u açın (9600 baud rate)
3. Sensörün önüne nesneler yerleştirin (2-20cm arası)
4. LED sütunlarının gerçek zamanlı değişimini izleyin

### Serial Monitor Çıktısı
```
=== MESAFE SEVİYE GÖSTERGESİ ===
Parlaklık: MAKSIMUM (15) - Video için
20cm: 8 sütun | 5cm: 1 sütun
================================
Mesafe: 15.2 cm -> 6 sütun
Mesafe: 12.8 cm -> 5 sütun  
Mesafe: 8.1 cm -> 3 sütun
```

## ⚙️ Yapılandırma

### Mesafe Aralıklarını Ayarlama
Koddaki `distanceLevels[]` dizisini değiştirin:
```cpp
// Varsayılan: 20cm'den 2.5cm'ye 8 adımda
float distanceLevels[8] = {20.0, 17.5, 15.0, 12.5, 10.0, 7.5, 5.0, 2.5};

// Örnek: 30cm'den 10cm'ye aralık
float distanceLevels[8] = {30.0, 27.5, 25.0, 22.5, 20.0, 17.5, 15.0, 12.5};
```

### LED Parlaklığını Ayarlama
Farklı ortamlar için parlaklığı değiştirin:
```cpp
// Maksimum parlaklık (video kayıt)
lc.setIntensity(0, 15);

// Orta parlaklık (normal oda)
lc.setIntensity(0, 12);

// Düşük parlaklık (karanlık ortam)
lc.setIntensity(0, 8);
```

### Güncelleme Hızını Ayarlama
Farklı tepki hızları için delay'i değiştirin:
```cpp
delay(200);  // Varsayılan: stabil, titreşim yok
delay(100);  // Hızlı tepki (titreşim olabilir)
delay(300);  // Ekstra stabil (yavaş tepki)
```


## 🐛 Sorun Giderme

### LED Matrix Sorunları
| Sorun | Çözüm |
|-------|--------|
| LED'ler yanmıyor | Güç bağlantılarını kontrol edin (VCC, GND) |
| Solgun LED'ler | Parlaklığı artırın: `lc.setIntensity(0, 15)` |
| Yanlış pattern | DIN, CS, CLK bağlantılarını doğrulayın |

### Sensör Sorunları
| Sorun | Çözüm |
|-------|--------|
| Kararsız okumalar | Trig/Echo bağlantılarını kontrol edin |
| Mesafe okumuyor | Sensöre 5V güç verildiğini doğrulayın |
| Yanlış mesafeler | Sensörün düz yüzeye baktığından emin olun |

### Titreşim Sorunları
| Sorun | Çözüm |
|-------|--------|
| LED titreşimi | Stabil güç kaynağı kullanın (harici 5V) |
| Kararsız okumalar | Test nesnesini sabit tutun |
| Hızlı değişimler | Delay'i 300ms'ye çıkarın |

## 📈 Gelecek Geliştirmeler

- [ ] **Radar Modu**: 180° tarama için servo motor ekleme
- [ ] **Çoklu Sensör**: Birden fazla HC-SR04 sensör desteği
- [ ] **Kablosuz**: WiFi bağlantılı ESP32/ESP8266 versiyonu
- [ ] **Veri Kayıt**: SD kart kayıt özelliği
- [ ] **Web Arayüzü**: Gerçek zamanlı web gösterge paneli
- [ ] **Ses Efektleri**: Yakınlığa dayalı ses geri bildirimi

## 🤝 Katkıda Bulunma

Katkılar memnuniyetle karşılanır! Lütfen Pull Request göndermekten çekinmeyin.

### Geliştirme Kurulumu
1. Repository'yi fork edin
2. Feature branch'inizi oluşturun (`git checkout -b feature/HarikaBirOzellik`)
3. Değişikliklerinizi commit edin (`git commit -m 'Harika bir özellik ekle'`)
4. Branch'i push edin (`git push origin feature/HarikaBirOzellik`)
5. Pull Request açın

## 📄 Lisans

Bu proje MIT Lisansı altında lisanslanmıştır - detaylar için [LICENSE](LICENSE) dosyasına bakın.

## 🙏 Teşekkürler

- **Arduino Topluluğu**'na mükemmel dokümantasyon için
- **LedControl Kütüphanesi** Eberhard Fahle'ye MAX7219 desteği için
- **HC-SR04 Sensör** üreticilerine güvenilir mesafe algılama için

## 📞 İletişim

- **GitHub**: [@kullaniciadin](https://github.com/kullaniciadin)
- **Email**: senin.mailin@ornek.com

---

⭐ **Bu repository'yi faydalı bulduysanız yıldızlamayı unutmayın!**

#Arduino #UltrasonikSensör #LEDMatrix #MesafeSensörü #Elektronik #DIY #Türkiye
