const int sensorPin = 36;
 int sensorValue = 0;
 int presencia = 0;
 const int led = 12;

 static int value = 1000;

void setup() {
 Serial.began(9600);
 pinMode(sensorPin,INPUT);
 pinMode(led, OUTPUT);
  }

void loop() {
sensorValue = analogRead(sensorPin);
if (sensorValue <= 800 || sensorValue >= 3200){Serial.print("Persona detectada");
Presencia = 1;
} 
else{
  presencia = 0;
  }
  serial.print(sensorValue)
if (Presencia==1){
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led,LOW);
}
else{
  digitalwrite(led,LOW);
}
}
