int lightSensor(int sensor_pin, int led_out){
    int sensorValue = analogRead(sensor_pin);

    if (sensorValue <= 200) {
        digitalWrite(led_out, HIGH);
    } else {
        digitalWrite(led_out, LOW);
    }
}