#include <stdio.h>

int engineControl(bool clockwise, int controllerPin, int angularPosition){
    do{
        if(proximityChech > 50){
            angle++;
        } else {
            angle=-45;
        }

        return angle;
    } while (clockwise == true);

    do{
        if(proximityChech > 50){
            angle--;
        } else {
            angle=+45;
        }
        
        return angle;
    } while (clockwise == false);
}

int engineControl(bool evade, bool clockwise){
    do{
        if(clockwise == true){
            angle++;
        } else {
            angle--;
        }

        return angle;
    } while (evade == false);

    if(evade == true){
        angle=+45;
        return angle;
    }
}

void loop(){
    int estadoBoton1 = digitalRead(botonPin1);
    int estadoBoton2 = digitalRead(botonPin2);

    if(estadoBoton1 == HIGH){

    }
}

