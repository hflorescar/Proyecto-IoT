const int sensorPin = 36;
 int sensorValue = 0;
 int presencia = 0;
 const int led = 12;

 static int value = 1000;

void setup() {
 Serial.begin(9600);
 pinMode(sensorPin,INPUT);
 pinMode(led, OUTPUT);
  }

void loop() {
sensorValue = analogRead(sensorPin);
if (sensorValue <= 800 || sensorValue >= 3200){
 Serial.print("Persona detectada");
presencia = 1;
} 
else{
  presencia = 0;
  }
  Serial.print(sensorValue);
if(presencia==1){
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led,LOW);
}
 else{
  digitalWrite(led,LOW);
}
}
