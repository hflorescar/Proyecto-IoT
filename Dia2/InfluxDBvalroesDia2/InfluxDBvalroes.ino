#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
  #elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
  #endif
  
#include <Ticker.h>

#include <SPI.h>
#include <LoRa.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

#define BAND 766E6

#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST 25
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define WIFI_SSID "Prueba"
#define WIFI_PASSWORD "12345678"

  #define INFLUXDB_URL "https://eu-central-1-1.aws.cloud2.influxdata.com"
  #define INFLUXDB_TOKEN "xFNI6GZ4ELD-x7miXcAi-Tr5J2yWj0VjU9NSrw1nOXfmZ18DrQ7EfuucFY-_TwxeTOVis7nPAFFnlNZ3rphyYQ=="
  #define INFLUXDB_ORG "09f459e63b442907"
  #define INFLUXDB_BUCKET "Prueba"

#define TZ_INFO "UTC1"

//SEPARAR STRING
#define DEBUG_ARRAY(a) {for (int index = 0; index < sizeof(a) / sizeof(a[0]); index++)   {Serial.print(a[index]); Serial.print('\t');} Serial.println();};

String str = "";
#define separador '.'
const int dataLength = 15;
int data[dataLength];
float div1;
float temp;
float temp1;
float temp2;
float div2;
float hum;
float hum1;
float hum2;
float div3;
float pres;
float pres1;
float pres2;

int flagInfluxDB = 0;
// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  
  // Declare Data point
  Point sensor("wifi_status");

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData;

Ticker timer;

void setup() {
  // put your setup code here, to run once:
//initialize Serial Monitor
  Serial.begin(115200);
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA RECEIVER ");
  display.display();

  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();

   // Setup wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.print("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
    Serial.println();
  
    // Accurate time is necessary for certificate validation and writing in batches
    // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  
  
    // Check server connection
    if (client.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(client.getServerUrl());
    } else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(client.getLastErrorMessage());
    }
  timer.attach_ms(10000, mandarInflux);
}

void loop() {
  // put your main code here, to run repeatedly:

  

    //try to parse packet
  int packetSize = LoRa.parsePacket();
  /*if (packetSize) {
    //received a packet
    Serial.print("Received packet ");

    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
    }
  }*/
    // separar string
 if (packetSize) {
  while (LoRa.available()) {
    str = LoRa.readString();
    //Serial.print(str);
  }

  Serial.println();
    for (int i = 0; i <= packetSize ; i++)
    {
      
      int index = str.indexOf(separador);
      data[i] = str.substring(0, index).toInt();
      str = str.substring(index + 1);
      
    }
    DEBUG_ARRAY(data);
 

  float(temp1)=data[0];
  float(temp2)=data[1];
  float(div1)=float(temp2)/float(100);
  float(temp)=float(temp1)+float(div1);
  float(hum1)=data[2];
  float(hum2)=data[3];
  float(div2)=float(hum2)/float(100);
  float(hum)=float(hum1)+float(div2);
  float(pres1)=data[4];
  float(pres2)=data[5];
  float(div3)=float(pres2)/float(100);
  float(pres)=float(pres1)+float(div3);
  /*Serial.print(float (temp));
  Serial.println(" ºC");
  Serial.print(float (hum));
  Serial.println(" %");
  Serial.print(float (pres));
  Serial.println(" Kpa");
 */
    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");    
    Serial.println(rssi);
/*
   // Dsiplay information
   display.clearDisplay();
   display.setCursor(0,0);
   display.print("LORA RECEIVER");
   display.setCursor(0,20);
   display.print("Received packet:");
   display.setCursor(0,30);
   display.print(LoRaData);
   display.setCursor(0,40);
   display.print("RSSI:");
   display.setCursor(30,40);
   display.print(rssi);
   display.display();   
   */
 }
   //delay(1);
  if(flagInfluxDB){   

  // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();

// Store measured value into point
    // Report RSSI of currently connected network
    //sensor.addField("rssi", WiFi.RSSI());
    sensor.addField("Temperatura",temp);
    sensor.addField("Humedad",hum);
    sensor.addField("Presion",pres);
    
  
    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());


  
  
    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connection lost");
    }
  
    // Write point
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }

  flagInfluxDB = 0;
  }
}
void mandarInflux() {
  flagInfluxDB = 1;
}



  


