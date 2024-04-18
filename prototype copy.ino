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

Servo cabeceoServo;
Servo alabeoServo;
Servo guinadaServo;

/*Fotoresistencias y sensor de iluminacion*/
int ft = A1;
int valorft = 0;
int led = 4;

/*Sensor de proximidad*/
int trig1 = 2;
int echo1 = 3;
int trig2 = 8;
int echo2 = 10;
int tiempo;
int distancia;

/*Controladores UI*/
int horarioAlabeo = 6; // Pin del primer pulsador (sentido horario)
int antihorarioAlabeo = 7; // Pin del segundo pulsador (sentido antihorario)
int horarioGuinada = 11;
int antihorarioGuinada = 13;

int alabeoServoPin = 5;  // Pin de control del servo "alabeoServo"
int guinadaServoPin = 12; // Pin de control del servo "guinadaServo"
int cabeceoServoPin = 9; // Pin de control del servo "cabeceoServo"
int angulo = 90;   // Ángulo actual del servo "alabeoServo"
int paso = 10;    // Paso de movimiento en grados
bool sentidoHorario = true; // Variable para rastrear la dirección del movimiento del servo

/*ARDUINO FUNCTIONS: Initialize variables and pinmodes*/
void setup() {
  Serial.begin(9600); //Iniciar Serial
  /*Sensor de iluminacion*/
  pinMode(ft, INPUT); //Fotorresistencia. Modo lectura
  pinMode(led, OUTPUT); //Alimentacion de LED alarma. Modo salida
  
  /*Sensor de proximidad*/
  pinMode(trig1, OUTPUT);/*Pulso para el trigger de sensor 1*/
  pinMode(echo1, INPUT); /*Lectura de datos. Echo de sensor 1*/
  pinMode(trig2, OUTPUT); /*Trigger sensor 2*/
  pinMode(echo2, INPUT); /*Lectura de echo sensor 2*/

  /*Configuraciones de servomotor*/
  alabeoServo.attach(alabeoServoPin);
  cabeceoServo.attach(cabeceoServoPin);
  guinadaServo.attach(guinadaServoPin);

  /*Configuraciones de control UI (user interface)*/
  pinMode(horarioAlabeo, INPUT_PULLUP); // Configura el primer pin del pulsador con resistencia pull-up interna
  pinMode(antihorarioAlabeo, INPUT_PULLUP); // Configura el segundo pin del pulsador con resistencia pull-up interna
  pinMode(horarioGuinada, INPUT_PULLUP);; 
  pinMode(antihorarioGuinada, INPUT_PULLUP);

  alabeoServo.write(angulo); // Establece el ángulo inicial del servo "alabeoServo" a 90 grados
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
*   @local @var distancia stores the value of distance calculated by D = T/V
*   @return distancia value of calculated distance. Float type. 
*/
float proximitySensor(int trigger, int echo){
    digitalWrite(trigger, HIGH);
    delay(1);
    digitalWrite(trigger, LOW);
    int tiempo = pulseIn(echo, HIGH);
    float distancia = tiempo / 58.2;
	  //Serial.println(distancia);
  
    return distancia;
}

/*Accion definida. Activa el estado de evasion*/
/*DEVNOTES: Funcion reutilizable*/
/*  @param distancia introudce la distancia calculada al algoritmo de comparacion*/
bool evadeFunction(int distancia){
  bool evade = false; //initialize in "No alert"
  if (distancia <= 50) {
    evade = true;
  }

  return evade;
}

/*Control de motor universal. Devuelve el angulo incrementado en tiempor real
*   @param evade interruptor logico que lee si el estado de evasion esta activo
*   @param clockwise interruptor logico de sentido horario si esta acivo
*   @param initAngle es el angulo de posicion actual
*/
int engineControl(bool evade, bool clockwise, int initAngle){
  int angle = initAngle;
    do{
        if(clockwise == true){
            angle=+10;
        } else {
            angle=-10;
        }

        return angle;
    } while (evade == false);

    if(evade == true){
        angle=45;
        return angle;
    }
}

/*Funcion que lee los estdos de dos botones de control
*   @param buttonPin1 numero de pin del boton de control de activacion horario
*   @param buttonPin2 numero de pin del boton de control de activacion antihorario
*/
bool clockwiseCheck(int buttonPin1, int buttonPin2){
  bool clockwise = true;
  int horario = digitalRead(buttonPin1);
  int antihorario = digitalRead(buttonPin2);

  if(antihorario == true){
    clockwise = false;
  }

  return clockwise
}

void loop(){
  /*Local variables declaration*/
  bool evadir;
  bool sentidoHorario;

  /*Check illumination*/
  lightSensor(ft, led);

  /*Movimiento de cabeceo*/
  /*1) Revision de proximidad y estado de evasion de emergencia*/
  evadir = evadeFunction(proximitySensor(trig1, echo1));
  
  if(evadir == true){
    cabeceoServo.write(45);
  }
  
  /*Movimiento de alabeo*/
  /*Revision de colision*/
  evadir = evadeFunction(proximitySensor(trig2, echo2));

  /*Lectura UI: Control por botones*/
  sentidoHorario = clockwiseCheck(horarioAlabeo, antihorarioAlabeo);

  /*Accionamiento maestro*/
  alabeoServo.write(engineControl(evadir, sentidoHorario, alabeoServo.read()));
}