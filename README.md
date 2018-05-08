# DAM IT!

## Instalación

Es necesario instalar la biblioteca `NewPing` desde la GUI de Arduino: menú `Sketch > Include Library > Manage Libraries...`


## Overview

Clases:

- `Robot`: es la principal que gestiona todo el hardware del robot (sensores/actuadores).
- `Motor`: controla un motor.
- `UltraSound`: controla un sensor de ultra-sonido.

En un programa arduino sólo es necesario crear un objeto `Robot`.
Es necesario **llamar siempre a `update()` al comenzar el `loop()`**.

    #include "Robot.h"
    Robot robot;

    void setup() {
        robot.setup();
    }

    void loop() {
        robot.update(); // Siempre al comenzar el loop
        robot.avanza();
        delay(100);
    }


## Movimiento

Los siguientes métodos configuran el estado de los motores. Terminan en seguida, por lo que es necesario poner un `delay()` después para dar tiempo a que el robot se mueva:

- `avanza()`
- `retrocede()`
- `para()`
- `avanzaIzda()`
- `avanzaDcha()`

Se puede pasar también el tiempo:

    avanza(1000);
    // Equivalente a:
    avanza();
    delay(1000);


## Sensores ultra sonido

Ya no usamos la clase `SR04`, sino `NewPing` (instalar manualmente desde Arduino).

La clase `UltraSound` gestiona los sensores y filtra los datos para mejorar las medidas.

La clase `Robot` incluye los tres sensores, y se puede leer el valor con:

    // Distancia que mide el sensor izquierdo (I)
    robot.ultraSonidoI.distancia();


