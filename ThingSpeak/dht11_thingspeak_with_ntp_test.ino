#include <ThingSpeak.h>
#include <DHT.h>
#include <String.h>
#include <NTPClient.h>

// For connecting to WiFi
#include <WiFi.h>
#include "esp_wpa2.h" // WPA2 library for connections to Enterprise networks
#include <C:\Users\User\Documents\Arduino\libraries\mywifidetails\mywifidetails.h> // Replace with your own .h file with WiFi details
#define EAP_IDENTITY eap_id
#define EAP_PASSWORD eap_pass
int modeWiFi; // To select between connecting WPA2 and WPA2 Enterprise

// ThingSpeak variables
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long channelID = 1763798;
const char* readAPIKey = "L768L6LQ1W8EWJ4F";
const char* writeAPIKey = "BZRBAEK4J8915MZG";
WiFiClient client;

// DHT setup
#define DHTPin 15
#define DHTType DHT11
DHT dht(DHTPin, DHTType);

// NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
// temporary variables for using millis() as timer
unsigned long prevTime = 0;
unsigned long delayTime = 10000;

void setup() {
  Serial.begin(115200);
  Serial.println("Please type '1' to connect home wifi or '2' to connect uni wifi");
  while (Serial.available() == 0) {
    modeWiFi = Serial.parseInt();
    if (modeWiFi != 0) {
      Serial.println(String(modeWiFi));
      switch (modeWiFi) {
        case 1:
          connectHomeWiFi();
          break;
        case 2:
          connectUCSIWiFi();
          break;
      }
    }
  }

  // Initialize DHT, NTP and ThingSpeak
  dht.begin();
  timeClient.begin();
  ThingSpeak.begin(client);
  if (ThingSpeak.begin(client)) {
    Serial.println("ThingSpeak connection established.");
    sendData();
  }
}

void loop() {
  
  if (millis() - prevTime >=  delayTime) {
    sendData();
    prevTime = millis();
  }
}

void sendData() {
  float tem = dht.readTemperature();
  float hum = dht.readHumidity();

  // setField sets the value before writing to ThingSpeak
  ThingSpeak.setField(1, tem);
  ThingSpeak.setField(2, hum);

  Serial.println("Temperature is  " + String( tem ) + "C and Humidity is " + String( hum ));
  int x = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  Serial.println("Please wait while we collect the data...");
  
}
// WiFi connect functions
void connectHomeWiFi() {
  // Connecting to WiFi
  WiFi.begin(home_ssid, home_pass);
  int homeCount = 0;
  Serial.print("Connecting to network " + String(home_ssid));
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

void connectUCSIWiFi() {
  const char* wpa2e_ssid = "Connect@UCSI";
  const char* host = "ucsiuniversity.edu.my";
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
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
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
