// Bluetooth ve Wi-Fi modülleri için kullanılacak seri portlar tanımlandı
#define BLUETOOTH_SERIAL Serial1
#define WIFI_SERIAL Serial2

// Buton pinleri
int buton1 = 2; // alt sırt yukarı
int buton2 = 3; // alt sırt aşağı
int buton3 = 4; // alt ayak yukarı
int buton4 = 5; // alt ayak aşağı
int buton5 = 6; // sırt yukarı
int buton6 = 7; // sırt aşağı
int buton7 = 8; // bacak yukarı
int buton8 = 9; // bacak aşağı
int buton9 = 10; // ayak yukarı
int buton10 = 11; // ayak aşağı

// Motor pinleri
int altsirtyukari = 30; // alt sırt yukarı
int altsirtasagi = 31; // alt sırt aşağı
int altayakyukari = 32; // alt ayak yukarı
int altayakasagi = 33; // alt ayak aşağı
int sirtyukari = 34; // sırt yukarı
int sirtasagi = 35; // sırt aşağı
int bacakyukari = 36; // bacak yukarı
int bacakasagi = 37; // bacak aşağı
int ayakyukari = 38; // ayak yukarı
int ayakasagi = 39; // ayak aşağı

// Diğer pinler
int enable = 8; // Enable pini
int sayici = 0; // Sayaç

String veri = ""; // Gelen veri için string değişkeni

void setup() {
  // Ana seri portu başlat (USB üzerinden bilgisayara)
  Serial.begin(9600); 
  BLUETOOTH_SERIAL.begin(9600); // Bluetooth için donanımsal seri port
  WIFI_SERIAL.begin(9600); // Wi-Fi için donanımsal seri port

  // Seri port üzerinden debug mesajı gönder
  Serial.println("Setup başlatıldı");

  // Buton pinlerini giriş olarak ayarla
  pinMode(buton1, INPUT);
  pinMode(buton2, INPUT);
  pinMode(buton3, INPUT);
  pinMode(buton4, INPUT);
  pinMode(buton5, INPUT);
  pinMode(buton6, INPUT);
  pinMode(buton7, INPUT);
  pinMode(buton8, INPUT);
  pinMode(buton9, INPUT);
  pinMode(buton10, INPUT);

  // Motor pinlerini çıkış olarak ayarla
  pinMode(altsirtyukari, OUTPUT);
  pinMode(altsirtasagi, OUTPUT);
  pinMode(altayakyukari, OUTPUT);
  pinMode(altayakasagi, OUTPUT);
  pinMode(sirtyukari, OUTPUT);
  pinMode(sirtasagi, OUTPUT);
  pinMode(bacakyukari, OUTPUT);
  pinMode(bacakasagi, OUTPUT);
  pinMode(ayakyukari, OUTPUT);
  pinMode(ayakasagi, OUTPUT);
  pinMode(enable, OUTPUT);

  Serial.println("Setup tamamlandı");
}

