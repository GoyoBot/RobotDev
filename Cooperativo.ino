#include "Robot.h"

Robot robot;

float angulo = 0.0;
float offset = 7.0;

void setup()
{
    robot.setVelocidad(70);
    // Serial.begin(115200);
    Serial.begin(9600);
    robot.setup();
}

void loop()
{
    robot.update(); // Siempre al comenzar el loop
    
    comprobarObstaculos();
    comprobarDireccion();
    //comprobarLineas();

    robot.avanza();
}

// -------------------- Dirección --------------------
void comprobarDireccion()
{
    if (abs(anguloRectificacion(angulo)) > offset)
    { // Si no esta colocado, para y recoloca
        robot.para();
        rectificarAngulo(angulo);
    }
}

// Comprueba que el robot esta mirando en la dirección correcta con un offset
float anguloRectificacion(float original)
{
    float actual = robot.anguloAbsoluto();
    if (abs(original - actual) > 180)
    {
        original += 360;
    }
    return original - actual;
}

// Para el robot y lo rota hasta que la dirección es la correcta
void rectificarAngulo(float original)
{
    float ang = anguloRectificacion(original);
    while (abs(ang) > offset)
    {
        //Serial.print("ar: " + String(ang));
        if (ang > 0)
        {
            //robot.avanzaDcha(512, 1);
            robot.motorI.drive(70);
            robot.motorD.drive(0);
            //Serial.println(" DERECHA");
        }
        else
        {
            //robot.avanzaIzda(512, 1);
            robot.motorI.drive(0);
            robot.motorD.drive(70);
            //Serial.println(" IZQUIERDA");
        }
        delay(10);
        robot.update();
        ang = anguloRectificacion(original);
    }
}

// -------------------- FinDirección --------------------







// -------------------- Obstaculos --------------------

int dist = 10;

void  comprobarObstaculos(){
    Serial.println(robot.ultraSonidoC.distancia());
    if(robot.ultraSonidoC.distancia() < dist){
        Serial.println("Detectado");
        evitarObstaculos(!(robot.ultraSonidoD.distancia() < dist));
    }
}

void evitarObstaculos(bool direccionGiro) { // true -> derecha --- false -> izquierda

  robot.para();
    if (direccionGiro) {
        robot.giraDcha90();
        robot.giraDcha90();///////////////////
    } else {
        robot.giraIzda90();    
        robot.giraIzda90();    
    }
    delay(1000);
    robot.para();
    delay(1000);

    bool direccionAComprobar = !direccionGiro; // true -> derecha --- fasle -> izquierda

    bool objetoDetectado = comprobarObjetoID(direccionAComprobar);

    while (objetoDetectado) {
        robot.avanza();
        robot.update();

        // comprobarObstaculos();
        // Comprobar lineas frontales

        objetoDetectado = comprobarObjetoID(direccionAComprobar);
    }

    delay(50);

    if (!direccionGiro) {
        robot.giraDcha90();
    } else {
        robot.giraIzda90();        
    }

}

bool comprobarObjetoID(bool direccion) {
    if (direccion) {
        return robot.ultraSonidoD.distancia() < 15;////////// Deberia ser mayor por si se aleja del objeto
    } else {
        return robot.ultraSonidoI.distancia() < 15;////////////
    }
}


// -------------------- FinObstaculos --------------------








// -------------------- Lineas --------------------

// Reinicia la comprobación del final del recorrido

// array que almacena la detección de la linea de los sensores
bool sensores[6] = {false, false, false, false, false, false};

// comprueba que se ha detectado una linea a través de todos los sensores por orden
// en menos de 2 segundos
bool comprobarLineas()
{
    //----------------------LineasFrontales---------------
   
    

    if(!(sensores[0])){
        sensores[0] = robot.irSobreNegro(robot.pins.IR[0]);
    }
    if(!(sensores[5])){
        sensores[5] = robot.irSobreNegro(robot.pins.IR[7]);
    }
    if(!(sensores[2])){
        sensores[2] = robot.irSobreNegro(robot.pins.IR[2]);
    }
    if(!(sensores[3])){
        sensores[3] = robot.irSobreNegro(robot.pins.IR[5]);
    }

    if(sensores[0] && sensores[5]){ //Linea frontal
        robot.para();
            if (abs(anguloRectificacion(angulo)) <= offset)
            { // Esta comprobación es necesaria puesto que solo en caso de que mire en la dirección correcta deberá ignorar la detección de lineas frontalmente
                Serial.println("Final");
                finalizarRecorido();
                
            }
            else
            {
                reedireccionPorLineaFrontal();
                Serial.println("Lateral");
                return true;
                
            }
    }

    if(sensores[0] && sensores[2] && !sensores[3]){ //Line lateral izquierda

        reedireccionPorLineaLateral(false);

    }else if(sensores[5] && sensores[3] && !sensores[2]){ // Linea Lateral derecha

        reedireccionPorLineaLateral(true);

    }
    return false;



   //----------------------FINLineasFrontales---------------
    
}

void resetFinRecorrido()
{
    for (int i = 0; i < 6; i++)
    {
        sensores[i] = false;
    }
}

// Para el robot hasta reiniciar arduino
void finalizarRecorido()
{
    robot.para();
    while (true)
    {
        Serial.print("Fin!");
        delay(1000);
    }
}

void reedireccionPorLineaLateral(bool esDcha){
  if(esDcha){
    rectificarAngulo(270.0);
  }else{
    rectificarAngulo(90.0);
  }
  robot.avanza(10);
}

void reedireccionPorLineaFrontal()
{
    if(robot.ultraSonidoD.distancia() > 10){ //Calibrar la distancia
        robot.gira180(); //Gira 180º a la derecha
    }else{
        robot.giraIzda90();
        robot.giraIzda90();
        robot.giraIzda90();
        robot.giraIzda90();
    }
}

// -------------------- FinLineas --------------------
