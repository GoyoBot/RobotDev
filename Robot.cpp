#include <Arduino.h>
#include <Wire.h>
#include "Robot.h"

/// Configura todo el robot estableciendo los valores de config de sensores y motores
Robot::Robot(RobotPins p):
	ultraSonidoI ( UltraSound(p.US.I.echo, p.US.I.trigger) ),
	ultraSonidoC ( UltraSound(p.US.C.echo, p.US.C.trigger) ),
	ultraSonidoD ( UltraSound(p.US.D.echo, p.US.D.trigger) ),
	motorI ( Motor(p.motor.IN1.I, p.motor.IN2.I, p.motor.PWM.I, p.motor.offset.I, p.motor.STBY) ),
	motorD ( Motor(p.motor.IN1.D, p.motor.IN2.D, p.motor.PWM.D, p.motor.offset.D, p.motor.STBY) ),
	mpu6050 ( Wire ),
	pins ( p )
{
}

void Robot::setup() {
	// Setup I2C connection for mpu6050
	Wire.begin();
	
	mpu6050.begin();
	mpu6050.calcGyroOffsets(true);
}

void Robot::update() {
	ultraSonidoI.update();
	ultraSonidoC.update();
	ultraSonidoD.update();
	mpu6050.update();
}

void Robot::avanza(unsigned long time) {
	motorI.drive(_velocidad);
	motorD.drive(_velocidad);
	delay(time);
}

void Robot::retrocede(unsigned long time) {
	motorI.drive(-_velocidad);
	motorD.drive(-_velocidad);
	delay(time);
}

void Robot::para(unsigned long time) {
	motorI.brake();
	motorD.brake();
	delay(time);
}

/// TODO: revisar esto
void Robot::avanzaIzda(unsigned int giro, unsigned long time) {
	giro = constrain(giro, 0, 255);
	motorI.drive(_velocidad - giro);
	motorD.drive(_velocidad);
	delay(time);
}

/// TODO: revisar esto
void Robot::avanzaDcha(unsigned int giro, unsigned long time) {
	giro = constrain(giro, 0, 255);
	motorI.drive(_velocidad);
	motorD.drive(_velocidad - giro);
	delay(time);
}

void Robot::avanzaGiro(int giro, unsigned long time) {
	if (giro > 0) {
		avanzaDcha(giro, time);
	} else {
		avanzaIzda(giro, time);
	}
}

/**
 * Gira a velocidad constante durante un tiempo definido.
 * Muy poco preciso: según se agotan las pilas, la velocidad de los motores es menor.
 */
/*
void Robot::giraIzda90() {
	// Valores de velocidad y delay por ensayo-error
	const int vel = 70;
	motorI.drive(-vel);
	motorD.drive(vel);
	delay(500);
}
*/

/**
 * Gira el robot cierto `anguloGiro` (positivo dcha, negativo izda)
 * El algoritmo
 */
void Robot::giraAngulo(float anguloGiro) {
	DEBUG_PRINTLN("## giraAngulo " + String(anguloGiro));

	const unsigned long initMillis = millis();
	const float anguloInicial = angulo();
	const float anguloFinal = anguloInicial + anguloGiro;
	const float minAngulo = 2.0;
	
	while (abs(angulo() - anguloFinal) > minAngulo) {
		const float anguloRestante = angulo() - anguloFinal;
		
		/* Multiplicar el ángulo por un factor para obtener la velocidad.
		 * Así, cuanto más ángulo nos quede por girar, más velocidad habrá,
		 * y si queda poco ángulo giramos más lento para mayor precisión.
		 * Ej: max velocidad = 255. Ángulo 90° a max velocidad => factor = 255/90 = 2.83
		 *     Pero 2.83 resulta demasiado rápido. Con factor de 1.0 es ok.
		 */
		int velocidad = anguloRestante * 1.0;
		
		// A menos de cierta velocidad los motores no se mueven
		if (velocidad > 0) {
			velocidad = max(velocidad, 70);
		} else {
			velocidad = min(velocidad, -70);
		}
		velocidad = constrain(velocidad, -255, 255);
		
		DEBUG_PRINTLN("## giraAngulo a: " + String(anguloRestante) + "\tv: " + String(velocidad));
		motorI.drive(-velocidad);
		motorD.drive(velocidad);
		delay(50);
		mpu6050.update();
	}
	DEBUG_PRINTLN("## giraAngulo fin");
}

void Robot::giraIzda90() {
	giraAngulo(-90.0);
}

void Robot::giraDcha90() {
	giraAngulo(90.0);
}

void Robot::gira180() {
	giraAngulo(180.0);
}

void Robot::setVelocidad(unsigned int velocidad) {
	_velocidad = constrain(velocidad, 0, 255);
}

bool Robot::irSobreNegro(int ir) {
	int value = analogRead(ir);
	if (value < pins.IR_threshold) {
		return true;
	} else {
		return false;
	}
}

bool Robot::irSobreBlanco(int ir) {
	return ! irSobreNegro(ir);
}

int Robot::irData(int ir) {
	return analogRead(ir);
}

float Robot::angulo() {
	// Ajuste del signo según colocación del sensor.
	return -1.0 * mpu6050.getAngleZ();
}

void Robot::printUltraSonido() {
	Serial.print(ultraSonidoI.distancia());
	Serial.print("\t");
	Serial.print(ultraSonidoC.distancia());
	Serial.print("\t");
	Serial.print(ultraSonidoD.distancia());
}

void Robot::printInfraRed() {
	for (int i = 0; i < 7; ++i) {
		Serial.print(irSobreNegro(pins.IR[i]));
		Serial.print(" ");
	}
	Serial.print(irSobreNegro(pins.IR[7]));
}

void Robot::printInfraRedRawData() {
	for (int i = 0; i < 7; ++i) {
		Serial.print(irData(pins.IR[i]));
		Serial.print("\t");
	}
	Serial.print(irData(pins.IR[7]));
}

