/*
Bibiloni Fontirroig, Josep Antoni
Chellak El Ouahabi, Mohamed Rida
Socias Forteza, Antoni 
*/
#include <stdio.h>
#include <limits.h>
#include "my_lib.h"

/*
 * Función:  main
 * --------------------
 * 
 *  Se lee fichero almacenado y se imprime los valores
 *  de cada elemento, suma, mínimo, máximo y media
 * 
 *  void *ptr: puntero a la pila
 *
 */
int main(int argc, char *argv[]) {

    struct my_stack *pila;
    int *data;
    
    if (!argv[1]) {  //Se comprueba sintaxis
        printf("USAGE:	./reader filename \n");
        return 0;
    }
    pila = my_stack_read(argv[1]);
    if (!pila) { //Error leer pila
        printf("Couldn't open stack file %s \n",argv[1]);
        return 0;
    }
    int mida=my_stack_len(pila);
    printf("Stack length: %d\n",mida);
    imprimirpila(pila);
    int i=0;
    int max = 0, min = INT_MAX, sum = 0, avg;
    while(pila->top && i < 10) { //Bucle para calcular operaciones
        data = (int *) my_stack_pop(pila);
        sum += *data; 
        if (*data < min){
            min = *data; 
        } 
        if (*data > max) {
            max = *data; 
        }
        i++;
    }
    avg = sum/mida; //Media
    printf("Items: %d Sum: %d Min: %d Max: %d Average: %d\n", mida, sum, min, max, avg);
}
