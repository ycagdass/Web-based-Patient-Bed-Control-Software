/*
* Hasta Yatağı Kontrol Sistemi
* Yazar: ycagdass
* Tarih: 2025-01-05
* 
* Bu kod ESP32 için hasta yatağı kontrol sistemi içindir.
* WiFi üzerinden web arayüzü ve fiziksel butonlarla kontrol sağlar.
*/

#include <WiFi.h>
#include <WebServer.h>

// HTML sayfası tanımı
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang='tr'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Hasta Yatağı Kontrol</title>
    <style>
        body {
            background: linear-gradient(135deg, #6a1b9a, #8e24aa);
            font-family: Arial, sans-serif;
            color: #fff;
            text-align: center;
            padding: 20px;
            margin: 0;
        }
        h1 {
            font-size: 2.5rem;
            margin-bottom: 30px;
            text-shadow: 0 4px 10px rgba(0,0,0,0.5);
        }
        #status {
            background-color: rgba(0,0,0,0.2);
            padding: 10px;
            border-radius: 5px;
            margin: 10px 0;
            transition: all 0.3s ease;
        }
        #status.active {
            background-color: rgba(0,255,0,0.2);
        }
        .control-row {
            display: flex;
            justify-content: center;
            align-items: center;
            margin: 20px 0;
            flex-wrap: wrap;
        }
        .button {
            display: inline-block;
            margin: 10px;
            padding: 20px 30px;
            font-size: 24px;
            color: #fff;
            background-color: #5e35b1;
            border: none;
            border-radius: 50%;
            cursor: pointer;
            transition: all 0.3s;
            box-shadow: 0 4px 10px rgba(0,0,0,0.3);
            user-select: none;
            -webkit-tap-highlight-color: transparent;
        }
        .button:hover {
            background-color: #7e57c2;
            transform: scale(1.1);
        }
        .button:active {
            transform: scale(0.95);
            background-color: #4527a0;
        }
        .label {
            font-size: 20px;
            margin: 0 15px;
            text-shadow: 0 2px 5px rgba(0,0,0,0.4);
            width: 100px;
        }
    </style>
</head>
<body>
    <h1>Hasta Yatağı Kontrol</h1>
    <div id="status">Durum: Hazır</div>
    <div id="connection">Bağlantı: Kontrol Ediliyor...</div>

    <!-- Alt Sırt -->
    <div class='control-row'>
        <button class='button' onmousedown='sendCommand("alt sirt yukari")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("alt sirt yukari")' ontouchend='sendCommand("dur")'>&#x2191;</button>
        <span class='label'>Alt Sırt</span>
        <button class='button' onmousedown='sendCommand("alt sirt asagi")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("alt sirt asagi")' ontouchend='sendCommand("dur")'>&#x2193;</button>
    </div>

    <!-- Alt Ayak -->
    <div class='control-row'>
        <button class='button' onmousedown='sendCommand("alt ayak yukari")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("alt ayak yukari")' ontouchend='sendCommand("dur")'>&#x2191;</button>
        <span class='label'>Alt Ayak</span>
        <button class='button' onmousedown='sendCommand("alt ayak asagi")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("alt ayak asagi")' ontouchend='sendCommand("dur")'>&#x2193;</button>
    </div>

    <!-- Sırt -->
    <div class='control-row'>
        <button class='button' onmousedown='sendCommand("sirt yukari")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("sirt yukari")' ontouchend='sendCommand("dur")'>&#x2191;</button>
        <span class='label'>Sırt</span>
        <button class='button' onmousedown='sendCommand("sirt asagi")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("sirt asagi")' ontouchend='sendCommand("dur")'>&#x2193;</button>
    </div>

    <!-- Bacak -->
    <div class='control-row'>
        <button class='button' onmousedown='sendCommand("bacak yukari")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("bacak yukari")' ontouchend='sendCommand("dur")'>&#x2191;</button>
        <span class='label'>Bacak</span>
        <button class='button' onmousedown='sendCommand("bacak asagi")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("bacak asagi")' ontouchend='sendCommand("dur")'>&#x2193;</button>
    </div>

    <!-- Ayak -->
    <div class='control-row'>
        <button class='button' onmousedown='sendCommand("ayak yukari")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("ayak yukari")' ontouchend='sendCommand("dur")'>&#x2191;</button>
        <span class='label'>Ayak</span>
        <button class='button' onmousedown='sendCommand("ayak asagi")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("ayak asagi")' ontouchend='sendCommand("dur")'>&#x2193;</button>
    </div>

    <!-- Yatak -->
    <div class='control-row'>
        <button class='button' onmousedown='sendCommand("yatak yukari")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("yatak yukari")' ontouchend='sendCommand("dur")'>&#x2191;</button>
        <span class='label'>Yatak</span>
        <button class='button' onmousedown='sendCommand("yatak asagi")' onmouseup='sendCommand("dur")' ontouchstart='sendCommand("yatak asagi")' ontouchend='sendCommand("dur")'>&#x2193;</button>
    </div>

    <script>
        const statusDiv = document.getElementById('status');
        const connectionDiv = document.getElementById('connection');
        let isConnected = true;

        function sendCommand(command) {
            if (!isConnected) {
                statusDiv.textContent = 'Durum: Bağlantı Yok!';
                return;
            }
            
            statusDiv.textContent = 'Durum: ' + command;
            statusDiv.classList.add('active');
            
            fetch('/control?command=' + encodeURIComponent(command))
                .then(response => response.text())
                .then(data => {
                    console.log('Komut:', command, data);
                    if(command === 'dur') {
                        setTimeout(() => {
                            statusDiv.textContent = 'Durum: Hazır';
                            statusDiv.classList.remove('active');
                        }, 500);
                    }
                })
                .catch(error => {
                    console.error('Hata:', error);
                    statusDiv.textContent = 'Durum: Hata!';
                    isConnected = false;
                    connectionDiv.textContent = 'Bağlantı: Kesik';
                    connectionDiv.style.color = '#ff4444';
                });
        }

        setInterval(() => {
            fetch('/').then(() => {
                if (!isConnected) {
                    isConnected = true;
                    connectionDiv.textContent = 'Bağlantı: Aktif';
                    connectionDiv.style.color = '#44ff44';
                }
            }).catch(() => {
                isConnected = false;
                connectionDiv.textContent = 'Bağlantı: Kesik';
                connectionDiv.style.color = '#ff4444';
            });
        }, 5000);
    </script>
