#include <Servo.h>

Servo cabeceo;
Servo otroServo;

int ft = 1;
int valorft = 0;
int led = 4;
int trig = 2;
int echo = 3;
int tiempo;
int distancia;
int botonPin1 = 6; // Pin del primer pulsador (sentido horario)
int botonPin2 = 7; // Pin del segundo pulsador (sentido antihorario)
int servoPin = 9;  // Pin de control del servo "otroServo"
int angulo = 90;   // Ángulo actual del servo "otroServo"
int paso = 10;    // Paso de movimiento en grados
bool sentidoHorario = true; // Variable para rastrear la dirección del movimiento del servo

void setup() {
  pinMode(ft, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  cabeceo.attach(5);
  otroServo.attach(servoPin); // Adjunta "otroServo" al pin correspondiente
  pinMode(botonPin1, INPUT_PULLUP); // Configura el primer pin del pulsador con resistencia pull-up interna
  pinMode(botonPin2, INPUT_PULLUP); // Configura el segundo pin del pulsador con resistencia pull-up interna

  otroServo.write(angulo); // Establece el ángulo inicial del servo "otroServo" a 90 grados
}

void loop() {
  valorft = analogRead(ft);
  Serial.print("Cantidad de luz: ");
  Serial.println(valorft);
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  tiempo = pulseIn(echo, HIGH);
  distancia = tiempo / 58.2;
  Serial.print("Distancia colision: ");
  Serial.println(distancia);
  Serial.println();
  delay(500);

  if (distancia <= 50) {
    cabeceo.write(45);
  }
  if (distancia > 50) {
    cabeceo.write(0);
  }

  if (valorft <= 200) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

  // Lee el estado de los pulsadores
  int estadoBoton1 = digitalRead(botonPin1);
  int estadoBoton2 = digitalRead(botonPin2);

  // Si el primer pulsador está presionado y el servo no ha alcanzado su límite superior, gira 10 grados en sentido horario
  if (estadoBoton1 == LOW && angulo < 180) {
    angulo += paso; // Aumenta el ángulo en el paso definido
    otroServo.write(angulo); // Mueve "otroServo" al ángulo calculado
    sentidoHorario = true; // Establece el sentido del movimiento como horario
    delay(200); // Retardo para evitar rebotes del pulsador
  }

  // Si el segundo pulsador está presionado y el servo no ha alcanzado su límite inferior, gira 10 grados en sentido antihorario
  if (estadoBoton2 == LOW && angulo > 0) {
    angulo -= paso; // Disminuye el ángulo en el paso definido
    otroServo.write(angulo); // Mueve "otroServo" al ángulo calculado
    sentidoHorario = false; // Establece el sentido del movimiento como antihorario
    delay(200); // Retardo para evitar rebotes del pulsador
  }
}