#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include "../config.h"

#define LCD_BACKLIGHT_PIN 21
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

#define UPDATE_INTERVAL (UPDATE_INTERVAL_SECONDS * 1000UL)


// Stock data structure
struct StockData {
  String symbol;
  String name;
  float price;
  float change;
  float change_percent;
  bool valid;
  bool changed; // Track if data changed
};

// Global variables
TFT_eSPI tft = TFT_eSPI();
static StockData stocks[NUM_STOCKS];
static time_t last_update_time = 0;

// Function declarations
void create_ui();
void fetch_stock_data();
void update_display();
void update_single_stock(int stock_index);
void show_initial_structure();

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("=== STOCK TRACKER WITH WIFIMANAGER v2.0 ===");
  Serial.println("This version uses WiFiManager for WiFi setup");
  
  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
  analogWrite(LCD_BACKLIGHT_PIN, LCD_BRIGHTNESS);
  
  
  // Initialize stock data
  for (int i = 0; i < NUM_STOCKS; i++) {
    stocks[i].symbol = STOCK_SYMBOLS[i];
    stocks[i].name = STOCK_NAMES[i];
    stocks[i].price = 0.0;
    stocks[i].change = 0.0;
    stocks[i].change_percent = 0.0;
    stocks[i].valid = false;
    stocks[i].changed = false;
  }
  
  // Setup WiFiManager
  WiFiManager wm;
  
  Serial.println("=== WIFI MANAGER DEBUG ===");
  Serial.println("Initializing WiFiManager...");
  
  // Check if we have saved WiFi credentials
  wifi_config_t conf;
  esp_wifi_get_config(WIFI_IF_STA, &conf);
  Serial.printf("Saved SSID: '%s'\n", conf.sta.ssid);
  Serial.printf("Saved SSID length: %d\n", strlen((char*)conf.sta.ssid));
  
  if (strlen((char*)conf.sta.ssid) > 0) {
    Serial.println("Found saved WiFi credentials!");
  } else {
    Serial.println("No saved WiFi credentials found");
  }
  
  // Configure WiFiManager to preserve settings
  wm.setDebugOutput(true);
  wm.setConfigPortalTimeout(120); // 2 minute timeout for config portal
  wm.setConnectTimeout(30); // 30 second timeout for connection attempts
  wm.setSaveParamsCallback([](){
    Serial.println("=== WIFI PARAMS SAVED CALLBACK ===");
  });
  wm.setSaveConfigCallback([](){
    Serial.println("=== WIFI CONFIG SAVED CALLBACK ===");
  });
  
  // Try to connect to saved WiFi first
  Serial.println("Attempting WiFi connection with autoConnect...");
  
  tft.setCursor(10, 10);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print("WiFi Setup...");
  
  // Show setup message on display
  tft.setCursor(10, 40);
  tft.setTextSize(1);
  tft.print("1. Connect to WiFi:");
  tft.setCursor(10, 55);
  tft.print("   Stock_Tracker_Setup");
  tft.setCursor(10, 75);
  tft.print("2. Enter WiFi credentials");
  tft.setCursor(10, 90);
  tft.print("3. Wait for connection...");
  
  // Try autoConnect first - only show portal if no saved WiFi
  Serial.println("Attempting WiFi connection...");
  
  bool wifi_result = wm.autoConnect("Stock_Tracker_Setup");
  
  if (!wifi_result) {
    Serial.println("=== WIFI CONNECTION FAILED ===");
    Serial.println("Failed to connect and hit timeout");
    tft.fillScreen(TFT_RED);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE);
    tft.print("WiFi Setup Failed");
    tft.setCursor(10, 30);
    tft.print("Restarting...");
    delay(3000);
    ESP.restart();
  }
  
  // Connected! Check credentials again
  Serial.println("=== WIFI CONNECTION SUCCESS ===");
  Serial.println("WiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  
  // Check if credentials are now saved
  wifi_config_t conf_after;
  esp_wifi_get_config(WIFI_IF_STA, &conf_after);
  Serial.printf("After connection - Saved SSID: '%s'\n", conf_after.sta.ssid);
  Serial.printf("After connection - Saved SSID length: %d\n", strlen((char*)conf_after.sta.ssid));
  
  // Stop WiFiManager AP mode (turn off hotspot)
  Serial.println("Stopping WiFiManager portal and AP...");
  wm.stopWebPortal();
  WiFi.softAPdisconnect(true);
  Serial.println("AP disconnected");
  
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.print("WiFi Connected!");
  delay(2000);
  
  // Configure time  
  configTime(TIMEZONE_OFFSET * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.printf("Time configured (UTC%d)\n", TIMEZONE_OFFSET);
  
  // Ready to start
  
  create_ui();
  
  // Show initial structure with all symbols
  show_initial_structure();
  
  fetch_stock_data();
}

void loop() {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    fetch_stock_data();
    lastUpdate = millis();
  }
  
  delay(1000);
}

