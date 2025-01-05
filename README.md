Hospital Bed Control System



Web interface and physical button controlled smart hospital bed system based on ESP32.

Last Updated: 2025-01-05 15:43:35 UTC
Author: ycagdass

📋 Features
Web-based control interface with responsive design
Physical button control system
WiFi connectivity for remote access
Real-time status monitoring
Multiple motor control for different bed sections
Emergency stop functionality
Connection status monitoring
Mobile-friendly interface
🔧 Technical Specifications
Hardware
Microcontroller: ESP32-WROOM-32
Motors: 5x Linear Actuators (12V/24V DC)
Buttons: 10x Physical Control Buttons
Power Supply: 12V/24V DC
Motor Drivers: PWM Compatible Motor Drivers
Software
Development Platform: Arduino IDE/PlatformIO
Programming Language: C++
Web Technologies: HTML, CSS, JavaScript
Network: WiFi 2.4GHz
Protocol: HTTP
📦 Installation
Clone the Repository
bash
git clone https://github.com/ycagdass/hospital-bed-control-system.git
cd hospital-bed-control-system
Configure WiFi Settings
C++
// Edit wifi credentials in config.h
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
Hardware Setup
Connect motors according to pin configuration
Wire up physical buttons
Connect power supply
Verify all connections
Upload Code
Open project in Arduino IDE or PlatformIO
Select ESP32 board
Upload code
📝 Usage
Web Interface
Connect to the same WiFi network as ESP32
Access web interface through ESP32's IP address
Use control buttons for bed adjustments
Monitor connection status
Physical Controls
Use physical buttons for direct control
Each section has up/down buttons
Emergency stop button available
🛠️ Pin Configuration
C++
// Button Pins
const int buton1 = 23;   // Lower back up
const int buton2 = 22;   // Lower back down
const int buton3 = 21;   // Lower leg up
const int buton4 = 19;   // Lower leg down
const int buton5 = 18;   // Back up
const int buton6 = 5;    // Back down
const int buton7 = 4;    // Leg up
const int buton8 = 2;    // Leg down
const int buton9 = 15;   // Foot up
const int buton10 = 13;  // Foot down

// Motor Control Pins
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
📁 Project Structure
Code
hospital-bed-control-system/
├── src/
│   └── main.cpp           # Main code file
├── include/
│   └── config.h          # Configuration file
├── docs/
│   ├── TECHNICAL.md     # Technical documentation
│   └── SETUP.md        # Setup guide
└── README.md
⚠️ Safety Features
Automatic connection loss detection
Motor protection system
Emergency stop control
Overload protection
Real-time status monitoring
Connection status indicator
🔍 Troubleshooting
Common issues and solutions:

WiFi Connection Issues

Check WiFi credentials
Verify ESP32 is powered
Reset ESP32
Motor Control Issues

Check power supply
Verify motor connections
Check driver connections
Web Interface Issues

Verify IP address
Check network connection
Clear browser cache
🔄 Maintenance
Daily Checks
Connection status verification
Motor operation test
Button functionality test
Weekly Checks
System log review
Motor lubrication
Mechanical connection check
📜 License
This project is licensed under the MIT License - see the LICENSE file for details.

📫 Support
For support and queries:

Create an issue in this repository
Contact: [your-email@domain.com]
Documentation: See docs/ folder
🔄 Updates
Version 1.0.0 (2025-01-05)
Initial release
Basic functionality implementation
Web interface implementation
Motor control system
Safety features
⚠️ Disclaimer
This software is provided "as is", without warranty of any kind. Use at your own risk. The author is not responsible for any damages or liabilities.

🤝 Contributing
Fork the repository
Create your feature branch
Commit your changes
Push to the branch
Create a new Pull Request
Made with ❤️ by ycagdass
Last Updated: 2025-01-05 15:43:35 UTC

⬆ back to top
