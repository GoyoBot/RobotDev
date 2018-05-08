#ifndef ROBOT_H
#define ROBOT_H

#include <MPU6050_tockn.h>

#include "Motor.h"
#include "UltraSound.h"

#define DEBUG 1

#ifdef DEBUG
 #define DEBUG_PRINT(x)     Serial.print(x)
 #define DEBUG_PRINTLN(x)   Serial.println(x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif

// Pines de tipo izquierda-derecha
typedef struct {
	int I, D;
} IzdaDcha;

// Pines ultrasonido
typedef struct {
	int echo, trigger;
} US_Pins;

/// Configuración de pines del robot
typedef struct RobotPins {
	struct {
		IzdaDcha IN1 = {2, 7};
		IzdaDcha IN2 = {4, 8};
		IzdaDcha PWM = {5, 6};
		int STBY = 9;
		IzdaDcha offset = {1, 1}; //< 1 o -1 para cambiar el giro
	} motor;
	
	/// Sensores Ultra Sonido. Con la biblioteca NewPing, trigger y echo pueden estar en el mismo pin
	struct {
		US_Pins I = {10, 10};
		US_Pins C = {12, 12};
		US_Pins D = {11, 11};
	} US;
	
	// Sensores Infrarrojo. Pines analógicos Ax.
	int IR[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
	
	/// Umbral IR para diferenciar blanco/negro
	int IR_threshold = 350;
} RobotPins;

const RobotPins DefaultPins;


class Robot {
public:
	Robot(const RobotPins p = DefaultPins);
	
	/// Configura el robot. Llamar en el setup() principal
	void setup();
	
	/// Actualiza el hardware interno. Llamar en cada ciclo loop()
	void update();
	
	void avanza(unsigned long time = 0);        ///< El robot avanza. El robot no se para al finalizar el método.
	void retrocede(unsigned long time = 0);     ///< El robot retrocede. El robot no se para al finalizar el método.
	void para(unsigned long time = 0);          ///< El robot se detiene y espera `time` milisegundos.
	
	/// Avanza y gira a la izda. Intensidad del `giro` entre [0, 512]
	void avanzaIzda(unsigned int giro, unsigned long time = 0);
	
	/// Avanza y gira a la dcha. Intensidad del `giro` entre [0, 512]
	void avanzaDcha(unsigned int giro, unsigned long time = 0);
	
	/// Avanza y gira a la izda (-250) o dcha (250). Intensidad del `giro` entre [-512, 512]
	void avanzaGiro(int giro, unsigned long time = 0);
	
	// Estos métodos bloquean la ejecución hasta que se haya completado el giro
	void giraIzda90();  ///< Giro completo de 90° a la izda
	void giraDcha90();  ///< Giro completo de 90° a la dcha
	void gira180();     ///< Giro de 180°
	
	/// Nueva velocidad del robot entre [0, 512]
	void setVelocidad(unsigned int velocidad);
	
	// Sensores Infrarrojo
	bool irSobreNegro(int ir);   ///< `ir` debe ser uno de pins.IR0, pins.IR1, ...
	bool irSobreBlanco(int ir);
	int irData(int ir);
	
	/**
	 * Obtener el ángulo del robot respecto a la calibración inicial en el setup() del robot.
	 * Sólo se actualiza tras hacer update().
	 * @return Número positivo: giro derecha. Negativo: izquierda.
	 */
	float angulo();
	float anguloAbsoluto();
	
	// Motores
	Motor motorI;
	Motor motorD;
	
	// Sensores de ultrasonidos
	UltraSound ultraSonidoI;
	UltraSound ultraSonidoC;
	UltraSound ultraSonidoD;
	
	// Sensor MPU6050: acelerómetro/giroscopio
	MPU6050 mpu6050;
	
	const RobotPins pins;  ///< Acceso a los pines
	
	// Para depurar
	void printUltraSonido();
	void printInfraRed();
	void printInfraRedRawData();

private:
	unsigned int _velocidad = 100; // Velocidad actual, inicializada
};


#endif // ROBOT_H