void create_ui() {
  tft.fillScreen(TFT_BLACK);
  
  // Title
  tft.setCursor(10, 5);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.print("STOCK TRACKER");
  
  
  // Draw table header
  tft.drawLine(10, 30, 230, 30, TFT_BLUE);
  tft.setCursor(10, 35);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(1);
  tft.print("Symbol");
  tft.setCursor(80, 35);
  tft.print("Price");
  tft.setCursor(150, 35);
  tft.print("Change");
  tft.drawLine(10, 50, 230, 50, TFT_BLUE);
}

void fetch_stock_data() {
  Serial.println("=== Starting stock data fetch ===");
  
  // Debug WiFi status
  Serial.printf("WiFi status: %d (WL_CONNECTED=%d)\n", WiFi.status(), WL_CONNECTED);
  Serial.printf("Current SSID: %s\n", WiFi.SSID().c_str());
  Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("ERROR: WiFi not connected!");
    
    // Check saved credentials
    wifi_config_t conf_check;
    esp_wifi_get_config(WIFI_IF_STA, &conf_check);
    Serial.printf("Saved credentials check - SSID: '%s'\n", conf_check.sta.ssid);
    
    return;
  }
  Serial.println("WiFi is connected");
  
  for (int i = 0; i < NUM_STOCKS; i++) {
    // Using Yahoo Finance API (free, no API key needed)
    String url = String("https://query1.finance.yahoo.com/v8/finance/chart/") + STOCK_SYMBOLS[i];
    
    Serial.printf("Fetching %s...\n", STOCK_SYMBOLS[i]);
    Serial.printf("URL: %s\n", url.c_str());
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(15000); // Increased timeout to 15 seconds
    http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
    http.addHeader("Accept", "application/json");
    http.addHeader("Connection", "close");
    
    int httpCode = http.GET();
    Serial.printf("HTTP Response Code: %d\n", httpCode);
    
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.printf("Payload length: %d\n", payload.length());
      
      DynamicJsonDocument doc(40 * 1024); // Increased to 40KB for large Yahoo responses
      DeserializationError error = deserializeJson(doc, payload);
      
      if (error) {
        Serial.printf("JSON parse error: %s\n", error.c_str());
      } else {
        Serial.println("JSON parsed successfully");
        
        if (doc["chart"]["result"][0]["meta"]) {
          JsonObject meta = doc["chart"]["result"][0]["meta"];
          
          float price = meta["regularMarketPrice"];
          float prev_close = meta["previousClose"];
          
          Serial.printf("Raw data - Current: %.2f, Previous: %.2f\n", price, prev_close);
          
          if (price > 0 && prev_close > 0) {
            float change = price - prev_close;
            float change_percent = (change / prev_close) * 100;
            
            // Check if data changed (always true on first time when valid=false)
            bool data_changed = true; // Always update on first successful fetch
            if (stocks[i].valid) {
              // Only check for changes if we already have valid data
              data_changed = (abs(stocks[i].price - price) > 0.01 ||
                            abs(stocks[i].change_percent - change_percent) > 0.01);
            }
            
            stocks[i].price = price;
            stocks[i].change = change;
            stocks[i].change_percent = change_percent;
            stocks[i].valid = true;
            stocks[i].changed = data_changed;
            
            Serial.printf("SUCCESS: %s: $%.2f (%+.2f%%) %s\n", 
                         STOCK_SYMBOLS[i], 
                         stocks[i].price, 
                         stocks[i].change_percent,
                         data_changed ? "[CHANGED]" : "");
            
            // Mark that we have new data (display will be updated later if needed)
          } else {
            Serial.printf("ERROR: Invalid price data for %s\n", STOCK_SYMBOLS[i]);
          }
        } else {
          Serial.println("ERROR: No chart data found");
        }
      }
    } else {
      Serial.printf("HTTP GET failed: %d\n", httpCode);
    }
    
    http.end();
    delay(500); // Rate limiting
  }
  
  // Check if any data changed
  bool any_changed = false;
  for (int i = 0; i < NUM_STOCKS; i++) {
    if (stocks[i].changed) {
      any_changed = true;
      break;
    }
  }
  
  // Show summary
  Serial.printf("=== Stock fetch complete. Valid stocks: ");
  for (int i = 0; i < NUM_STOCKS; i++) {
    if (stocks[i].valid) {
      Serial.printf("%s ", STOCK_SYMBOLS[i]);
    }
  }
  Serial.printf("- Changes: %s\n", any_changed ? "YES" : "NO");
  
  // Count valid stocks
  int valid_stocks = 0;
  for (int i = 0; i < NUM_STOCKS; i++) {
    if (stocks[i].valid) valid_stocks++;
  }
  
  // Only update display if data actually changed
  if (any_changed) {
    Serial.println("Updating display due to data changes");
    last_update_time = time(nullptr); // Update timestamp only when data changes
    update_display();
    
    // Reset change flags
    for (int i = 0; i < NUM_STOCKS; i++) {
      stocks[i].changed = false;
    }
  } else {
    Serial.println("No data changes, skipping display update");
  }
}

