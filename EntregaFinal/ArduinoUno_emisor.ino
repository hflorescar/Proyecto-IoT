#include <Config.h>
#include <EasyBuzzer.h>



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

byte validKey1[4] = { 0x43, 0xD3, 0xBC, 0x94 };  // Ejemplo de clave valida



//Función para comparar dos vectores
bool isEqualArray(byte* arrayA, byte* arrayB, int length)
{
  for (int index = 0; index < length; index++)
  {
    if (arrayA[index] != arrayB[index]) return false;
  }
  return true;
}

//const byte pinBuzzer = 5;
long long int tiempoactual;
long long int intervalo = 1000;
long long int timer = 0; 

void setup() {
  Serial.begin(115200); // Iniciar la comunicación serial a 9600 baudios
  SPI.begin();        // Iniciar SPI bus
  mfrc522.PCD_Init(); // Iniciar MFRC522


}



void loop() {

  if (mfrc522.PICC_IsNewCardPresent())
  {
     //Seleccionamos una tarjeta
    if (mfrc522.PICC_ReadCardSerial())
    {
      // Comparar ID con las claves válidas
      if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4))
        {
          //Serial.println("Tarjeta valida");
           for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
  }
        //analogWrite(pinBuzzer, 100);
        EasyBuzzer.beep(1500);
        delay(1000);
        EasyBuzzer.stopBeep();
        }
      else
       { //Serial.println("Tarjeta invalida");
       //analogWrite(pinBuzzer, 200);
       EasyBuzzer.beep(950);
        delay(1000);
        EasyBuzzer.stopBeep();
       }
 mfrc522.PICC_HaltA();
    }
  }

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
 /* for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
  }
  //Serial.println();
  // Detectar tarjeta
*/
}
}

 


  