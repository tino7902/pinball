#include <AccelStepper.h>

// Definir tipo de motor (4 cables = FULL4WIRE)
#define MOTOR_INTERFACE_TYPE 4

// set up de los sensores hall
const int pinHallUnidad = A5;
int valorHallUnidad;

// Nota el orden de pines: 1, 3, 2, 4
AccelStepper stepper_unidad(MOTOR_INTERFACE_TYPE, 30, 32, 31, 33);  // poner reversa
AccelStepper stepper_decena(MOTOR_INTERFACE_TYPE, 40, 42, 41, 43);
AccelStepper stepper_centena(MOTOR_INTERFACE_TYPE, 50, 52, 51, 53);  // poner reversa

const float PASOS_POR_VUELTA = 2048.0;

const float DIENTES_MOTOR = 32.0;
const float DIENTES_PALETAS = 16.0;
const int CANTIDAD_PALETAS = 16;

const float TRANSMICION = DIENTES_PALETAS / DIENTES_MOTOR;
int pasos_por_paleta = (PASOS_POR_VUELTA * TRANSMICION) / CANTIDAD_PALETAS;

int unidad_actual = 0;
int decena_actual = 0;
int centena_actual = 0;


void setup() {
  Serial.begin(9600);
  // configuracion motores
  stepper_unidad.setMaxSpeed(1000);
  stepper_unidad.setAcceleration(500);
  stepper_unidad.setSpeed(-1000);
  stepper_decena.setMaxSpeed(1000);
  stepper_decena.setAcceleration(500);
  stepper_decena.setSpeed(200);
  stepper_centena.setMaxSpeed(1000);
  stepper_centena.setAcceleration(500);
  stepper_centena.setSpeed(-200);

  randomSeed(analogRead(A0));
}

void loop() {
  //moverTresCifras(random(100, 999));
  //delay(1000);
  /*stepper_unidad.runSpeed();
  stepper_decena.runSpeed();
  stepper_centena.runSpeed();
  */

  /* for(int i = 0; i <=15; i++){
    moverAPaleta(i, stepper_unidad, true, unidad_actual);
    valorHallUnidad = analogRead(pinHallUnidad);
    Serial.print("valor hall: ");
    Serial.println(valorHallUnidad);
    delay(1000);
  } */
  moverACero(stepper_unidad, unidad_actual, valorHallUnidad);
}
boolean revisarReed(int reed, int &reedEstado) {
  reedEstado = digitalRead(reed);
  if (reedEstado == HIGH) return true;
  else return false;
}

void moverACero(AccelStepper stepper, int &actual, int &valorHall) {
  while (true) {
    valorHall = analogRead(pinHallUnidad);
    Serial.print("valor hall: ");
    Serial.println(valorHall);
    if (valorHall > 10) {
      stepper_unidad.runSpeed();
    } else {
      stepper.stop();
      actual = 0;
      break;
    }
  }
}
void moverTresCifras(int destino) {
  Serial.println("");
  Serial.print("/***** el destino es: ");
  Serial.print(destino);
  Serial.println(" *****\ ");

  int unidad = destino % 10;
  destino = destino / 10;
  int decena = destino % 10;
  int centena = destino / 10;
  /*
  Serial.print("la unidad es: ");
  Serial.println(unidad);
  Serial.print("la decena es: ");
  Serial.println(decena);
  Serial.print("la centena es: ");
  Serial.println(centena);
  */

  Serial.println(" ----- inicio mov unidad ----- ");
  moverAPaleta(unidad, stepper_unidad, true, unidad_actual);
  Serial.println(" ----- fin mov unidad ----- ");
  Serial.println(" ----- inicio mov decena ----- ");
  moverAPaleta(decena, stepper_decena, false, decena_actual);
  Serial.println(" ----- fin mov decena ----- ");
  Serial.println(" ----- inicio mov centena ----- ");
  moverAPaleta(centena, stepper_centena, true, centena_actual);
  Serial.println(" ----- fin mov centena ----- ");
}

void moverAPaleta(int destino, AccelStepper stepper, bool reversa, int &actual) {

  if (destino < 0 || destino > 15) {
    Serial.println("nro fuera de rango :");
    return;
  } else {
    //Serial.print("inicia en posicion: ");
    //Serial.println(actual);

    // 1. Calcular distancia en "espacios de paleta"
    int saltos = (destino - actual + CANTIDAD_PALETAS) % CANTIDAD_PALETAS;

    // 2. Convertir saltos a pasos de motor
    long pasos_a_mover = saltos * pasos_por_paleta;

    // 3. Mover
    if (pasos_a_mover > 0) {
      if (reversa) {
        stepper.move(-pasos_a_mover);
        stepper.runToPosition();
      } else {
        stepper.move(pasos_a_mover);
        stepper.runToPosition();
      }
    }

    // 4. Actualizar posición actual
    actual = destino;

    Serial.print("posicion: ");
    Serial.println(destino);
  }
}
