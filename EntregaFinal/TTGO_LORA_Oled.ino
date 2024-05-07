// Librerias sensor temperatura.
#include <Adafruit_BME280.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>

//Librarias LoRa
#include <SPI.h>
#include <LoRa.h>

//Librarias display Oled
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//Definir los pins usados para el modulo LoRa transceiver
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

//433E6 para Asia
//866E6 para Europe
//915E6 para North America
#define BAND 766E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST 25
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define RX_PIN 4 
//Contador de paquetes
int counter = 0;
int Lecture=0;
int cont = 0;
int ciclos=0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
HardwareSerial MySerial(1);
//Sensor
 Adafruit_BME280 bme; // use I2C interface
 Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
 Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
 Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

const int sensorPin = 36;
 int sensorValue = 0;
 int presencia = 0;
 const int led = 12;

 static int value = 1000;

void setup() {
  Serial.begin(115200);
  Serial.println(F("BME280 Sensor event test"));
  MySerial.begin(115200, SERIAL_8N1, RX_PIN, -1);
  if (!bme.begin()) {
    Serial.println(F("No se ha encontrado el sensor BME280"));
    while (1) delay(10);
  }
  
  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
  
 pinMode(sensorPin,INPUT);
 pinMode(led, OUTPUT);

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  
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
  display.print("LORA SENDER ");
  display.display();
  
  Serial.println("LoRa Sender Test");

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
  display.print("LoRa Initializing OK!");
  display.display();
  delay(100);
}

void loop() {

  sensorValue = analogRead(sensorPin);
if (sensorValue >= 100 || sensorValue <= 3900){
cont++;
ciclos++;
presencia = 1;
} 
else{
  presencia = 0;
  ciclos++;
  }



  
if(cont >= 399 && ciclos <= 400){
  presencia=1;
  ciclos = 0;
  cont = 0;
}
  else if (cont >= 400 && presencia == 0 ){
    presencia=1;
  ciclos = 0;
  cont = 0;
  }


  if(presencia==1){
  digitalWrite(led, HIGH);
  delay(5000);
  digitalWrite(led,LOW);
  
}
 else{
  digitalWrite(led,LOW);
}
   
   String message = "";
  while (MySerial.available()) {
    message = MySerial.readString();
   Lecture = 1;
  }

  if (message.length() > 0) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(message);
    display.display();
  }
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  Serial.print(F("Temperatura = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Humedad = "));
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print(F("Presion = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");
  Serial.print(message);
  Serial.println();
  delay(1000);
  
  //Mandar datos por Lora
  
  Serial.print("Sending packet: ");
  Serial.println(counter);
  
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  //LoRa.print(F("Temperatura = "));
  LoRa.print(temp_event.temperature);
  LoRa.print(".");
  //LoRa.println(" *C");

  //LoRa.print(F("Humedad = "));
  LoRa.print(humidity_event.relative_humidity);
  LoRa.print(".");
  //LoRa.println(" %");

  //LoRa.print(F("Presion = "));
  LoRa.print(pressure_event.pressure/10);
  LoRa.print(".");
  
  LoRa.beginPacket();
  //LoRa.println(" hPa");
   if(Lecture){
    Serial.println("Envio lectura");
    //LoRa.beginPacket();
    LoRa.print(75);
    LoRa.print(".");
    LoRa.print(message);
    //LoRa.endPacket();
    Lecture=0;
  }
   else{
     //LoRa.print(F("Temperatura = "));
  LoRa.print(temp_event.temperature);
  LoRa.print(".");
  //LoRa.println(" *C");

  //LoRa.print(F("Humedad = "));
  LoRa.print(humidity_event.relative_humidity);
  LoRa.print(".");
  //LoRa.println(" %");

  //LoRa.print(F("Presion = "));
  LoRa.print(pressure_event.pressure/10);
  LoRa.print(".");
   }
  //LoRa.print(counter);
  LoRa.endPacket();

/*
  if(Lecture){
    Serial.println("Envio lectura");
    LoRa.beginPacket();
    LoRa.print(75);
    LoRa.print(".");
    LoRa.print(message);
    LoRa.endPacket();
    Lecture=0;
  }*/
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,12);
  display.setTextSize(0.5);
  display.print("LoRa packet sent.");
  display.setCursor(0,22);
  display.setTextSize(0.5);
  display.print("Counter:");
  display.setCursor(50,22);
  display.setTextSize(0.5);
  display.print(counter);
  
  display.setCursor(0,35);
  display.print(F("Temp.= "));
  display.setCursor(37,35);
  display.print(temp_event.temperature);
  display.setCursor(64,35);
  display.print(" C");

  display.setCursor(0,45);
  display.print(F("Hum. = "));
  display.setCursor(37,45);
  display.print(humidity_event.relative_humidity);
  display.setCursor(64,45);
  display.print(" %");

    display.setCursor(0,55);
  display.print(F("Pres.= "));
  display.setCursor(37,55);
  display.print(pressure_event.pressure);
  display.setCursor(68,55);
  display.print(" hPa");

  display.display();

  counter++;
  
  delay(2000);
  
}
