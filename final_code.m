// Final code added to ESP32 as of April 2023

#include <WiFi.h>
#include <String.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <C:\Users\Matthew\Documents\Arduino\libraries\mywifidetails\mywifidetails.h> // get EAP username and password from here
#define EAP_IDENTITY eap_id
#define EAP_PASSWORD eap_pass
int modeWiFi;

#include "time.h"
const char* ntpServer = "swntp.ucsi.edu.my";
const long  gmtOffset_sec = 28800; //GMT+8:00
const int   daylightOffset_sec = 0;

#include "ADS1X15.h"
ADS1115 ADS(0x48); // default address, ADDR connected to GND
int16_t SP110ADC, BPW34_60, BPW34_70, BPW34_80;

#include "ThingSpeak.h"
unsigned long myChannelNumber = 1788802;
const char * myWriteAPIKey = "QUVECGF3ZJD2PHON";
WiFiClient client;


void ConnectEnterpriseWiFi() {
  const char* wpa2e_ssid = "Connect@UCSI";
  const char* host = "ucsiuniversity.edu.my"; //external server domain for HTTP connection after authentification
  int wpa2e_count = 0;
  delay(10);
  Serial.println();
  Serial.print("Connecting to network: " + String(wpa2e_ssid));
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  WiFi.begin(wpa2e_ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_IDENTITY, EAP_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wpa2e_count++;
    if (wpa2e_count >= 60) { //after 30 seconds timeout - reset board
      Serial.println("\nCouldn't get a wifi connection. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("\nConnected to UCSI Wifi with IP Address: ");
  Serial.println(WiFi.localIP());
}

void GetNTPsetRTC(){
  // Getting NTP server time
  Serial.print("Getting NTP server time to set ESP32 RTC.");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Successfully obtained NTP server time and set ESP32 RTC");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void gotoDeepSleep(){
  Serial.println("Entering Sleep Mode...");
  esp_sleep_enable_timer_wakeup(600 * 60 * 1000000ULL); // Wake up timer in the next 10 hours
  esp_deep_sleep_start();
}

void setup(){
  // Setup Serial Port
  Serial.begin(115200);
  // Connect to Wi-Fi
  ConnectEnterpriseWiFi();
  // Setup ADC
  ADS.begin();
  ADS.setGain(1);
  // Get NTP server time and set EPS32 RTC
  GetNTPsetRTC();
  // Setup ThingSpeak
  ThingSpeak.begin(client);
}

void loop(){
  // Check for WiFi connectivity
  if (WiFi.status() != WL_CONNECTED) {
    ConnectEnterpriseWiFi();
  }
  
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  
  // Send data to Thingspeak at per minute rate
  if (timeinfo.tm_sec==0){
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    // Goto to sleep at 8pm
    if (timeinfo.tm_hour==20){
      gotoDeepSleep();
    }
    // Read ADC
    SP110ADC = ADS.readADC(0);
    BPW34_60 = ADS.readADC(1); // 68Ohm  resistor
    BPW34_70 = ADS.readADC(2); // 75Ohm  resistor
    BPW34_80 = ADS.readADC(3); // 82Ohm  resistor
    // Set ThingSpeak Field
    ThingSpeak.setField(1, SP110ADC);
    ThingSpeak.setField(2, BPW34_60);
    ThingSpeak.setField(3, BPW34_70);
    ThingSpeak.setField(4, BPW34_80);
    // Write to ThingSpeak
    int StatusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(1000); // prevent loop duplicate write
    // if fail to write in the 1st attempt, try 2nd Attempt after 30s
    //if (StatusCode != 200){
    //  delay(30000);
    //  int StatusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    //}
    
    // Sync RTC with NTP server time hourly
    if (timeinfo.tm_min==0){
     configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }
     
  }
}
