#include "Arduino_BMI270_BMM150.h"

const int NUM_SAMPLES = 119;
int samplesRead = 0;

void setup() {
  Serial.begin(9600);
  
  // Esperar el puerto serie máximo 3 segundos, luego avanzar de todas formas.
  // Esto evita que el USB de la placa colapse y Windows deje de reconocerla.
  unsigned long start = millis();
  while (!Serial && millis() - start < 3000); 

  // Inicializar la IMU de forma segura
  if (!IMU.begin()) { 
    Serial.println("ERROR_IMU"); 
    // En lugar de congelar la placa, la dejamos seguir para no perder el USB
  } else {
    Serial.println("READY");
  }
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd == "START") {
      float aX, aY, aZ, gX, gY, gZ;
      samplesRead = 0;
      
      while (samplesRead < NUM_SAMPLES) {
        if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
          IMU.readAcceleration(aX, aY, aZ);
          IMU.readGyroscope(gX, gY, gZ);
          
          Serial.print("unlabeled,"); 
          Serial.print(aX, 4); Serial.print(",");
          Serial.print(aY, 4); Serial.print(",");
          Serial.print(aZ, 4); Serial.print(",");
          Serial.print(gX, 4); Serial.print(",");
          Serial.print(gY, 4); Serial.print(",");
          Serial.println(gZ, 4);
          samplesRead++;
        }
      }
      Serial.println("END");
    }
  }
}
