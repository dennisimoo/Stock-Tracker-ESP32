# ESP32 Stock Tracker

A real-time stock price tracker for ESP32 displays showing live stock data from Yahoo Finance. Perfect for monitoring your favorite tech stocks with a clean, easy-to-read interface.

## 🚀 Features

- **Live Stock Prices** - Real-time data from Yahoo Finance (no API key needed!)
- **Clean Display** - Organized table format (Symbol | Price | Change)
- **Smart Updates** - Only refreshes when prices actually change
- **Color Coding** - Green for gains, red for losses
- **12-Hour Time** - Shows last update time in PST with AM/PM
- **WiFi Auto-Connect** - Automatically connects to your network
- **Power Efficient** - Minimal screen updates to save battery

## 📋 Hardware Requirements

- **ESP32-2432S028R** (Cheap Yellow Display) with 2.8" TFT touchscreen
- **USB Cable** for programming and power
- **WiFi Network** for internet connectivity

## ⚙️ Setup Instructions

### 1. Install PlatformIO

**Option A: VS Code Extension (Recommended)**
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Open VS Code and install the "PlatformIO IDE" extension
3. Restart VS Code

**Option B: Command Line**
```bash
pip install platformio
```

### 2. Configure WiFi Settings

**IMPORTANT:** Before uploading, you must configure your WiFi credentials!

1. Open `config.h` in the root directory
2. Edit these lines with your WiFi information:
```cpp
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

### 3. Customize Settings (Optional)

In `config.h`, you can also modify:
- **Timezone**: Change `TIMEZONE_OFFSET` for your location
  - PST: -8, PDT: -7
  - MST: -7, MDT: -6  
  - CST: -6, CDT: -5
  - EST: -5, EDT: -4
- **Stocks**: Modify `STOCK_SYMBOLS` array to track different stocks
- **Update Rate**: Change `UPDATE_INTERVAL_SECONDS` (default: 30 seconds)
- **Brightness**: Adjust `LCD_BRIGHTNESS` (0-255, default: 255)

### 4. Connect Hardware

1. Connect your ESP32 to your computer via USB
2. Note the serial port (you'll need this for uploading)

### 5. Upload Code

**Option A: VS Code PlatformIO**
1. Open this project folder in VS Code
2. Click the PlatformIO icon in the sidebar
3. Click "Upload" button

**Option B: Command Line**
```bash
# Find your serial port first
ls /dev/cu.*  # macOS/Linux
# or
mode  # Windows

# Upload (replace with your actual port)
pio run --target upload --upload-port /dev/cu.usbserial-210
```

**Common Serial Ports:**
- macOS: `/dev/cu.usbserial-210` or `/dev/cu.SLAB_USBtoUART`
- Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`
- Windows: `COM3`, `COM4`, etc.

### 6. Monitor Output (Optional)

To see debug information:
```bash
pio device monitor --port /dev/cu.usbserial-210
```

## 📊 Display Format

The screen shows:
```
STOCK TRACKER
Symbol    Price     Change
AAPL      $202.92   -0.21%
GOOGL     $194.67   -0.19%
NVDA      $178.26   -0.97%
TSLA      $308.72   -0.17%
META      $763.46   -1.66%
AMZN      $213.75   +0.99%
MSFT      $527.75   -1.47%
AMD       $174.31   -1.40%

Last updated: 7:48:54 PM PST
```

## 🔧 Troubleshooting

### WiFi Connection Issues
- Double-check SSID and password in `config.h`
- Ensure your WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Check that your network allows new devices

### Upload Issues
- Make sure you have the correct serial port
- Try pressing the BOOT button on ESP32 during upload
- Check USB cable connection

### Display Issues
- Verify TFT_eSPI library is properly configured
- Check `TFT_eSPI/User_Setup.h` for correct pin definitions
- Adjust brightness in `config.h` if display is too dim/bright

### Stock Data Issues
- Check internet connection
- Yahoo Finance API is free but may have rate limits
- Increase `UPDATE_INTERVAL_SECONDS` if getting errors

## 📁 Project Structure

```
StockTracker/
├── config.h              # Configuration settings (WiFi, stocks, etc.)
├── src/
│   └── main.cpp          # Main application code
│   └── display.jpg       # Image
├── platformio.ini        # PlatformIO build configuration
├── .gitignore           # Git ignore rules
└── README.md            # This file
```

## 🤝 Contributing

Feel free to submit issues and enhancement requests!

## 📄 License

This project is open source and available under the MIT License.

---

**Happy Stock Tracking! 📈**
