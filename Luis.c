#include <stdio.h>
#include <stdlib.h>

/*Prototipos*/
int collectData(int arraySize, bool randomize);
void bubbleSort(int array[], int size);
void insertionSort(int array[], int size)
void printData(array[], size);

/*Main ----------------------*/
int main(){
    bool setRandomFunction = false; 
    int size; 
    int mode;

    printf("Inserte la cantidad de datos de su array: \n");
    scanf("%i", &size);
    printf("Desea que rellenemos los datos por usted?? \n1) Si\n2)No\n");
    scanf("%i", &mode);

    int datos[size];

    if(mode == 1){
        setRandomFunction = true;
    }

    datos[size] = collectData(size, setRandomFunction);
    bubbleSort(datos[size], size);

    if(setRandomFunction == true){
        for (int i = 0; i < size; i++) {
        printf("%d ", datos[i]);
        }
        printf("\n");
    }

    if(setRandomFunction == false){
        printf("Desea ordenamiento por insercion? \n1) Si\n2) No\n");
        scanf("&i", mode);
        if(mode == 1){
            insertionSort(datos[size], size);
        } else {
            collectData[size];
        }
    }
    

    printData(datos[size])
    return 0; 
}

/*Funciones auxiliares*/
int collectData(int arraySize, bool randomize){
    int iterator = 0;
    int dataContainer[arraySize];
    int num;

    SYSTEM("CLS");

    while(iterator < sizeof(dataContainer)){
       if(randomize == false){
            printf("Inserte valor de arreglo %i:", iterator);
            scanf("%i", &num)
            dataContainer[iterator] = num; 
       } else {
            dataContainer[iterator] = random();
       }
        iterator++;
    }

    return dataContainer[arraySize];
}

void bubbleSort(int array[], int size) {
    int temp;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Intercambiar elementos si están en el orden incorrecto
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void insertionSort(int array[], int size) {
    int i, key, j;
    for (i = 1; i < size; i++) {
        key = array[i];
        j = i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

void printData(array[], size){
    for(int i = 0; i<size; i++){
        printf("%i) %i\n", i, array[i]);
    }
    printf("\nFin");
}