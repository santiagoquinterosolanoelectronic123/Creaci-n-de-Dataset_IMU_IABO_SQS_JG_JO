import serial
import time
import os

PUERTO = 'COM8'  # Tu puerto verificado
BAUD_RATE = 9600

def capturar_dataset():
    print("=== CAPTURA AUTOMATIZADA DE DATASET ===")
    num_gesto = int(input("Número de gesto a grabar (1 al 5): "))
    total_ventanas = int(input("¿Cuántas ventanas tomarás para este gesto? (Recomendado: 10 a 15): "))

    try:
        print(f"\nAbriendo comunicación en {PUERTO}...")
        arduino = serial.Serial(PUERTO, BAUD_RATE, timeout=1)
        
        # Estabilización de USB Nativo
        arduino.dtr = False
        arduino.rts = False
        time.sleep(0.5)
        arduino.dtr = True
        arduino.rts = True
        time.sleep(2)
        
        print("Conexión establecida exitosamente.")

        for v in range(1, total_ventanas + 1):
            ruta = f'data/raw/gesto_{num_gesto}/ventana_{v:02d}.csv'
            os.makedirs(os.path.dirname(ruta), exist_ok=True)
            
            input(f"\n[Gesto {num_gesto} | Ventana {v}/{total_ventanas}] Prepara tu mano y presiona ENTER...")
            print("Capturando datos...")
            
            arduino.reset_input_buffer()
            arduino.write(b"START\n")
            arduino.flush()
            
            with open(ruta, "w") as f:
                f.write("label,aX,aY,aZ,gX,gY,gZ\n")
                registros = 0
                tiempo_inicio = time.time()
                
                while True:
                    if arduino.in_waiting > 0:
                        linea = arduino.readline().decode('utf-8', errors='ignore').strip()
                        
                        if linea == "END":
                            print(f" -> Éxito: Guardada ventana {v} ({registros} muestras en {ruta})")
                            break
                        elif linea and linea not in ["READY", "ERROR", "ERROR_IMU"]:
                            f.write(linea + "\n")
                            registros += 1
                    
                    # Reintento de START si pasan 3 segundos sin respuesta
                    if time.time() - tiempo_inicio > 3:
                        print(" [Reenviando START...]")
                        arduino.write(b"START\n")
                        arduino.flush()
                        tiempo_inicio = time.time()

        print(f"\n¡FINALIZADO! Se capturaron las {total_ventanas} ventanas del Gesto {num_gesto}.")

    except Exception as e:
        print(f"Error de conexión: {e}")
    finally:
        if 'arduino' in locals() and arduino.is_open:
            arduino.close()
            print("Puerto cerrado correctamente.")

if _name_ == "_main_":
    capturar_dataset()
