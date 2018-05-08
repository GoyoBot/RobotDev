#include "Robot.h"
#include "SimpleTimer.h"

Robot robot;

float angulo = 0.0;
float offset = 5.0;

SimpleTimer timer;
int timerId = -1;

void setup() {
    // Serial.begin(115200);
    Serial.begin(9600);
    robot.setup();
    // angulo = robot.anguloAbsoluto();
}

void loop() {
    robot.update();  // Siempre al comenzar el loop

	if (abs(anguloRectificacion()) > offset) { // Si no esta colocado, para y recoloca
        robot.para();
        rectificarAngulo();
    }

    // comprobarLineasLaterales();

    if (abs(anguloRectificacion()) <= offset) { // Esta comprobación es necesaria puesto que solo en caso de que mire en la dirección correcta deberá ignorar la detección de lineas frontalmente
        //comprobarFinRecorrido();
    } else {
        // comprobarLineasFrontales();
    }

    robot.avanza();

}

// Comprueba que el robot esta mirando en la dirección correcta con un offset
float anguloRectificacion() {
    float actual = robot.anguloAbsoluto();
    float original = angulo;
    if (abs(original-actual)>180){
       original+=360;
    }
   return original - actual;
}

// Para el robot y lo rota hasta que la dirección es la correcta
void rectificarAngulo() {
    float ang = anguloRectificacion();
    while (abs(ang) > offset) {
        Serial.print("ar: " + String(ang));
        if (ang > 0) {
            robot.avanzaDcha(512, 10);
            Serial.println(" DERECHA");
        } else {
            robot.avanzaIzda(512, 10);
            Serial.println(" IZQUIERDA");
        }
        ang = anguloRectificacion();
            Serial.println("anguloact: " + String(ang));
    }
}

// array que almacena la detección de la linea de los sensores
bool sensores[6] = {false, false, false, false, false, false};

// comprueba que se ha detectado una linea a través de todos los sensores por orden
// en menos de 2 segundos
void comprobarFinRecorrido() {
    if (!(sensores[2] && sensores[3])) {
        sensores[2] = robot.irSobreNegro(robot.pins.IR[3]);
        sensores[3] = robot.irSobreNegro(robot.pins.IR[4]);

        if (timerId != -1 && timer.isEnabled(timerId)) {
            timerId = timer.setTimeout(2000, resetFinRecorrido);    
        }

    } else if (!(sensores[1] && sensores[4])) {
        sensores[1] = robot.irSobreNegro(robot.pins.IR[2]);
        sensores[4] = robot.irSobreNegro(robot.pins.IR[5]);
    } else if (!(sensores[0] && sensores[5])) {
        sensores[0] = robot.irSobreNegro(robot.pins.IR[0]);
        sensores[5] = robot.irSobreNegro(robot.pins.IR[7]);

        if (sensores[0] && sensores[5]) {
            finalizarRecorido();
        }

    }

}

// Reinicia la comprobación del final del recorrido
void resetFinRecorrido() {
    for (int i = 0; i < 6; i++) {
        sensores[i] = false;
    }
}

// Para el robot hasta reiniciar arduino
void finalizarRecorido() {
    robot.para();
    while(true) {
        Serial.print("Fin!");
        delay(1000);
    }
}
