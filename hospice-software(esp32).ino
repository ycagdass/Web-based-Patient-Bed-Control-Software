* Hasta Yatagı Wi-Fi Üzerinden Kontrol
* yazar: ycagdass


#include <WiFi.h>
#include <WebServer.h>

// WiFi ayarları
const char* ssid = "DENEME";
const char* password = "123456789";

WebServer server(80);

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang='tr'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Hasta Yatağı Kontrol</title>
    <style>
        :root {
            --background: linear-gradient(135deg, #f5f7fa, #c3cfe2);
            --text-color: #2c3e50;
            --container-bg: rgba(255, 255, 255, 0.8);
            --row-bg: rgba(255, 255, 255, 0.6);
            --button-gradient: linear-gradient(145deg, #3498db, #2980b9);
            --button-shadow: rgba(52, 152, 219, 0.2);
            --dur-button-gradient: linear-gradient(145deg, #e74c3c, #c0392b);
            --heading-color: #34495e;
        }

        [data-theme="dark"] {
            --background: linear-gradient(135deg, #2c3e50, #34495e);
            --text-color: #ecf0f1;
            --container-bg: rgba(0, 0, 0, 0.5);
            --row-bg: rgba(0, 0, 0, 0.3);
            --button-gradient: linear-gradient(145deg, #2980b9, #1a5276);
            --button-shadow: rgba(52, 152, 219, 0.4);
            --dur-button-gradient: linear-gradient(145deg, #c0392b, #922b21);
            --heading-color: #ecf0f1;
        }

        body {
            background: var(--background);
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            color: var(--text-color);
            text-align: center;
            padding: 20px;
            margin: 0;
            min-height: 100vh;
            transition: all 0.3s ease;
        }

        .theme-switch {
            position: relative;
            display: inline-block;
            margin: 20px auto;
        }

        .theme-switch input {
            display: none;
        }

        .slider {
            display: inline-flex;
            align-items: center;
            justify-content: space-between;
            width: 120px;
            height: 40px;
            background: var(--row-bg);
            border-radius: 20px;
            cursor: pointer;
            padding: 0 10px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }

        .slider:before {
            content: '🌞';
            font-size: 20px;
        }

        .slider:after {
            content: '🌙';
            font-size: 20px;
        }

        input:checked + .slider {
            background: var(--row-bg);
        }

        h1 {
            font-size: 2.2rem;
            margin-bottom: 40px;
            color: var(--heading-color);
            font-weight: 500;
            text-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }

        .control-row {
            display: flex;
            justify-content: center;
            align-items: center;
            margin: 15px 0;
            flex-wrap: wrap;
            padding: 10px;
            background: var(--row-bg);
            border-radius: 15px;
            backdrop-filter: blur(5px);
            max-width: 600px;
            margin-left: auto;
            margin-right: auto;
            transition: all 0.3s ease;
        }

        .button {
            display: inline-flex;
            align-items: center;
            justify-content: center;
            margin: 8px;
            width: 60px;
            height: 60px;
            font-size: 24px;
            color: #fff;
            background: var(--button-gradient);
            border: none;
            border-radius: 50%;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px var(--button-shadow);
            user-select: none;
            -webkit-tap-highlight-color: transparent;
        }

        .button:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 20px var(--button-shadow);
        }

        .button:active {
            transform: translateY(1px);
            box-shadow: 0 2px 10px var(--button-shadow);
        }

        .button.active {
            background: linear-gradient(145deg, #27ae60, #2ecc71) !important;
            box-shadow: 0 4px 15px rgba(46, 204, 113, 0.4) !important;
            animation: pulse 1.5s infinite;
        }

        .dur-button {
            background: var(--dur-button-gradient);
        }

        .dur-button.active {
            background: linear-gradient(145deg, #e74c3c, #c0392b) !important;
            box-shadow: 0 4px 15px rgba(231, 76, 60, 0.4) !important;
        }

        @keyframes pulse {
            0% {
                transform: scale(1);
            }
            50% {
                transform: scale(1.05);
            }
            100% {
                transform: scale(1);
            }
        }

        .label {
            font-size: 18px;
            margin: 0 20px;
            color: var(--text-color);
            font-weight: 500;
            width: 120px;
            transition: all 0.3s ease;
        }

        .container {
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            background: var(--container-bg);
            border-radius: 20px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
            transition: all 0.3s ease;
        }

        [data-theme="dark"] .button.active {
            background: linear-gradient(145deg, #16a085, #1abc9c) !important;
            box-shadow: 0 4px 15px rgba(26, 188, 156, 0.4) !important;
        }

        [data-theme="dark"] .dur-button.active {
            background: linear-gradient(145deg, #c0392b, #922b21) !important;
            box-shadow: 0 4px 15px rgba(192, 57, 43, 0.4) !important;
        }

        @media (max-width: 600px) {
            .control-row {
                padding: 5px;
            }
            .button {
                width: 50px;
                height: 50px;
                font-size: 20px;
            }
            .label {
                font-size: 16px;
                width: 100px;
            }
        }
    </style>
</head>
<body>
    <label class="theme-switch">
        <input type="checkbox" id="theme-toggle">
        <span class="slider"></span>
    </label>

    <div class="container">
        <h1>Hasta Yatağı Kontrol</h1>

        <!-- Alt Sırt -->
        <div class='control-row'>
            <button class='button' onclick='sendCommand("alt sirt yukari", this)'>&#x2191;</button>
            <span class='label'>Alt Sırt</span>
            <button class='button' onclick='sendCommand("alt sirt asagi", this)'>&#x2193;</button>
        </div>

        <!-- Alt Ayak -->
        <div class='control-row'>
            <button class='button' onclick='sendCommand("alt ayak yukari", this)'>&#x2191;</button>
            <span class='label'>Alt Ayak</span>
            <button class='button' onclick='sendCommand("alt ayak asagi", this)'>&#x2193;</button>
        </div>

        <!-- Sırt -->
        <div class='control-row'>
            <button class='button' onclick='sendCommand("sirt yukari", this)'>&#x2191;</button>
            <span class='label'>Sırt</span>
            <button class='button' onclick='sendCommand("sirt asagi", this)'>&#x2193;</button>
        </div>

        <!-- Bacak -->
        <div class='control-row'>
            <button class='button' onclick='sendCommand("bacak yukari", this)'>&#x2191;</button>
            <span class='label'>Bacak</span>
            <button class='button' onclick='sendCommand("bacak asagi", this)'>&#x2193;</button>
        </div>

        <!-- Ayak -->
        <div class='control-row'>
            <button class='button' onclick='sendCommand("ayak yukari", this)'>&#x2191;</button>
            <span class='label'>Ayak</span>
            <button class='button' onclick='sendCommand("ayak asagi", this)'>&#x2193;</button>
        </div>

        <!-- Yatak -->
        <div class='control-row'>
            <button class='button' onclick='sendCommand("yatak yukari", this)'>&#x2191;</button>
            <span class='label'>Yatak</span>
            <button class='button' onclick='sendCommand("yatak asagi", this)'>&#x2193;</button>
        </div>

        <!-- Dur -->
        <div class='control-row'>
            <button class='button dur-button' onclick='sendCommand("dur", this)'>Dur</button>
        </div>
    </div>

    <script>
        let activeButton = null;

        // Komut gönderme ve buton aktivasyonu fonksiyonu
        async function sendCommand(command, button) {
            console.log('Komut gönderildi:', command);
            
            // Önceki aktif butonu temizle
            if (activeButton) {
                activeButton.classList.remove('active');
            }

            // Yeni butonu aktif et
            button.classList.add('active');
            activeButton = button;

            // Dur komutunda tüm butonların aktifliğini kaldır
            if (command === 'dur') {
                setTimeout(() => {
                    button.classList.remove('active');
                    activeButton = null;
                }, 500);
            }

            await fetch('/control?command=' + encodeURIComponent(command));
        }

        // Tema değiştirme fonksiyonu
        document.getElementById('theme-toggle').addEventListener('change', function(e) {
            if(e.target.checked) {
                document.documentElement.setAttribute('data-theme', 'dark');
                localStorage.setItem('theme', 'dark');
            } else {
                document.documentElement.setAttribute('data-theme', 'light');
                localStorage.setItem('theme', 'light');
            }
        });

        // Sayfa yüklendiğinde tema kontrolü
        document.addEventListener('DOMContentLoaded', function() {
            const savedTheme = localStorage.getItem('theme') || 'light';
            document.documentElement.setAttribute('data-theme', savedTheme);
            document.getElementById('theme-toggle').checked = savedTheme === 'dark';
        });
    </script>
</body>
</html>
)=====";

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.println("");
    Serial.println("WiFi Baglaniyor");
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi Baglandi!");
        Serial.print("IP Adresi: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("WiFi Baglanti Hatasi!");
        Serial.println("Sistem yeniden baslatiliyor...");
        ESP.restart();
    }
}

void handleRoot() {
    server.send(200, "text/html", FPSTR(index_html));
}

void handleControl() {
    String command = server.arg("command");
    if (command.length() > 0) {
        // Komutu karakter karakter seri porta gönder
        for (int i = 0; i < command.length(); i++) {
            Serial.print((char)command[i]);
        }
       // Serial.print(); // Yeni satır karakteri ekle
    }
    server.send(200, "text/plain", "Komut alindi");
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    
    setupWiFi();
    
    server.on("/", handleRoot);
    server.on("/control", handleControl);
    server.begin();
    
    Serial.println("Web sunucusu baslatildi");
    Serial.println("Sistem hazir!");
}

void loop() {
    server.handleClient();
}
