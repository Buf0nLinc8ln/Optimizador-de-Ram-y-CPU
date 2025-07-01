# Optimizador-de-RAM-y-CPU

## Descripción del proyecto

Este proyecto permite **imprimir, filtrar y controlar procesos** del sistema con base en su uso de memoria RAM y CPU. Desarrollado en lenguaje C, se emplea una **estructura modular** y hace uso de una **GTK 4** como interfaz grafica, para que el proyecto sea interactivo.

### Funcionalidades principales:


- Filtros por:
  - PID
  - UID (número o nombre de usuario)
  - GID (número o nombre de grupo)
  - Nombre del proceso
  - PPID
  - GPID
  - Estado (S, R, Z, T, D, F o B)
  - Límite de uso de RAM
  - Límite de uso de CPU
- Control de procesos:
  - Suspender 
  - Reanudar 
  - Eliminar 
- Interfaz GTK 4:
  - Entrada de parámetros
  - Botones de control

Funciones auxiliares como:
- Verificación de enteros.
- Cálculo de porcentaje de uso de CPU.
- Manejo de listas enlazadas para almacenar procesos filtrados.



    

## Dependencias, instalación y Pasos para ejecución:
```bash
sudo apt install libgtk-4-dev libprocps-dev # instalar las dos bibliotecas usadas
make ## generar el binario
./programa # ejecutar el programa