</body>
</html>
)=====";

// WiFi ayarları
const char* ssid = "SUPERONLINE_WiFi_27AB";
const char* password = "canpolat_1950";

WebServer server(80);

// Pin tanımlamaları
const int buton1 = 23;   // alt sırt yukarı
const int buton2 = 22;   // alt sırt aşağı
const int buton3 = 21;   // alt ayak yukarı
const int buton4 = 19;   // alt ayak aşağı
const int buton5 = 18;   // sırt yukarı
const int buton6 = 5;    // sırt aşağı
const int buton7 = 4;    // bacak yukarı
const int buton8 = 2;    // bacak aşağı
const int buton9 = 15;   // ayak yukarı
const int buton10 = 13;  // ayak aşağı

// Motor kontrol pinleri
const int altsirtyukari = 32;
const int altsirtasagi = 33;
const int altayakyukari = 25;
const int altayakasagi = 26;
const int sirtyukari = 27;
const int sirtasagi = 14;
const int bacakyukari = 12;
const int bacakasagi = 16;
const int ayakyukari = 17;
const int ayakasagi = 35;
const int enable = 34;

int sayici = 0;
String veri = "";

void durumBildir(const String& durum) {
    Serial.println("----------------------------------------");
    Serial.println("Durum: " + durum);
    Serial.println("----------------------------------------");
    if (WiFi.status() == WL_CONNECTED) {
        server.send(200, "text/plain", durum);
    }
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.println("");
    Serial.println("----------------------------------------");
    Serial.println("WiFi Baglaniyor");
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    Serial.println("");
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("----------------------------------------");
        Serial.println("WiFi Baglandi!");
        Serial.print("IP Adresi: ");
        Serial.println(WiFi.localIP());
        Serial.println("----------------------------------------");
    } else {
        Serial.println("WiFi Baglanti Hatasi!");
        Serial.println("Sistem yeniden baslatiliyor...");
        Serial.println("----------------------------------------");
        ESP.restart();
    }
}

void handleRoot() {
    server.send(200, "text/html", FPSTR(index_html));
}

