#ifndef UltraSound_H
#define UltraSound_H

// Instalar esta biblioteca desde Arduino GUI
#include <NewPing.h>  // https://bitbucket.org/teckel12/arduino-new-ping

/**
 * Los sensores de ultrasonido son problemáticos, y necesitan limpiar las medidas.
 */
class UltraSound
{
public:
	UltraSound(int echo, int trigger);  /// Con NewPing, echo y trigger pueden ser el mismo pin
	
	/// Actualiza el sensor. Llamar en cada ciclo loop()
	void update();
	
	/// Distancia en cm
	long distancia();
	
private:
	NewPing _sensor;
	long _distancia;                   // Distancia actual filtrada en `update()`
	const int LimiteMedidasZero = 10;  // Número de medidas de filtrado
	int _medidasZero;                  // Número de medidas en cero
};

#endif