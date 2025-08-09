# ESP32 Stock Tracker

A real-time stock price tracker for ESP32 with 2.8" TFT displays. Shows live stock data from Yahoo Finance with automatic WiFi setup and clean interface.

## 🖨️ 3D Printable Case

**Download the 3D printable case from MakerWorld:**
[Stock Tracker ESP32 Display - MakerWorld](https://makerworld.com/en/models/1683317-stock-tracker-esp32-display#profileId-1783519)

The case protects your ESP32 display and gives it a clean, professional desk appearance.

## 🚀 Features

- **Live Stock Prices** - Real-time data from Yahoo Finance (no API key needed)
- **Clean Display** - Organized table format (Symbol | Price | Change)
- **Smart Updates** - Only refreshes when prices actually change
- **Color Coding** - Green for gains, red for losses
- **WiFi Manager** - Automatic captive portal setup for WiFi credentials
- **Browser Installation** - Flash firmware directly from your browser
- **Pre-configured** - Ready to use with popular tech stocks

## 📋 Hardware Requirements

- **ESP32 with 2.8" TFT display** (ESP32-2432S028R, also called "Cheap Yellow Display")
  - [Amazon Link](https://www.amazon.com/s?k=ESP32-2432S028R)
  - [AliExpress Link](https://www.aliexpress.com/w/wholesale-ESP32-2432S028R.html)
- **USB Cable** for programming and power
- **WiFi Network** for internet connectivity
- **3D Printed Case** - [Download from MakerWorld](https://makerworld.com/en/models/1683317-stock-tracker-esp32-display#profileId-1783519)

## ⚙️ Installation Options

### Option 1: Browser Installation (Recommended)

1. **Use the live webflasher** (easiest option):
   **Go to: [https://stocktracker.apps.dj/](https://stocktracker.apps.dj/)**

2. **Or run the webflasher locally** in Chrome or Edge browser:
   ```bash
   # Run the webflasher locally
   cd Stock_Tracker_webflasher
   python3 -m http.server 8003
   ```
   Then go to `http://localhost:8003`

3. **Or deploy with Docker:**
   ```bash
   docker-compose up -d
   ```
   Then go to `http://localhost:8003`

4. **Connect and flash:**
   - Connect ESP32 to USB
   - Click "Connect & Install Stock Tracker"
   - Select USB Serial port
   - Wait for installation to complete

5. **WiFi Setup (automatic):**
   - ESP32 creates hotspot "Stock_Tracker_Setup"
   - Connect with your phone
   - Enter your WiFi credentials
   - ESP32 automatically connects and starts tracking stocks

### Option 2: Manual Development Setup

1. **Install PlatformIO:**
   ```bash
   pip install platformio
   ```

2. **Clone and build:**
   ```bash
   git clone <repository-url>
   cd Stock_Tracker
   platformio run
   ```

3. **Upload firmware:**
   ```bash
   platformio run --target upload
   ```

4. **Monitor output:**
   ```bash
   platformio device monitor --port /dev/cu.usbserial-110
   ```

## 📊 Pre-configured Stocks

The tracker comes pre-loaded with these popular tech stocks:
- **AAPL** - Apple Inc.
- **GOOGL** - Alphabet Inc.
- **NVDA** - NVIDIA Corporation
- **TSLA** - Tesla Inc.
- **META** - Meta Platforms Inc.
- **AMZN** - Amazon.com Inc.
- **MSFT** - Microsoft Corporation
- **AMD** - Advanced Micro Devices Inc.

## 📊 Display Format

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

Live (8 stocks)
```

## ⚙️ Configuration

### Changing Stocks
Edit `config.h` to modify the stock list:
```cpp
static const char* STOCK_SYMBOLS[] = {"AAPL", "GOOGL", "NVDA", "TSLA", "META", "AMZN", "MSFT", "AMD"};
static const char* STOCK_NAMES[] = {"Apple", "Alphabet", "NVIDIA", "Tesla", "Meta", "Amazon", "Microsoft", "AMD"};
```

### Timezone
Currently hardcoded to Pacific Time (PST). Change in `config.h`:
```cpp
#define TIMEZONE_OFFSET -8  // PST (Pacific Standard Time)
```

### Update Frequency
Default is 60 seconds. Change in `config.h`:
```cpp
#define UPDATE_INTERVAL_SECONDS 60
```

## 🔧 Troubleshooting

### WiFi Issues
- WiFi credentials are saved automatically after first setup
- If WiFi setup runs every boot, check that `autoConnect()` is used in code
- ESP32 only supports 2.4GHz networks

### Browser Installation Issues
- Only works in Chrome, Edge, or Opera browsers
- Requires HTTPS or localhost
- Make sure ESP32 is connected via USB before clicking Connect

### Display Issues
- Verify 2.8" TFT display is properly connected
- Check TFT_eSPI configuration in `platformio.ini`

### Stock Data Issues
- Requires internet connection
- Yahoo Finance API is free but may have rate limits
- Increase update interval if getting errors

## 📁 Project Structure

```
Stock_Tracker/
├── src/
│   └── main.cpp                    # ESP32 firmware
├── Stock_Tracker_webflasher/       # Browser-based installer
│   ├── index.html                 # Webflasher interface
│   ├── manifest-2.8inch.json      # ESP Web Tools manifest
│   ├── stock_tracker.png          # Logo image
│   └── *.bin                      # Firmware binaries
├── config.h                       # Configuration settings
├── platformio.ini                 # PlatformIO build config
├── Dockerfile                     # Docker deployment
├── docker-compose.yml             # Docker Compose config
├── .gitignore                     # Git ignore rules
├── context.txt                    # Development documentation
└── README.md                      # This file
```

## 🐳 Docker Deployment

Deploy the webflasher using Docker:

```bash
# Build and run
docker-compose up -d

# Or manually
docker build -t stock-tracker-webflasher .
docker run -d -p 8003:8003 stock-tracker-webflasher
```

Access at `http://localhost:8003`

## 🤝 Contributing

Feel free to submit issues and enhancement requests!

## 📄 License

This project is open source and available under the MIT License.

---

**Happy Stock Tracking!**