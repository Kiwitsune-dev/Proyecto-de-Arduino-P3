void(bool nearObject){
    while(nearObject != false){
        
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
}