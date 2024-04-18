#include <Servo.h>

Servo cabeceo;
Servo alabeo;
Servo guinada;

/*Variables definidas para funcion de iluminacion*/
int ft = 1;
int valorft = 0;
int led = 4;

/*Variables definidas para los sensor de inclinacion*/
int inclinacionDerecha = 10;
int inclinacionIzquierda = 11;

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
int greenLight = 0;
int redLight = 1;

/*Alimentacion para alarma sonora*/
int buzzerPin = 13;

/*Pin de lectura del infrarrojo*/
int infrarrojo = 12;

void setup() {
  /*Inicializacion del puerto Serial*/
  Serial.begin(9600);

  /*Configuracion de pines del sistema de iluminacion*/
  pinMode(ft, INPUT);
  pinMode(led, OUTPUT);

  /*Inicializacion de pines del sistema de inclinacion*/
  pinMode(inclinacionDerecha, INPUT);
  pinMOde(inclinacionIzquierda, INPUT);
  
  /*Configuracion de servomotores*/
  cabeceo.attach(5); // Adjunta "cabeceo" al pin 5
  alabeo.attach(9); // Adjunta "alabeo" al pin 9
  guinada.attach(10); // Adjunta "guinada" al pin 10

  /*Configuracion de pines para el sistema de control por botones*/
  pinMode(botonPin1, INPUT_PULLUP); // Configura el primer pin del pulsador con resistencia pull-up interna
  pinMode(botonPin2, INPUT_PULLUP); // Configura el segundo pin del pulsador con resistencia pull-up interna

  /*Configuracion de pines de la funcion de alarma sonora*/
  pinMode(greenLight, OUTPUT);
  pinMode(redLight, OUTPUT);

  /*Inicizalizacion del pin infrarrojo como lectura digital*/
  pinMode(infrarrojo, INPUT);

  /*Inicializacion de pines de luz de navegacion como alimentacion*/
  pinMode(greenLight, OUTPUT);
  pinMode(redLight, OUTPUT);

  /*Inicializacion del pin buzzer como alimentacion*/
  pinMode(buzzerPin, OUTPUT);

  alabeo.write(angulo); // Establece el ángulo inicial del servo "alabeo" a 90 grados
}


/* Funcion que envia pulsos definidos por tiempo para un buzzer. 
* @param pin pin de coneccion al buzzer
* @param time tiempo de encendido y apagado
*/
void alarmaIntermitente(int pin, int time){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(timer);
  digitalWrite(pin, LOW);
  delay(timer);
}

/*Funcion que comprueba si el sensor detecta inclinacion en el umbral a la izquierda
* @param pinIzquierda corresponde al pin de lectura de estados digitales
*/
bool alarmaIzquierda(int pinIzquierda){
  bool inclinacion = false;
  if(pinIzquierda == HIGH){
    inclinacion = true;
  }
  return inclinacion;
}

/*Funcion que comprueba si el sensor detecta inclinacion en el umbral a la derecha
* @param pinDerecha corresponde al pin de lectura de estados digitales
*/
bool alarmaDerecha(int pinDerecha){
  bool inclinacion = false; 
  if(pinDerecha == HIGH){
    inclinacion == true;
  }
  return inclinacion;
}

/* Funcion que alterna pulsos de iluminacion en dos leds que simula las navlights
* @param RIGHT corresponde al pin de LED rojo
* @param LEFT corresponde al pin de LED verde
* @param time corresponde al tiempo de intermitencia
*/
void navLights(int RIGHT, int LEFT, int time){
  digitalWrite(RIGHT, HIGH);
  delay(timer);
  digitalWrite(RIGHT, LOW);
  delay(timer);
  digitalWrite(LEFT, HIGH);s
  delay(timer);
  digitalWrite(LEFT, LOW);
  delay(timer);
}

/*Funcion que lee el estado digital del sensor de contacto infrarrojo
* @param pin es el pin de lectura digital del sensor
*/
bool sensorIR(int pin){
  bool contacto = false;
  if(pin == HIGH){
    contacto = true;
  }
  return contacto;
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
void loop() {
  /*Funcion de las luces de navegacion*/
  //navLights(greenLight, redLight, 500);

  /*Funcion de iluminacion*/
  valorft = analogRead(ft);
  Serial.print("Cantidad de luz: ");
  Serial.println(valorft);

  /*Alarma sonora*/
  int alarmaEstado = sensorIR(buzzerPin);

  if(sensorIR(infrarrrojo) == true){
    alarmaIntermitente(buzzerPin);
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