void loop() {
  digitalWrite(enable, LOW); // Enable pini düşük seviyeye çek

  // Bluetooth'tan veri oku
  if (BLUETOOTH_SERIAL.available()) {
    Serial.println("Bluetooth verisi mevcut");
    while (BLUETOOTH_SERIAL.available()) {
      char c = (char)BLUETOOTH_SERIAL.read();
      veri += c;
      delay(25);
    }
  }

  // Wi-Fi'dan veri oku
  if (WIFI_SERIAL.available()) {
    Serial.println("Wi-Fi verisi mevcut");
    while (WIFI_SERIAL.available()) {
      char c = (char)WIFI_SERIAL.read();
      veri += c;
      delay(25);
    }
  }

  if (veri.length() > 0) {
    Serial.print("Gelen veri: ");
    Serial.println(veri);
  }

  // Gelen komutları kontrol et ve motorları çalıştır/durdur
  kontrolMotor("alt sirt yukari", altsirtyukari);
  kontrolMotor("alt sirt asagi", altsirtasagi);
  kontrolMotor("alt ayak yukari", altayakyukari);
  kontrolMotor("alt ayak asagi", altayakasagi);
  kontrolMotor("sirt yukari", sirtyukari);
  kontrolMotor("sirt asagi", sirtasagi);
  kontrolMotor("bacak yukari", bacakyukari);
  kontrolMotor("bacak asagi", bacakasagi);
  kontrolMotor("ayak yukari", ayakyukari);
  kontrolMotor("ayak asagi", ayakasagi);

  // Gövde kontrolü
  if (veri == "yatak yukari") {
    Serial.println("Komut: yatak yukari");
    kontrolCiftMotor(altsirtyukari, altayakyukari);
  }
  if (veri == "yatak asagi") {
    Serial.println("Komut: yatak asagi");
    kontrolCiftMotor(altsirtasagi, altayakasagi);
  }

  // Buton kontrolü
  kontrolButon(buton1, buton2, altsirtyukari, altsirtasagi);
  kontrolButon(buton3, buton4, altayakyukari, altayakasagi);
  kontrolButon(buton5, buton6, sirtyukari, sirtasagi);
  kontrolButon(buton7, buton8, bacakyukari, bacakasagi);
  kontrolButon(buton9, buton10, ayakyukari, ayakasagi);

  veri = ""; // Gelen veriyi sıfırla
  delay(50);
  sayici = (sayici + 1);
  if (sayici == 1000 && Serial.available() == 0) {
    digitalWrite(enable, HIGH);
    delay(250);
    digitalWrite(enable, LOW);
    sayici = 0;
  }
}

// Motor kontrol fonksiyonu
void kontrolMotor(String komut, int pin) {
  if (veri == komut) {
    Serial.print("Komut: ");
    Serial.println(komut);
    veri = "";
    while (1) {
      digitalWrite(pin, HIGH);
      if (BLUETOOTH_SERIAL.available()) {
        char c = (char)BLUETOOTH_SERIAL.read();
        veri += c;
        delay(25);
      }
      if (WIFI_SERIAL.available()) {
        char c = (char)WIFI_SERIAL.read();
        veri += c;
        delay(25);
      }
      if (veri != "dur" && veri != "") {
        veri = "";
        digitalWrite(pin, LOW);
        Serial.print("Motor durduruluyor: ");
        Serial.println(pin);
        break;
      }
      if (veri == "dur") {
        digitalWrite(pin, LOW);
        Serial.print("Motor durduruluyor: ");
        Serial.println(pin);
        break;
      }
    }
  }
}

// Çift motor kontrol fonksiyonu
void kontrolCiftMotor(int pin1, int pin2) {
  veri = "";
  while (1) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    if (BLUETOOTH_SERIAL.available()) {
      char c = (char)BLUETOOTH_SERIAL.read();
      veri += c;
      delay(25);
    }
    if (WIFI_SERIAL.available()) {
      char c = (char)WIFI_SERIAL.read();
      veri += c;
      delay(25);
    }
    if (veri != "dur" && veri != "") {
      veri = "";
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      Serial.print("Çift motor durduruluyor: ");
      Serial.print(pin1);
      Serial.print(" ve ");
      Serial.println(pin2);
      break;
    }
    if (veri == "dur") {
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      Serial.print("Çift motor durduruluyor: ");
      Serial.print(pin1);
      Serial.print(" ve ");
      Serial.println(pin2);
      break;
    }
  }
}

// Buton kontrol fonksiyonu
void kontrolButon(int butonYukari, int butonAsagi, int motorYukari, int motorAsagi) {
  if ((digitalRead(butonYukari) == HIGH) && (digitalRead(butonAsagi) == LOW)) {
    digitalWrite(motorYukari, HIGH);
    Serial.print("Buton: ");
    Serial.print(butonYukari);
    Serial.print(" Motor yukari: ");
    Serial.println(motorYukari);
  } else {
    digitalWrite(motorYukari, LOW);
  }

  if ((digitalRead(butonAsagi) == HIGH) && (digitalRead(butonYukari) == LOW)) {
    digitalWrite(motorAsagi, HIGH);
    Serial.print("Buton: ");
    Serial.print(butonAsagi);
    Serial.print(" Motor asagi: ");
    Serial.println(motorAsagi);
  } else {
    digitalWrite(motorAsagi, LOW);
  }
}
