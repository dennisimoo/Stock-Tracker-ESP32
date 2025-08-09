// Stock Tracker Configuration
// Edit these settings for your setup

#ifndef CONFIG_H
#define CONFIG_H

// WiFi Settings - Will be configured via WiFiManager
// No hardcoded credentials needed

// Time Zone Settings (offset from UTC in hours)
#define TIMEZONE_OFFSET -8  // PST (Pacific Standard Time)

// Stock symbols to track
const char* STOCK_SYMBOLS[] = {"AAPL", "GOOGL", "NVDA", "TSLA", "META", "AMZN", "MSFT", "AMD"};
const char* STOCK_NAMES[] = {"AAPL", "GOOGL", "NVDA", "TSLA", "META", "AMZN", "MSFT", "AMD"};
const int NUM_STOCKS = 8;

// API Settings (Yahoo Finance - no API key needed)
#define UPDATE_INTERVAL_SECONDS 60

// Display Settings
#define LCD_BRIGHTNESS 255

#endif 