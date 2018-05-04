#include "Robot.h"
#include <Arduino.h>

/// Configura todo el robot estableciendo los valores de config de sensores y motores
Robot::Robot(RobotPins p):
	ultraSonidoI ( UltraSound(p.US.I.echo, p.US.I.trigger) ),
	ultraSonidoC ( UltraSound(p.US.C.echo, p.US.C.trigger) ),
	ultraSonidoD ( UltraSound(p.US.D.echo, p.US.D.trigger) ),
	motorI ( Motor(p.motor.IN1.I, p.motor.IN2.I, p.motor.PWM.I, p.motor.offset.I, p.motor.STBY) ),
	motorD ( Motor(p.motor.IN1.D, p.motor.IN2.D, p.motor.PWM.D, p.motor.offset.D, p.motor.STBY) ),
	pins (p)
{
}

void Robot::update() {
	ultraSonidoI.update();
	ultraSonidoC.update();
	ultraSonidoD.update();
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

void Robot::giraIzda90() {
	// Valores de velocidad y delay por ensayo-error
	const int vel = 70;
	motorI.drive(-vel);
	motorD.drive(vel);
	delay(500);
}

void Robot::giraDcha90() {
	const int vel = 70;
	motorI.drive(vel);
	motorD.drive(-vel);
	delay(500);
}

void Robot::gira180() {
	giraDcha90();
	giraDcha90();
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

