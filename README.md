# Generador de Hidrogramas

Este programa en C++ permite generar hidrogramas de caudal a partir de ciertos parámetros ingresados por el usuario. El hidrograma se calcula utilizando el método de SCS (Servicio de Conservación de Suelos de los Estados Unidos).

## Requisitos

- Compilador de C++ compatible con el estándar C++11 o superior.
- Sistema operativo compatible con la ejecución de programas en C++.

## Funcionalidades

- Cálculo del hidrograma de caudal a partir de los siguientes parámetros:
  - Caudal de punta (Qp).
  - Tiempo de concentración (tc).
  - Caudal base (Qb).
  - Factor de descarga del hidrograma.
  - Intervalo de tiempo para calcular el hidrograma.

## Instrucciones de Uso

1. Clona o descarga este repositorio en tu sistema local.
2. Abre una terminal en el directorio donde se encuentra el archivo `generador_hidrogramas.cpp`.
3. Compila el programa utilizando un compilador de C++ compatible.
   ```bash
   g++ generador_hidrogramas.cpp -o generador_hidrogramas

## Instrucciones de Uso
4. Ejecuta el programa generado.
   ```bash
   ./generador_hidrogramas

5. El programa generará un archivo DAT llamado `datos.DAT` con los resultados del hidrograma, este archivo se puede usar para cargarlo en SWMM como una serie de datos de entrada.

## Contribuciones

Las contribuciones son bienvenidas. Si encuentras algún problema o tienes alguna mejora, no dudes en abrir un issue o enviar un pull request.


