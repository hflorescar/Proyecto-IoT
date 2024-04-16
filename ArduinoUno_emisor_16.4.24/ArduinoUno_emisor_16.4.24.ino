/*  Proyecto IoT industrial
Investigando un poco las comunicaciones
Esta parte es la lectura de la tarjeta desde el Arduino,
por ahora solo envio el UID al lilygo */

//Incluimos la libreria del lector RFID
#include <MFRC522.h>

//Definimos los pines SS Y RST del lector RFID 
#define RST_PIN         9          
#define SS_PIN          10   

// Creamos la instancia MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN);  

long long int tiempoactual;
long long int intervalo = 1000;
long long int timer = 0;

void setup() {
  Serial.begin(9600); // Iniciar la comunicación serial a 9600 baudios
  SPI.begin();        // Iniciar SPI bus
  mfrc522.PCD_Init(); // Iniciar MFRC522
}

void loop() {

tiempoactual = millis();

if(tiempoactual - timer >= intervalo){
  timer = tiempoactual;
  // Buscar tarjetas
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Seleccionar una de las tarjetas
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Enviar UID de la tarjeta
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
  }
  Serial.println();
}
}