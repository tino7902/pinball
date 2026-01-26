#include <AccelStepper.h>

// Definir tipo de motor (4 cables = FULL4WIRE)
#define MOTOR_INTERFACE_TYPE 4

// Motor 1 (Pines 34, 32, 33, 31) <- Nota el orden: 1, 3, 2, 4 para esta librería suele ser necesario cruzar los cables centrales
AccelStepper stepper1(MOTOR_INTERFACE_TYPE, 30, 32, 31, 33);

// Motor 2 (Pines de ejemplo)
//AccelStepper stepper2(MOTOR_INTERFACE_TYPE, 10, 12, 11, 13);

void setup() {
  Serial.begin(9600);
  // Configurar Motor 1
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);
  stepper1.setSpeed(600); // Velocidad constante

  // Configurar Motor 2
  /*stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);
  stepper2.setSpeed(-200); // Velocidad negativa = otra dirección*/
}

void loop() {
  Serial.println("inicio mov");
  stepper1.move(100);
  Serial.println("fin mov");
  stepper1.runToPosition();
  delay(1000);
  //stepper2.runSpeed();
}