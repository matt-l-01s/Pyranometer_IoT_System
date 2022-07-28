// Connect to Uni WiFi only. ESP32
// For connecting to WiFi
#include <WiFi.h>
#include <String.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <C:\Users\Matthew\Documents\Arduino\libraries\mywifidetails\mywifidetails.h>
#define EAP_IDENTITY eap_id //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD eap_pass //your Eduroam password
int modeWiFi;

// ThingSpeak variables
#include <ThingSpeak.h>
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long channelID = 1788802;
const char* readAPIKey = "FBI3BM2JOO810YFU";
const char* writeAPIKey = "QUVECGF3ZJD2PHON";
WiFiClient client;

// NTP setup
#include <NTP.h>
WiFiUDP ntpUDP;
NTP ntp(ntpUDP);
unsigned long prevTime = 0;
unsigned long delayTime = 59;
unsigned long prevNTP = 0;

// ADC Setup
#include "esp_adc_cal.h"
#define Sensor1    32
int ADC_value = 0;
float Voltage = 0.0;
float final_value = 0.0;

void setup() {
  Serial.begin(9600);
  connectUCSIWiFi();

  ntp.ruleSTD("+08", Last, Tue, Jun, 0, 0);
  ntp.begin();
  ThingSpeak.begin(client);
  if (ThingSpeak.begin(client)) {
    Serial.println("ThingSpeak connection established.");
    //sendData();
  }
}

void loop() {
  ntp.update();
  prevNTP = ntp.seconds();
  //  Serial.println(prevNTP);
  if (prevNTP - prevTime >=  delayTime) {
    sendData();
    //    prevTime = prevNTP;
  }
}

void sendData() {
  ADC_value = analogRead(Sensor1);
  // Calibrate ADC & Get Voltage (in mV)
  Voltage = readADC_Cal(ADC_value);

  // Print The Readings
  Serial.print("ADC: ");
  Serial.println(ADC_value);
  Serial.print("Voltage (V): ");
  Serial.println(Voltage / 1000);
  delay(1000);

  // setField sets the value before writing to ThingSpeak
  ThingSpeak.setField(1, ADC_value);

  int x = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  Serial.println("Please wait while we collect the data...");

}

// ADC function
uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;

  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

void connectUCSIWiFi() {
  const char* wpa2e_ssid = "Connect@UCSI"; // Eduroam wpa2e_ssid
  const char* host = "ucsiuniversity.edu.my"; //external server domain for HTTP connection after authentification
  int wpa2e_count = 0;

  // Code for connecting to UCSI WiFi
  delay(10);
  Serial.println();
  Serial.print("Connecting to network: " + String(wpa2e_ssid));
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wifi_sta_wpa2_ent_enable();
  WiFi.begin(wpa2e_ssid); //connect to wifi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wpa2e_count++;
    if (wpa2e_count >= 60) { //after 30 seconds timeout - reset board
      Serial.println("\nCouldn't get a wifi connection. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("\nConnected to wifi with IP Address: ");
  Serial.println(WiFi.localIP());
}
