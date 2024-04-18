float proximitySensor(int trigger, int echo){
    digitalWrite(trigger, HIGH);
    delay(1);
    digitalWrite(trigger, LOW);
    int tiempo = pulseIn(echo, HIGH);
    float distancia = tiempo / 58.2;
	  Serial.println(distancia);
  
    return distancia;
}

void servo_proximy(int distancia){
  if (distancia <= 50) {
    //ServoID.write(45);
    Serial.println("SERVO 45");
  }
  if (distancia > 50) {
    //ServoID.write(0);
    Serial.println("SERVO 0");
  }
}

