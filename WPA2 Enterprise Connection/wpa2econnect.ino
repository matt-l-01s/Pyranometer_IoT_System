// Based on WiFiClientEnterprise example file

#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
// #include full path to mywifidetails file here.

// Variables for connecting to UCSI WiFi
#define EAP_IDENTITY eap_id //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD eap_pass //your Eduroam password
const char* wpa2e_ssid = "Connect@UCSI"; // Eduroam wpa2e_ssid
const char* host = "ucsiuniversity.edu.my"; //external server domain for HTTP connection after authentification
int wpa2e_count = 0;


void setup() {
  Serial.begin(115200);

  // Code for connecting to UCSI WiFi
  delay(10);
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(wpa2e_ssid);
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
    if(wpa2e_count>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
}
void loop() {
  if (WiFi.status() == WL_CONNECTED) { //if we are connected to Eduroam network
    wpa2e_count = 0; //reset wpa2e_count
    Serial.println("Wifi is still connected with IP: "); 
    Serial.println(WiFi.localIP());   //inform user about his IP address
  }else if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
    WiFi.begin(wpa2e_ssid);      
  }
  while (WiFi.status() != WL_CONNECTED) { //during lost connection, print dots
    delay(500);
    Serial.print(".");
    wpa2e_count++;
    if(wpa2e_count>=60){ //30 seconds timeout - reset board
    ESP.restart();
    }
  }
  delay(1000);
}
