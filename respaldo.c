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
* Añadidos sensor secundario y tercer servomotor
* Añadida documentacion
* Por terminar: Programacion del tercer servo con sensor
* Corregir funcion de sensor de luz
* Comprobar logica 
*/

#include <Servo.h>

Servo cabeceo;
Servo otroServo;
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
int servoPin = 9;  // Pin de control del servo "otroServo"
int angulo = 90;   // Ángulo actual del servo "otroServo"
int paso = 10;    // Paso de movimiento en grados
bool sentidoHorario = true; // Variable para rastrear la dirección del movimiento del servo

/*ARDUINO FUNCTIONS: Initialize variables and pinmodes*/
void setup() {
  Serial.begin(9600); //Iniciar Serial
  /*Sensor de iluminacion*/
  pinMode(ft, INPUT); //Fotorresistencia. Modo lectura
  pinMode(led, OUTPUT); //Alimentacion de LED alarma. Modo salida
  
  /*Sensor de proximidad*/
  pinMode(trig, OUTPUT);/*Pulso para el trigger*/
  pinMode(echo, INPUT); /*Lectura de datos. Pin echo*/

  /*Configuraciones de servomotor*/
  cabeceo.attach(5);
  otroServo.attach(servoPin); // Adjunta "otroServo" al pin correspondiente

  /*Configuraciones de control UI (user interface)*/
  pinMode(botonPin1, INPUT_PULLUP); // Configura el primer pin del pulsador con resistencia pull-up interna
  pinMode(botonPin2, INPUT_PULLUP); // Configura el segundo pin del pulsador con resistencia pull-up interna

  otroServo.write(angulo); // Establece el ángulo inicial del servo "otroServo" a 90 grados
}

/*TEST MODE: Function that only reads an analog lecture from an analog pin connected to sensor
*   @param pin reads an analog pin
*   @return sensorValue returns a value in a range between 0-1023
*/
int analog_read(int pin){ 
   int sensorValue = analogRead(pin);
   return sensorValue;
}

/*TEST MODE: Function that read and print angular position of all servos
*   @param none
*   @returns none
*/
void servoPosition(){
  Serial.println(cabeceo.read());
  Serial.println(otroServo.read());
  Serial.println(guinada.read());
}

/*Function that read sensor value and turns on a LED. Reusable
*   @Param sensor_pin: input parameter of an analog sensor pin. Reads Pin number
*   @Param led_out: pin output. Dynamic supply for LED. Pin number*/
int lightSensor(int sensor_pin, int led_out){
    int sensorValue = analogRead(sensor_pin);

    if (sensorValue <= 200) {
        digitalWrite(led_out, HIGH);
    } else {
        digitalWrite(led_out, LOW);
    }
}

/*Function that read proximity sensor value and calculate distance of proximity
*DEVNOTES: Esta funcion es reutilizable. Devuelve la distancia para cada sensor configurado
*   @param trigger reads trigger value from sensor. Defined as pin number
*   @param echo reads echo value from sensor
*   @local @var tiempo stores the value of time of a sonic echo
    @local @var distancia stores the value of distance calculated by D = T/V
*   @return distancia value of calculated distance. Float type. 
*/
float proximitySensor(int trigger, int echo){
    digitalWrite(trigger, HIGH);
    delay(1);
    digitalWrite(trigger, LOW);
    int tiempo = pulseIn(echo, HIGH);
    float distancia = tiempo / 58.2;
	  Serial.println(distancia);
  
    return distancia;
}

/*Function that initialize an action if true conditions: detected an object*/
/*DEVNOTES: Funcion no reutilizable*/
void cabeceoAction(int distancia){
  if (distancia <= 50) {
    cabeceo.write(45);
    //Serial.println("SERVO 45");
  }
  if (distancia > 50) {
    cabeceo.write(0);
    //cabeceo.println("SERVO 0");
  }
}

void loop(){
  int estadoBoton1 = digitalRead(botonPin1);
  int estadoBoton2 = digitalRead(botonPin2);
      
    lightSensor(ft, led);
    cabeceoAction(proximitySensor(trig, echo));

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