## Tarea 1 - Corte 2° - Creación de Dataset: Para identificar gestos 

**Asignatura:** IABO - Inteligencia Artificial de Borde  
**Institución:** Escuela Colombiana de Ingeniería Julio Garavito  
**Autores:** Santiago Quintero, Juliana Ómbita y Juan Gómez  

---

## 📌 Descripción de la tarea:
La tarea implementa un flujo de procesamiento completo (*pipeline*) para la captura, procesamiento y clasificación de 5 clases de gestos utilizando las lecturas inerciales de un microcontrolador Arduino Nano 33 BLE Sense. 

A partir de las señales continuas del acelerómetro y giroscopio de 6 ejes ($aX, aY, aZ, gX, gY, gZ$), se estructuran ventanas temporales de 119 muestras. De cada ventana se extraen 36 características estadísticas (6 por eje), las cuales se normalizan con *Z-score* y se reducen mediante Análisis de Componentes Principales (PCA). Finalmente, se evalúan modelos geométricos de clasificación como la distancia a centroides y $k$-Vecinos Más Cercanos ($k$-NN).

---

## 🛠️ Requisitos del sistema

### Hardware
* **Microcontrolador:** Arduino Nano 33 BLE Sense.
* **Librería de sensor:** `Arduino_BMI270_BMM150`.
* **Conexión:** Cable USB a serie (Baud rate: 9600).

### Software y librerías
* **Arduino IDE** (para compilación y carga del código C++).
* **Python 3.x** con los siguientes paquetes:
  ```bash
  pip install pyserial numpy pandas scikit-learn matplotlib**

## 🚀 Guía de ejecución (Paso a Paso)

> ⚠️ **IMPORTANTE:** Para garantizar una comunicación serial correcta y evitar errores de conexión en el puerto COM, **debes cargar y ejecutar primero el código C++ en la placa Arduino antes de iniciar el script de Python**.

### Paso 1: Carga del código embebido en el Arduino (C++)
1. Conecta el Arduino Nano 33 BLE Sense a tu computadora vía USB.
2. Abre el software **Arduino IDE**.
3. Carga el firmware C++ que gestiona la lectura de la IMU y el protocolo de negociación (*handshake*) mediante los comandos `START` y `END`.
4. Selecciona la placa y el puerto COM correspondiente.
5. Compila y sube (*Upload*) el programa al microcontrolador.
6. Cierra el **Serial Monitor** de Arduino IDE para liberar el puerto COM.

### Paso 2: Ejecución del script de captura en Python
1. Abre tu terminal o entorno de desarrollo en Python.
2. Asegúrate de configurar la variable `PUERTO` en el script con el puerto COM detectado para tu Arduino (ej. `'COM8'`)[cite: 1].
3. Ejecuta el script de automatización de captura en Python[cite: 1]:
   ```bash
   python capturar_dataset.py
