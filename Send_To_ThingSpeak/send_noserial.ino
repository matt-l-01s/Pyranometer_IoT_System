// Connect to Uni WiFi only
#include <WiFi.h>
#include <String.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <C:\Users\Matthew\Documents\Arduino\libraries\mywifidetails\mywifidetails.h> // get EAP username and password from here
#define EAP_IDENTITY eap_id
#define EAP_PASSWORD eap_pass
int modeWiFi;

// ThingSpeak variables
#include <ThingSpeak.h>
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long channelID = 1788802;
const char* writeAPIKey = "QUVECGF3ZJD2PHON"; // Only Write API Key needed
WiFiClient client;

// NTP setup
#include <NTP.h>
WiFiUDP ntpUDP;
NTP ntp(ntpUDP);
unsigned long prevTime = 0;
unsigned long delayTime = 59; // Delay time is 60 seconds, starting from 0 and ending at 59
unsigned long prevNTP = 0;

// ADC Setup
#include "ADS1X15.h"
ADS1115 ADS(0x48); // default address, ADDR connected to GND
int ADCValueRef, ADCValueSensor1, ADCValueSensor2, ADCValueSensor3; // Store reading from 3 sensors as well as reference sensor

void setup() {
  connectUCSIWiFi();

  ADS.begin();
  ADS.setGain(1);
  ntp.ruleSTD("+08", Last, Tue, Jun, 0, 0);
  ntp.begin();
  ThingSpeak.begin(client);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectUCSIWiFi();
  }
  ntp.update();
  prevNTP = ntp.seconds();
  if (prevNTP - prevTime >=  delayTime) {
    sendData();
  }
}

void sendData() {
    ADCValueRef = ADS.readADC(0); // Collect the sensor readings from the 4 ADS1115 channels
    ADCValueSensor1 = ADS.readADC(1);
    ADCValueSensor2 = ADS.readADC(2);
    ADCValueSensor3 = ADS.readADC(3);

    // setField sets the value before writing to ThingSpeak
    ThingSpeak.setField(1, ADCValueRef);
    ThingSpeak.setField(2, ADCValueSensor1);
    ThingSpeak.setField(3, ADCValueSensor2);
    ThingSpeak.setField(4, ADCValueSensor3);

    // Write the data
    int sendDataStatus = ThingSpeak.writeFields(channelID, writeAPIKey);
}

void connectUCSIWiFi() {
  const char* wpa2e_ssid = "Connect@UCSI"; // Eduroam wpa2e_ssid
  const char* host = "ucsiuniversity.edu.my"; //external server domain for HTTP connection after authentification
  int wpa2e_count = 0;

  // Code for connecting to UCSI WiFi
  delay(10);
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wifi_sta_wpa2_ent_enable();
  WiFi.begin(wpa2e_ssid); //connect to wifi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    wpa2e_count++;
    if (wpa2e_count >= 60) { //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
}
