long long int tiempoactual;
long long int intervalo = 1000;
long long int timer = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
tiempoactual = millis();

if(tiempoactual - timer >= intervalo){
  timer = tiempoactual;
  //Hace lo que sea



}
}