void handleControl() {
    String command = server.arg("command");
    if (command.length() > 0) {
        veri = command;
        Serial.println("----------------------------------------");
        Serial.println("Web komut alindi: " + command);
        Serial.println("----------------------------------------");
    }
    server.send(200, "text/plain", "Komut alindi");
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("");
    Serial.println("========================================");
    Serial.println("Hasta Yatagi Kontrol Sistemi");
    Serial.println("Sistem Baslatiliyor...");
    Serial.println("========================================");
    
    // Pin modlarını ayarla
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
    
    // Başlangıçta tüm motorları kapat
    digitalWrite(altsirtyukari, LOW);
    digitalWrite(altsirtasagi, LOW);
    digitalWrite(altayakyukari, LOW);
    digitalWrite(altayakasagi, LOW);
    digitalWrite(sirtyukari, LOW);
    digitalWrite(sirtasagi, LOW);
    digitalWrite(bacakyukari, LOW);
    digitalWrite(bacakasagi, LOW);
    digitalWrite(ayakyukari, LOW);
    digitalWrite(ayakasagi, LOW);
    digitalWrite(enable, LOW);
    
    setupWiFi();
    
    server.on("/", handleRoot);
    server.on("/control", handleControl);
    server.begin();
    
    Serial.println("----------------------------------------");
    Serial.println("Web sunucusu baslatildi");
    Serial.println("Sistem hazir!");
    Serial.println("----------------------------------------");
}

