#include <ThingSpeak.h>
#include <DHT.h>
#include <String.h>
#include <NTP.h>

// For connecting to WiFi
#include <WiFi.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <C:\Users\Matthew\Documents\Arduino\libraries\mywifidetails\mywifidetails.h>
#define EAP_IDENTITY eap_id //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD eap_pass //your Eduroam password
int modeWiFi;

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
NTP ntp(ntpUDP);
unsigned long prevTime = 0;
unsigned long delayTime = 59;
unsigned long prevNTP = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Please type '1' to connect home wifi, '2' to connect hotspot or 3 to connect uni wifi.");
  while (Serial.available() == 0) {

    modeWiFi = Serial.parseInt(); // while loop repeats this line until int detected in serial monitor
    if (modeWiFi != 0) {
      Serial.println(String(modeWiFi));
      switch (modeWiFi) {
        case 1:
          connectWiFi(home_ssid, home_pass);
          break;
        case 2:
          connectWiFi(hots_ssid, hots_pass);
          break;
        case 3:
          connectUCSIWiFi();
          break;
      }
    }
  }

  dht.begin();
  ntp.ruleSTD("+08", Last, Tue, Jun, 0, 0);
  ntp.begin();
  ThingSpeak.begin(client);
  if (ThingSpeak.begin(client)) {
    Serial.println("ThingSpeak connection established.");
//    sendData();
  }
}

void loop() {
  ntp.update();
  prevNTP = ntp.seconds();
//  Serial.println(prevNTP);
  if (prevNTP == delayTime) {
    sendData();
  }
}

void sendData() {
  float tem = dht.readTemperature();
  float hum = dht.readHumidity();
  String timeStamp = "At " + String(ntp.formattedTime("%A %T"));

  // setField sets the value before writing to ThingSpeak
  ThingSpeak.setField(1, tem);
  ThingSpeak.setField(2, hum);

  Serial.println(timeStamp + ", temperature is  " + String( tem ) + "C and Humidity is " + String( hum ));
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
  esp_wifi_sta_wpa2_ent_enable(); // Changed 20/7/22 due to unknown issues
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
