
#include <DFRobot_SHT3x.h>




DFRobot_SHT3x sht3x(&Wire, /*address=*/0x44, /*RST=*/4);
//DFRobot_SHT3x   sht3x;

float Temperatura;
float humedad;


void setup() {

  Serial.begin(9600);


  while (sht3x.begin() != 0) {
    Serial.println("Failed to Initialize the chip, please confirm the wire connection");
    delay(1000);
  }

  Serial.print("Chip serial number");
  Serial.println(sht3x.readSerialNumber());


  if (!sht3x.softReset()) {
    Serial.println("Failed to Initialize the chip....");
  }




  Serial.println("------------------Read adta in single measurement mode-----------------------");
}

void loop() {










  Temperatura = sht3x.getTemperatureC();
  humedad = sht3x.getHumidityRH();



  /**
       * getTemperatureF:Get the meansured temperature(℉).
       * @return Return float temperature data.
       */
  /*Serial.print(sht3x.getTemperatureF());
      Serial.print(" F ");
      Serial.print("Relative Humidity(%RH):");*/
  /**
       * getHumidityRH: Get the meansured humidity (%RH)
       * @return Return float humidity data
       */
  //Serial.print(sht3x.getHumidityRH());
  //Serial.println(" %RH");



  /**
       * @brief Get temperature and humidity data in single measurement mode.
       * @param repeatability Set repeatability to read temperature and humidity data with the type eRepeatability_t.
       * @note  Optional parameters:
                   eRepeatability_High /**In high repeatability mode, the humidity repeatability is 0.10%RH, the temperature repeatability is 0.06°C
                   eRepeatability_Medium,/**In medium repeatability mode, the humidity repeatability is 0.15%RH, the temperature repeatability is 0.12°C.
                   eRepeatability_Low, /**In low repeatability mode, the humidity repeatability is0.25%RH, the temperature repeatability is 0.24°C
       * @return Return a structure containing celsius temperature (°C), Fahrenheit temperature (°F), relative humidity(%RH), status code.
       * @n Return O indicates right data return.
      DFRobot_SHT3x::sRHAndTemp_t data = sht3x.readTemperatureAndHumidity(sht3x.eRepeatability_High);
      if(data.ERR == 0){
        Serial.print("Ambient Temperature(°C/F):");
        Serial.print(data.TemperatureC);
        Serial.print(" C/");
        Serial.print(data.TemperatureF);
        Serial.print(" F ");
        Serial.print("Relative Humidity(%RH):");
        Serial.print(data.Humidity);
        Serial.println(" %RH");
      }
      */
  delay(1000);

  Serial.print("temperatura ambiente (°C/F): ");
  Serial.println(Temperatura);


  Serial.print("humedad ambiente (%RH) : ");
  Serial.println(humedad);
}