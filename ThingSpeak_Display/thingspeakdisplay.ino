// For connecting to WiFi
#include <WiFi.h>
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
const int field1 = 1; // ThingSpeak reading to be obtained from Channel 1
int field1Value; // Variable to save ThingSpeak reading to be displayed
String field1TimeStamp;
WiFiClient client;

// NTP setup
#include <NTP.h>
WiFiUDP ntpUDP;
NTP ntp(ntpUDP);
unsigned long prevTime = 0;
unsigned long delayTime = 59;
unsigned long prevNTP = 0;

void setup() {
  Serial.begin(115200);
  //  Serial.println("Please type '1' to connect home wifi, '2' to connect hotspot or 3 to connect uni wifi.");
  //  while (Serial.available() == 0) {

  //    modeWiFi = Serial.parseInt(); // while loop repeats this line until int detected in serial monitor
  //    if (modeWiFi != 0) {
  //      Serial.println(String(modeWiFi));
  //      switch (modeWiFi) {
  //        case 1:
  //connectWiFi(home_ssid, home_pass);
  //          break;
  //        case 2:
  //          connectWiFi(hots_ssid, hots_pass);
  //          break;
  //        case 3:
            connectUCSIWiFi();
  //          break;
  //      }
  //  }
  ntp.ruleSTD("+08", Last, Tue, Jun, 0, 0);
  ntp.begin();
  ThingSpeak.begin(client);
  if (ThingSpeak.begin(client)) {
    Serial.println("ThingSpeak connection established.");
  }
}

void loop() {
  ntp.update();
  prevNTP = ntp.seconds();
  //  Serial.println(prevNTP);
  if (prevNTP - prevTime >=  delayTime) {  // If 60 seconds passed, get ThingSpeak reading
  ReceiveData();
  }
}

void ReceiveData() {
  field1Value = ThingSpeak.readIntField(channelID, field1, readAPIKey);
  field1TimeStamp = ThingSpeak.readCreatedAt(channelID, readAPIKey);
  int receiveStatusCode = ThingSpeak.getLastReadStatus();
  if (receiveStatusCode == 200)
  {
    Serial.print("At time ");
    Serial.print(field1TimeStamp);
    Serial.print(" the raw ADC Value is ");
    Serial.println(field1Value);
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  Serial.println("Please wait while we collect the data...");
}

// WiFi connect functions
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

// Delete later
void connectWiFi(char* the_ssid, char* the_pass) {
  // connecting to wifi
  WiFi.begin(the_ssid, the_pass);
  int homeCount = 0;
  Serial.print("Connecting to network " + String(the_ssid));
  while ( WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    homeCount++;
    if ( homeCount >= 60) {
      Serial.println("\nCouldn't get a wifi connection. Restarting...");
      ESP.restart();
      return;
    }
  }
  Serial.println("\nConnected to wifi with IP Address: ");
  Serial.println(WiFi.localIP());
}