void update_display() {
  Serial.println("=== Updating display ===");
  
  // Clear data area
  tft.fillRect(10, 55, 220, 200, TFT_BLACK);
  
  // Count valid stocks
  int valid_count = 0;
  for (int i = 0; i < NUM_STOCKS; i++) {
    if (stocks[i].valid) {
      valid_count++;
    }
  }
  Serial.printf("Displaying %d valid stocks\n", valid_count);
  
  // Display all stocks in table format
  int y = 60;
  for (int i = 0; i < NUM_STOCKS; i++) {
    // Show symbol even if not valid yet
    tft.setCursor(10, y);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.print(STOCK_SYMBOLS[i]);
    
    if (stocks[i].valid) {
      Serial.printf("Drawing %s at y=%d: $%.2f (%.2f%%)\n", 
                   stocks[i].symbol.c_str(), y, stocks[i].price, stocks[i].change_percent);
      
      // Price
      tft.setCursor(80, y);
      tft.setTextColor(TFT_WHITE);
      tft.printf("$%.2f", stocks[i].price);
      
      // Change
      tft.setCursor(150, y);
      uint16_t color = (stocks[i].change >= 0) ? TFT_GREEN : TFT_RED;
      tft.setTextColor(color);
      tft.printf("%+.2f%%", stocks[i].change_percent);
    } else {
      // Show "Loading..." for invalid stocks
      tft.setCursor(80, y);
      tft.setTextColor(TFT_YELLOW);
      tft.print("Loading...");
      Serial.printf("Stock %d (%s) is not valid - showing loading\n", i, STOCK_SYMBOLS[i]);
    }
    
    y += 20;
  }
  
  // Clear the status area to remove any old text
  tft.fillRect(10, 240, 220, 80, TFT_BLACK);
  
  // Status - Show connection status only
  tft.setCursor(10, 280);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(1);
  if (valid_count == 0) {
    tft.print("Connecting...");
  } else {
    tft.printf("Live (%d stocks)", valid_count);
  }
  
  // Commented out last updated time display
  /*
  struct tm timeinfo;
  if (last_update_time > 0 && localtime_r(&last_update_time, &timeinfo)) {
    // Convert to 12-hour format
    int hour12 = timeinfo.tm_hour;
    if (hour12 == 0) hour12 = 12;
    else if (hour12 > 12) hour12 -= 12;
    const char* ampm = (timeinfo.tm_hour >= 12) ? "PM" : "AM";
    
    tft.printf("Last updated: %d:%02d %s", 
               hour12, timeinfo.tm_min, ampm);
  }
  */
  
  Serial.println("=== Display update complete ===");
}

void update_single_stock(int stock_index) {
  Serial.printf("=== Updating single stock: %s ===\n", STOCK_SYMBOLS[stock_index]);
  
  // Calculate Y position for this stock
  int y = 60 + (stock_index * 20);
  
  // Clear just this stock's row
  tft.fillRect(10, y, 220, 20, TFT_BLACK);
  
  // Show symbol
  tft.setCursor(10, y);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.print(STOCK_SYMBOLS[stock_index]);
  
  if (stocks[stock_index].valid) {
    Serial.printf("Drawing %s at y=%d: $%.2f (%.2f%%)\n", 
                 stocks[stock_index].symbol.c_str(), y, stocks[stock_index].price, stocks[stock_index].change_percent);
    
    // Price
    tft.setCursor(80, y);
    tft.setTextColor(TFT_WHITE);
    tft.printf("$%.2f", stocks[stock_index].price);
    
    // Change
    tft.setCursor(150, y);
    uint16_t color = (stocks[stock_index].change >= 0) ? TFT_GREEN : TFT_RED;
    tft.setTextColor(color);
    tft.printf("%+.2f%%", stocks[stock_index].change_percent);
  }
  
  // Update status area
  tft.fillRect(10, 240, 220, 80, TFT_BLACK);
  tft.setCursor(10, 280);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(1);
  
  // Count valid stocks
  int valid_count = 0;
  for (int i = 0; i < NUM_STOCKS; i++) {
    if (stocks[i].valid) valid_count++;
  }
  
  if (valid_count == 0) {
    tft.print("Connecting...");
  } else {
    tft.printf("Live (%d stocks)", valid_count);
  }
  
  Serial.printf("=== Single stock update complete for %s ===\n", STOCK_SYMBOLS[stock_index]);
}

void show_initial_structure() {
  Serial.println("=== Showing initial structure ===");
  
  // Clear data area
  tft.fillRect(10, 55, 220, 200, TFT_BLACK);
  
  // Display all stock symbols with "Loading..." placeholders
  int y = 60;
  for (int i = 0; i < NUM_STOCKS; i++) {
    // Show symbol
    tft.setCursor(10, y);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.print(STOCK_SYMBOLS[i]);
    
    // Show "Loading..." placeholder
    tft.setCursor(80, y);
    tft.setTextColor(TFT_YELLOW);
    tft.print("Loading...");
    
    y += 20;
  }
  
  // Status
  tft.fillRect(10, 240, 220, 80, TFT_BLACK);
  tft.setCursor(10, 280);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(1);
  tft.print("Connecting...");
  
  Serial.println("=== Initial structure complete ===");
}

