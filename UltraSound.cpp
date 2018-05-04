#include "UltraSound.h"

UltraSound::UltraSound(int echo, int trigger): _sensor(trigger, echo, 200), _medidasZero(0), _distancia(0) {
}

/** 
 * En ocasiones, el sensor toma una medida errónea de valor 0.
 * Filtramos esta medida para tener una medida estable:
 *  - Si la medida es 0, no se actualiza _distancia.
 *  - Pero si se toma 0 más de limiteMedidasZero veces seguidas, entonces _distancia sí que pasa a 0.
 */
void UltraSound::update() {
	int medida = _sensor.ping_cm();
	if (medida == 0) {
		++_medidasZero;
		if (_medidasZero > LimiteMedidasZero) {
			_distancia = 0;
			_medidasZero = 0;
		}
	} else {
		_distancia = medida;
		_medidasZero = 0;
	}
}

long UltraSound::distancia() {
	return _distancia;
}
