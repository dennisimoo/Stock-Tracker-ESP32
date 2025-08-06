// Stock Tracker Configuration
// Edit these settings for your setup

#ifndef CONFIG_H
#define CONFIG_H

// WiFi Settings - CONFIGURE THESE BEFORE UPLOADING
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Time Zone Settings (offset from UTC in hours)
#define TIMEZONE_OFFSET -8  // PST: -8, PDT: -7, MST: -7, MDT: -6, CST: -6, CDT: -5, EST: -5, EDT: -4

// Stock symbols to track
const char* STOCK_SYMBOLS[] = {"AAPL", "GOOGL", "NVDA", "TSLA", "META", "AMZN", "MSFT", "AMD"};
const char* STOCK_NAMES[] = {"Apple", "Google", "NVIDIA", "Tesla", "Meta", "Amazon", "Microsoft", "AMD"};
const int NUM_STOCKS = 8;

// API Settings (Yahoo Finance - no API key needed)
#define UPDATE_INTERVAL_SECONDS 30

// Display Settings
#define LCD_BRIGHTNESS 255  // 0-255

#endif 