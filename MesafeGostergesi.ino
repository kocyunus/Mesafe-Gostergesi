/*
MESAFE SEVİYE GÖSTERGESİ - ANİMASYONSUZ
=======================================

Çalışma Mantığı:
- 20cm ve uzak: Tüm LED'ler AÇIK (8 sıra)
- 17.5cm: 7 sıra açık (üst 1 sıra söner)
- 15cm: 6 sıra açık (üst 2 sıra söner)
- 12.5cm: 5 sıra açık (üst 3 sıra söner)
- 10cm: 4 sıra açık (üst 4 sıra söner)
- 7.5cm: 3 sıra açık (üst 5 sıra söner)
- 5cm ve yakın: Sadece 1 sıra açık (en alt)

BAĞLANTI:
=========
MAX7219 Matrix: VCC->5V, GND->GND, DIN->11, CS->10, CLK->13
HC-SR04: VCC->5V, GND->GND, Trig->7, Echo->8
*/

#include <LedControl.h>

// Pin tanımları
#define TRIG_PIN 7
#define ECHO_PIN 8
#define DATA_PIN 11
#define CS_PIN 10
#define CLK_PIN 13

// LED Matrix
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1);

// Mesafe seviyeleri (cm) - 20'den 5'e kadar 8 seviye
float distanceLevels[8] = {20.0, 17.5, 15.0, 12.5, 10.0, 7.5, 5.0, 2.5};

void setup() {
  Serial.begin(9600);
  
  // Sensor pinleri
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Matrix ayarları
  lc.shutdown(0, false);         // Matrix'i aktif et
  lc.setIntensity(0, 15);        // Video için maksimum parlaklık (0-15)
  lc.clearDisplay(0);            // Temizle
  
  // Parlaklık testi
  delay(500);
  for (int brightness = 0; brightness <= 15; brightness++) {
    lc.setIntensity(0, brightness);
    lc.setLed(0, 0, 0, true);  // Test LED'i
    delay(100);
  }
  lc.clearDisplay(0);
  lc.setIntensity(0, 15);  // Maksimum parlaklığa geri dön       
  
  Serial.println("=== MESAFE SEVİYE GÖSTERGESİ ===");
  Serial.println("Parlaklık: MAKSIMUM (15) - Video için");
  Serial.println("20cm: 8 sütun | 5cm: 1 sütun");
  Serial.println("================================");
}

void loop() {
  // Mesafe ölç
  float distance = measureDistance();
  
  // Kaç sütun yanacağını hesapla
  int colsToShow = calculateRows(distance);
  
  // LED'leri güncelle
  updateMatrix(colsToShow);
  
  // Serial çıktı
  Serial.print("Mesafe: ");
  Serial.print(distance, 1);
  Serial.print(" cm -> ");
  Serial.print(colsToShow);
  Serial.println(" sütun");
  
  delay(200); // Yavaş güncelleme - flicker önleme
}

// Mesafe ölçüm - STABIL OKUMA
float measureDistance() {
  // 3 ölçüm al, ortancasını kullan (noise filtreleme)
  float readings[3];
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH);
    readings[i] = duration * 0.034 / 2.0;
    
    if (readings[i] >= 400 || readings[i] <= 1) {
      readings[i] = 999;
    }
    
    delay(10); // Ölçümler arası kısa bekleme
  }
  
  // Sıralama (bubble sort)
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2 - i; j++) {
      if (readings[j] > readings[j + 1]) {
        float temp = readings[j];
        readings[j] = readings[j + 1];
        readings[j + 1] = temp;
      }
    }
  }
  
  return readings[1]; // Ortanca değer
}

// Mesafeye göre kaç sütun yanacağını hesapla
int calculateRows(float distance) {
  // 20cm ve üzeri: Tüm sütunlar (8)
  if (distance >= 20.0 || distance == 999) {
    return 8;
  }
  
  // 5cm altı: Hiç sütun yok
  if (distance < 2.5) {
    return 0;
  }
  
  // Hangi seviyede olduğunu bul
  for (int i = 0; i < 8; i++) {
    if (distance >= distanceLevels[i]) {
      return 8 - i; // Sütun sayısı
    }
  }
  
  return 1; // En az 1 sütun
}

// Matrix'i güncelle - FLICKER ÖNLEME
void updateMatrix(int colsToShow) {
  // Önceki durumla aynıysa hiçbir şey yapma
  static int lastColsShown = -1;
  
  if (colsToShow == lastColsShown) {
    return; // Değişiklik yok, matrix'i boşuna güncelleme
  }
  
  // Sadece değişen sütunları güncelle
  if (colsToShow > lastColsShown) {
    // LED ekle
    for (int col = lastColsShown; col < colsToShow; col++) {
      if (col >= 0) {
        for (int row = 0; row < 8; row++) {
          lc.setLed(0, row, col, true);
        }
      }
    }
  } else {
    // LED kaldır
    for (int col = colsToShow; col < lastColsShown; col++) {
      for (int row = 0; row < 8; row++) {
        lc.setLed(0, row, col, false);
      }
    }
  }
  
  lastColsShown = colsToShow;
}

/*
MESAFE - SIRA TABLOSU:
=====================
20.0cm ve üzeri -> 8 sıra (tümü açık)
17.5cm -> 7 sıra
15.0cm -> 6 sıra  
12.5cm -> 5 sıra
10.0cm -> 4 sıra
7.5cm  -> 3 sıra
5.0cm  -> 2 sıra
2.5cm  -> 1 sıra
2.5cm altı -> 0 sıra (tümü kapalı)

ÖZELLİKLER:
===========
- Animasyon YOK
- Flicker YOK  
- Açık LED'ler sürekli açık
- Smooth seviye geçişi
- Serial'de mesafe + sıra sayısı

AYAR DEĞİŞTİRME:
================
Mesafe aralığını değiştirmek için:
distanceLevels[] dizisindeki değerleri değiştirin

Örnek: 30cm'den 10cm'ye:
float distanceLevels[8] = {30.0, 27.5, 25.0, 22.5, 20.0, 17.5, 15.0, 12.5};
*/