#include <Servo.h>

Servo cabeceo;
Servo alabeo;
Servo guinada;

int ft = 1;
int valorft = 0;
int led = 4;
int trig = 2;
int echo = 3;
int tiempo;
int distancia;
int botonPin1 = 6; // Pin del primer pulsador (sentido horario)
int botonPin2 = 7; // Pin del segundo pulsador (sentido antihorario)
int angulo = 90;   // Ángulo actual del servo "alabeo"
int anguloGuinada = 90;
int paso = 10;    // Paso de movimiento en grados
int pasoGuinada = 10;
bool sentidoHorario = true; // Variable para rastrear la dirección del movimiento del servo

int supply1 = 0;
int supply2 = 1;

void setup() {
  /*Inicializacion del puerto Serial*/
  Serial.begin(9600);

  /*P*/
  pinMode(ft, INPUT);
  pinMode(led, OUTPUT);
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  cabeceo.attach(5); // Adjunta "cabeceo" al pin 5
  alabeo.attach(9); // Adjunta "alabeo" al pin 9
  guinada.attach(10); // Adjunta "guinada" al pin 10
  pinMode(botonPin1, INPUT_PULLUP); // Configura el primer pin del pulsador con resistencia pull-up interna
  pinMode(botonPin2, INPUT_PULLUP); // Configura el segundo pin del pulsador con resistencia pull-up interna

  /*Configuracion de pines de la funcion de alarma sonora*/
  pinMode(supply1, OUTPUT);
  pinMode(supply2, OUTPUT);

  alabeo.write(angulo); // Establece el ángulo inicial del servo "alabeo" a 90 grados
}

/*void funcionIntermitencia(int pin, int time){
  pinMode(pin, OUTPUT);
  digital.write(pin, HIGH);
  delay(timer);
  digital.write(pin, LOW);
  delay(timer);
}

void navLights(int RIGHT, int LEFT, int time){
  digital.write(RIGHT, HIGH);
  delay(timer);
  digital.write(RIGHT, LOW);
  delay(timer);
  digital.write(LEFT, HIGH);s
  delay(timer);
  digital.write(LEFT, LOW);
  delay(timer);
}*/

bool sensorIR(int pin){
  bool contacto = false;
  if(pin == HIGH){
    contacto = true;
  }
  return contacto;
}

void loop() {
  /*Funcion de las luces de navegacion*/
  //navLights(supply1, supply2, 500);

  /*Funcion de iluminacion*/
  valorft = analogRead(ft);
  Serial.print("Cantidad de luz: ");
  Serial.println(valorft);

  /*Funcion de proximidad*/
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  tiempo = pulseIn(echo, HIGH);
  distancia = tiempo / 58.2;
  Serial.print("Distancia colision: ");
  Serial.println(distancia);
  Serial.println();
  delay(500);

  /*Accion de respuesta: evasion en cabeceo
  if (distancia <= 50) {
    funcionIntermitencia(13, 500); //Alarma sonora
    cabeceo.write(45);
  }
  if (distancia > 50) {
    cabeceo.write(0);
  }*/

  if(sensorIR(11) == true){
    //funcionIntermitencia(13, 500); //Alarma sonora
    cabeceo.write(45);
  }

  if (sensorIR(11) == false) {
    cabeceo.write(0);
  }

  /*Accion de respuesta: Encendido de luz de pista*/
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
    alabeo.write(angulo); // Mueve "alabeo" al ángulo calculado

    anguloGuinada += pasoGuinada;
    guinada.write(anguloGuinada);

    sentidoHorario = true; // Establece el sentido del movimiento como horario
    delay(200); // Retardo para evitar rebotes del pulsador
  }

  // Si el segundo pulsador está presionado y el servo no ha alcanzado su límite inferior, gira 10 grados en sentido antihorario
  if (estadoBoton2 == LOW && angulo > 0) {
    angulo -= paso; // Disminuye el ángulo en el paso definido
    alabeo.write(angulo); // Mueve "alabeo" al ángulo calculado

    anguloGuinada -= pasoGuinada;
    guinada.write(anguloGuinada);

    sentidoHorario = false; // Establece el sentido del movimiento como antihorario
    delay(200); // Retardo para evitar rebotes del pulsador
  }
}