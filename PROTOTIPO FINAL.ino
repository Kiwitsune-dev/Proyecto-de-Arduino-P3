/*NOTAS GENERALES
CODIGO ELABORADO POR: HERMAN EMMANUEL R. CRUZ, FERNANDO JIMENEZ ZAMORA
UNIVERSIDAD AERONAUTICA EN QUERETARO
ING. EN DISEÑO MECANICO AERONAUTICO
CONTROL DE MOTORES ELECTRICOS
GRUPO 11A 2024-2
*/

/*NOTAS DE DOCUMENTACION
*   Documentacion basada en el formato de definicion y descpricpcion JAVADOC. 
*   Describa las variables con "@local @var" o bien "@global @var"
*   Describa los parametros de funcion con "@param"
*   Describa la salida de una funcion con "@return"
*   No se requiere la implementacion de clases
*   La descripcion de autores y versiones no está definida por JAVADOC. Consulte notas generales
*/

/* NOTAS DE LA VERSION
* LA PRESENTE CONTIENE LAS MODIFICACIONES DE LA EXCLUSION DEL MOVIMIENTO DE CABECEO
* VERSION 5.0 FINALIZADA PARA SU REVISION
* PUBLICADA EN GITHUB EL DIA 19 DE ABRIL DE 2024 PARA ACCESO PUBLICO
*/

#include <Servo.h>

/*Declaracion de motores y asignacion de tags*/
//Servo cabeceo;
Servo alabeo;
Servo guinada;

/*Variables definidas para funcion de iluminacion*/
int ft = 5;
int valorft = 0;
int led = 4;

/*Variables definidas para sensor de proximidad*/
int trig = 2;
int echo = 3;
int tiempo;
int distancia;

/*Variables definidas para funcion de pulsadores*/
int botonPin1 = 6; // Pin del primer pulsador (sentido horario)
int botonPin2 = 7; // Pin del segundo pulsador (sentido antihorario)

/*Variables definidas para las funciones de servomotor*/
int angulo = 90;   // Ángulo actual del servo "alabeo"
int anguloGuinada = 90;
int paso = 10;    // Paso de movimiento en grados
int pasoGuinada = 10;
bool sentidoHorario = true; // Variable para rastrear la dirección del movimiento del servo

/*Pines definidos para las luces de navegacion*/
int greenLight = 8;
int redLight = 13;

/*Pines del sensor de inclinacion*/
int derecha = 11;
int izquierda = 12;

/*Funcion de configuracion principal*/
void setup() {
  /*Inicializacion del puerto Serial*/
  Serial.begin(9600);

  /*Pines de proximidad*/
  pinMode(ft, INPUT);
  pinMode(led, OUTPUT);
  
  /*Configuracion de pines del sensor de proximidad*/
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  /*Configuracion de los pines de servos*/
  //cabeceo.attach(5); // Adjunta "cabeceo" al pin 5
  alabeo.attach(9); // Adjunta "alabeo" al pin 9
  guinada.attach(10); // Adjunta "guinada" al pin 10

  /*Configuracion de */
  pinMode(botonPin1, INPUT_PULLUP); // Configura el primer pin del pulsador con resistencia pull-up interna
  pinMode(botonPin2, INPUT_PULLUP); // Configura el segundo pin del pulsador con resistencia pull-up interna

  /*Configuracion de pines de la funcion de alarma sonora*/
  pinMode(greenLight, OUTPUT);
  pinMode(redLight, OUTPUT);

  /*Inicializando pines de sensor de posicion*/
  pinMode(derecha, INPUT);
  pinMode(izquierda, INPUT);

  //alabeo.write(angulo); // Establece el ángulo inicial del servo "alabeo" a 90 grados
}

/* Funcion que alterna pulsos de iluminacion en dos leds que simula las navlights
* @param RIGHT corresponde al pin de LED rojo
* @param LEFT corresponde al pin de LED verde
* @param time corresponde al tiempo de intermitencia
*/
void navLights(int RIGHT, int LEFT, int timer){
  digitalWrite(RIGHT, HIGH);
  delay(timer);
  digitalWrite(RIGHT, LOW);
  delay(timer);
  digitalWrite(LEFT, HIGH);
  delay(timer);
  digitalWrite(LEFT, LOW);
  delay(timer);
}

void loop() {
  /*Funcion de las luces de navegacion*/
  navLights(greenLight, redLight, 200);

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

  /*Accion de respuesta: evasion en alabeo y guinada*/
  //Genera una desviacion angular en ambos movimientos de motor en caso de colision. Adicionalmente genera un mensaje de consola
  if (distancia <= 10 && distancia != 0) {
    alabeo.write(110);
    guinada.write(110);
    Serial.println("Alarma: COLISION!!");
  } //Si no hay colision, regresa a su estado de maniobra normal
  if (distancia > 10) {
    alabeo.write(angulo);
    guinada.write(anguloGuinada);
  }

  /*Accion de respuesta: Iluminacion*/
  //Genera una respuesta de accionamiento de LEDS en caso de no detectar luz suficiente. Simula condiciones nocturnas
  if (valorft <= 150) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

  /*Accion de respuesta: Detector de inclinacion*/
  int estadoDerecha = digitalRead(derecha);
  int estadoIzquierda = digitalRead(izquierda);

  //La condicion detecta si hay inclinacion. Excluye el mensaje de consola en caso de que el detector IZQUIERDA este activo
  if(estadoDerecha == HIGH && estadoIzquierda == LOW){
    Serial.println("Estado alabeo: Inclinado a la derecha");
  }

  //Devuelve el estado de inclinacion si unicamente hay un sensor encendido y genera un mensaje de consola
  if(estadoIzquierda == HIGH && estadoDerecha == LOW){
    Serial.println("Estado alabeo: Inclinado a la izquierda");
  }

  // Lee el estado de los pulsadores
  int estadoBoton1 = digitalRead(botonPin1);
  int estadoBoton2 = digitalRead(botonPin2);

  // Si el primer pulsador está presionado y el servo no ha alcanzado su límite superior, gira 10 grados en sentido horario
  if (estadoBoton1 == LOW && angulo < 180) {
    angulo += paso; // Aumenta el ángulo en el paso definido
    alabeo.write(angulo); // Mueve "alabeo" al ángulo calculado

    //Respuesta de Guinada
    anguloGuinada -= pasoGuinada;
    guinada.write(anguloGuinada);

    sentidoHorario = true; // Establece el sentido del movimiento como horario
    delay(200); // Retardo para evitar rebotes del pulsador
  }

  // Si el segundo pulsador está presionado y el servo no ha alcanzado su límite inferior, gira 10 grados en sentido antihorario
  if (estadoBoton2 == LOW && angulo > 0) {
    angulo -= paso; // Disminuye el ángulo en el paso definido
    alabeo.write(angulo); // Mueve "alabeo" al ángulo calculado

    //Respuesta de Guinada
    anguloGuinada += pasoGuinada;
    guinada.write(anguloGuinada);

    sentidoHorario = false; // Establece el sentido del movimiento como antihorario
    delay(200); // Retardo para evitar rebotes del pulsador
  }
}