void loop() {
    server.handleClient();
    digitalWrite(enable, LOW);

    while (Serial.available()) {
        char c = (char)Serial.read();
        veri += c;
        veri.toLowerCase();
        delay(10);
    }
    
    if(veri.length() > 0) {
        Serial.println("----------------------------------------");
        Serial.println("Alinan Komut: " + veri);
        Serial.println("----------------------------------------");
    }

    // Alt Sırt Yukarı Kontrolü
    if (veri == "alt sirt yukari") {
        veri = "";
        while(1) {
            digitalWrite(altsirtyukari, HIGH);
            durumBildir("Alt sirt yukari hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(altsirtyukari, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(altsirtyukari, LOW);
                break;
            }
        }
    }

    // Alt Sırt Aşağı Kontrolü
    if (veri == "alt sirt asagi") {
        veri = "";
        while(1) {
            digitalWrite(altsirtasagi, HIGH);
            durumBildir("Alt sirt asagi hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(altsirtasagi, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(altsirtasagi, LOW);
                break;
            }
        }
    }

    // Alt Ayak Yukarı Kontrolü
    if (veri == "alt ayak yukari") {
        veri = "";
        while(1) {
            digitalWrite(altayakyukari, HIGH);
            durumBildir("Alt ayak yukari hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(altayakyukari, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(altayakyukari, LOW);
                break;
            }
        }
    }

    // Alt Ayak Aşağı Kontrolü
    if (veri == "alt ayak asagi") {
        veri = "";
        while(1) {
            digitalWrite(altayakasagi, HIGH);
            durumBildir("Alt ayak asagi hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(altayakasagi, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(altayakasagi, LOW);
                break;
            }
        }
    }

    // Sırt Yukarı Kontrolü
    if (veri == "sirt yukari") {
        veri = "";
        while(1) {
            digitalWrite(sirtyukari, HIGH);
            durumBildir("Sirt yukari hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(sirtyukari, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(sirtyukari, LOW);
                break;
            }
        }
    }

    // Sırt Aşağı Kontrolü
    if (veri == "sirt asagi") {
        veri = "";
        while(1) {
            digitalWrite(sirtasagi, HIGH);
            durumBildir("Sirt asagi hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(sirtasagi, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(sirtasagi, LOW);
                break;
            }
        }
    }

    // Bacak Yukarı Kontrolü
    if (veri == "bacak yukari") {
        veri = "";
        while(1) {
            digitalWrite(bacakyukari, HIGH);
            durumBildir("Bacak yukari hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(bacakyukari, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(bacakyukari, LOW);
                break;
            }
        }
    }

    // Bacak Aşağı Kontrolü
    if (veri == "bacak asagi") {
        veri = "";
        while(1) {
            digitalWrite(bacakasagi, HIGH);
            durumBildir("Bacak asagi hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(bacakasagi, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(bacakasagi, LOW);
                break;
            }
        }
    }

    // Ayak Yukarı Kontrolü
    if (veri == "ayak yukari") {
        veri = "";
        while(1) {
            digitalWrite(ayakyukari, HIGH);
            durumBildir("Ayak yukari hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(ayakyukari, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(ayakyukari, LOW);
                break;
            }
        }
    }

    // Ayak Aşağı Kontrolü
    if (veri == "ayak asagi") {
        veri = "";
        while(1) {
            digitalWrite(ayakasagi, HIGH);
            durumBildir("Ayak asagi hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(ayakasagi, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(ayakasagi, LOW);
                break;
            }
        }
    }

    // Yatak Yukarı Kontrolü
    if (veri == "yatak yukari") {
        veri = "";
        while(1) {
            digitalWrite(altsirtyukari, HIGH);
            digitalWrite(altayakyukari, HIGH);
            durumBildir("Yatak yukari hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(altsirtyukari, LOW);
                digitalWrite(altayakyukari, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(altsirtyukari, LOW);
                digitalWrite(altayakyukari, LOW);
                break;
            }
        }
    }

    // Yatak Aşağı Kontrolü
    if (veri == "yatak asagi") {
        veri = "";
        while(1) {
            digitalWrite(altsirtasagi, HIGH);
            digitalWrite(altayakasagi, HIGH);
            durumBildir("Yatak asagi hareket ediyor");
            
            if(Serial.available()) {
                char c = (char)Serial.read();
                veri += c;
                veri.toLowerCase();
                delay(10);
            }
            server.handleClient();
            if (veri != "dur" && veri != "") {
                veri = "";
                digitalWrite(altsirtasagi, LOW);
                digitalWrite(altayakasagi, LOW);
                break;
            }
            if (veri == "dur") {
                digitalWrite(altsirtasagi, LOW);
                digitalWrite(altayakasagi, LOW);
                break;
            }
        }
    }

    // Buton Kontrolleri
    if ((digitalRead(buton1) == 1) && (digitalRead(buton2) == 0)) {
        digitalWrite(altsirtyukari, HIGH);
    } else {
        digitalWrite(altsirtyukari, LOW);
    }
    
    if ((digitalRead(buton2) == 1) && (digitalRead(buton1) == 0)) {
        digitalWrite(altsirtasagi, HIGH);
    } else {
        digitalWrite(altsirtasagi, LOW);
    }
    
    if ((digitalRead(buton3) == 1) && (digitalRead(buton4) == 0)) {
        digitalWrite(altayakyukari, HIGH);
    } else {
        digitalWrite(altayakyukari, LOW);
    }
    
    if ((digitalRead(buton4) == 1) && (digitalRead(buton3) == 0)) {
        digitalWrite(altayakasagi, HIGH);
    } else {
        digitalWrite(altayakasagi, LOW);
    }
    if ((digitalRead(buton5) == 1) && (digitalRead(buton6) == 0)) {
        digitalWrite(sirtyukari, HIGH);
    } else {
        digitalWrite(sirtyukari, LOW);
    }
    
    if ((digitalRead(buton6) == 1) && (digitalRead(buton5) == 0)) {
        digitalWrite(sirtasagi, HIGH);
    } else {
        digitalWrite(sirtasagi, LOW);
    }
    
    if ((digitalRead(buton7) == 1) && (digitalRead(buton8) == 0)) {
        digitalWrite(bacakyukari, HIGH);
    } else {
        digitalWrite(bacakyukari, LOW);
    }
    
    if ((digitalRead(buton8) == 1) && (digitalRead(buton7) == 0)) {
        digitalWrite(bacakasagi, HIGH);
    } else {
        digitalWrite(bacakasagi, LOW);
    }
    
    if ((digitalRead(buton9) == 1) && (digitalRead(buton10) == 0)) {
        digitalWrite(ayakyukari, HIGH);
    } else {
        digitalWrite(ayakyukari, LOW);
    }
    
    if ((digitalRead(buton10) == 1) && (digitalRead(buton9) == 0)) {
        digitalWrite(ayakasagi, HIGH);
    } else {
        digitalWrite(ayakasagi, LOW);
    }
    veri = "";
    delay(50);
    sayici++;
    if (sayici == 1000 && Serial.available() == 0) {
        digitalWrite(enable, HIGH);
        delay(250);
        digitalWrite(enable, LOW);
        sayici = 0;
    }
}
    