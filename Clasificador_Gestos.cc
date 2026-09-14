#include <Arduino_BMI270_BMM150.h>

const int NUM_SAMPLES = 119;
int sampleCount = 0;

// Búferes para almacenamiento de señales
float gX_buffer[NUM_SAMPLES];
float gY_buffer[NUM_SAMPLES];
float gZ_buffer[NUM_SAMPLES];
float aY_buffer[NUM_SAMPLES];

// Árbol de Decisión en C++ (Hiperparámetros optimizados)
int clasificarGesto(float gX_energia, float gZ_varianza, float aY_energia, float gY_varianza) {
    if (gX_energia <= 3268.46f) {
        return 1; // Reposo
    } else {
        if (gZ_varianza <= 2775.89f) {
            if (aY_energia <= 3.31f) {
                return 4; // Desplazamiento Vertical
            } else {
                return 3; // Desplazamiento Adelante y Atrás
            }
        } else {
            if (gY_varianza <= 1138.13f) {
                return 2; // Desplazamiento Horizontal
            } else {
                return 5; // Rotación
            }
        }
    }
}

// Mapeo HMI
const char* obtenerNombreGesto(int idGesto) {
    switch (idGesto) {
        case 1: return "Reposo";
        case 2: return "Desplazamiento Horizontal";
        case 3: return "Desplazamiento Adelante y Atras";
        case 4: return "Desplazamiento Vertical";
        case 5: return "Rotacion";
        default: return "Desconocido";
    }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("¡Error al inicializar la IMU!");
    while (1);
  }

  Serial.println("=================================================");
  Serial.println("  CLASIFICADOR DE GESTOS EN TIEMPO REAL (GINI)  ");
  Serial.println("=================================================");
  Serial.println("SISTEMA LISTO. Realice un gesto con la placa...");
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    aY_buffer[sampleCount] = ay;
    gX_buffer[sampleCount] = gx;
    gY_buffer[sampleCount] = gy;
    gZ_buffer[sampleCount] = gz;

    sampleCount++;

    if (sampleCount >= NUM_SAMPLES) {
      float gX_energia = 0.0f;
      float aY_energia = 0.0f;
      float gY_suma = 0.0f;
      float gZ_suma = 0.0f;

      for (int i = 0; i < NUM_SAMPLES; i++) {
        gX_energia += gX_buffer[i] * gX_buffer[i];
        aY_energia += aY_buffer[i] * aY_buffer[i];
        gY_suma += gY_buffer[i];
        gZ_suma += gZ_buffer[i];
      }

      float gY_media = gY_suma / NUM_SAMPLES;
      float gZ_media = gZ_suma / NUM_SAMPLES;

      float gY_varianza = 0.0f;
      float gZ_varianza = 0.0f;

      for (int i = 0; i < NUM_SAMPLES; i++) {
        float diffY = gY_buffer[i] - gY_media;
        float diffZ = gZ_buffer[i] - gZ_media;
        gY_varianza += diffY * diffY;
        gZ_varianza += diffZ * diffZ;
      }

      gY_varianza /= NUM_SAMPLES;
      gZ_varianza /= NUM_SAMPLES;

      int idGesto = clasificarGesto(gX_energia, gZ_varianza, aY_energia, gY_varianza);
      const char* nombreGesto = obtenerNombreGesto(idGesto);

      Serial.print("GESTO DETECTADO: [ ");
      Serial.print(nombreGesto);
      Serial.print(" ] (ID: ");
      Serial.print(idGesto);
      Serial.print(") | gX_E: ");
      Serial.print(gX_energia, 1);
      Serial.print(" | gZ_Var: ");
      Serial.print(gZ_varianza, 1);
      Serial.print(" | aY_E: ");
      Serial.print(aY_energia, 2);
      Serial.print(" | gY_Var: ");
      Serial.println(gY_varianza, 1);

      sampleCount = 0;
    }
  }
}
