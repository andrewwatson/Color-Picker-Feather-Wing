#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>

#define WLAN_SSID       "WIFI SSID"
#define WLAN_PASS       "WIFI PASSWORD"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "" // Put your adafruit.io username here
#define AIO_KEY         "" // Put your adafruit.io key here

/********************* NeoPixel Matrix Setup *********************************/
int matrixPin = 4;
int columns = 8;
int rows = 4;

int lastPin = (columns * rows - 1);
  
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(columns, rows, matrixPin,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
  
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe colorpicker = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/colors");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  matrix.begin();
  matrix.setBrightness(100);
  matrix.fillScreen(0);
  matrix.show();
  
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  displayOn();
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&colorpicker);
}

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &colorpicker) {
      Serial.print(F("Got: "));
      String lastRead = (char *)colorpicker.lastread;

      Serial.printf("RAW: %s\n", lastRead.c_str());
      
      String redHex = lastRead.substring(1,3);
      int red = hexToDec(redHex);
      Serial.printf("RED: %s %d\n", redHex.c_str(), red);

      String greenHex = lastRead.substring(3,5);
      int green = hexToDec(greenHex);
      Serial.printf("GREEN: %s %d\n", greenHex.c_str(), green);

      String blueHex = lastRead.substring(5,7);
      int blue = hexToDec(blueHex);
      Serial.printf("BLUE: %s %d\n", blueHex.c_str(), blue);

      matrix.fillScreen(matrix.Color(red, green, blue));
      matrix.show();
      delay(1000);
  
      // back to black
      matrix.fillScreen(0);
      matrix.show();
    }
  }


  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


unsigned int hexToDec(String hexString) {
  
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}

void displayOn() {
  matrix.setBrightness(25);
  matrix.fillScreen(matrix.Color(0,0,0));
  green(1);
  green(2);
  green(4);
  green(7);
  green(8);
  green(11);
  green(12);
  green(13);
  green(15);
  green(16);
  green(19);
  green(20);
  green(22);
  green(23);
  green(25);
  green(26);
  green(28);
  green(31);
  matrix.show();
  delay(1000);
  matrix.fillScreen(0);
  matrix.show();
}

void green(int pixel) {
  matrix.setPixelColor(pixel, 0, 255, 0);
}
