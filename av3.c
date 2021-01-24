/*
Bibiloni Fontirroig, Josep Antoni
Chellak El Ouahabi, Mohamed Rida
Socias Forteza, Antoni 
*/
#include <pthread.h>  
#include "my_lib.h"


#define NUM_THREADS 10
#define N 1000000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Función:  worker
 * --------------------
 * 
 *  Función con un bucle con N iteraciones
 *  En cada iteración se extraerá un elemento de la pila, 
 *  se incrementará en uno el valor del entero de datos, 
 *  y se volverá a insertar en la pila. 
 *  Ya que son hilos se utilizan mutex en las secciones criticas
 *  de pop y push
 * 
 *  void *ptr: puntero a la pila
 *
 */
void *worker(void *ptr) {
    struct my_stack *stack = (struct my_stack*)ptr;
    for (int i = 0; i < N; i++) {   //N iterations
        pthread_mutex_lock(&mutex);
        int *data= (int *) my_stack_pop(stack); 
        pthread_mutex_unlock(&mutex);
        (*data)++;  //Incrementamos dato
        pthread_mutex_lock(&mutex);
        my_stack_push(stack, data);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

/*
 * Función:  main
 * --------------------
 * 
 *  Primero se prepara la pila, se inician los hilos 
 *  con la función worker, hasta que acaben. Después se
 *  vuelca la pila en el fichero
 *
 */
int main(int argc, char *argv[]) {

    struct my_stack *pila;
    int nodes = 0; //Contador nodos
    int elements;  //Elementos escritos

    if (argv[1] == NULL) {  //Sintaxis incorrecta
        printf("USAGE:\t ./av3 filename \n");
        return 0;
    }
    pila = my_stack_read(argv[1]);
    printf("Threads: %i, Iterations: %i \n",NUM_THREADS,N);
    if (pila!=NULL) {  //Pila existe
        printf("stack->size: %d\n",pila->size);
        printf("original stack content:\n");
        imprimirpila(pila);
        printf("original stack lenght: %i\n",my_stack_len(pila));
        nodes = my_stack_len(pila);
        while (nodes<10) {
            int *data = malloc(sizeof(int));
            *data = 0;
            my_stack_push(pila, data);
            nodes++;
        }
    } else {  //Pila no existe
        pila = my_stack_init(sizeof(int));
        printf("stack->size: %d\n",pila->size);
        printf("original stack content:\n");
        printf("original stack lenght: %i\n",my_stack_len(pila));
        for (int i = 0;i<10;i++) {
            int *data = malloc(sizeof(int));
            *data = 0;
            my_stack_push(pila, data);
        }

    }
    printf("stack content for treatment:\n");
    imprimirpila(pila);
    printf("new stack lenght: %i\n",my_stack_len(pila));

    pthread_t thread[NUM_THREADS]; //Array identificadores hilos

    int r;
    for(size_t i=0;i<NUM_THREADS;i++){
        r = pthread_create(&thread[i],NULL,worker,pila);
        if(r){  //Comprobamos si hay un error
            printf("ERROR");
            exit(-1);
        }else{
            printf("%li) Thread %ld created \n",i,thread[i]);
        }
    }
    //Se bloquea el hilo principal hasta que acaben los hilos
    for (size_t i = 0; i < NUM_THREADS; ++i){  
        pthread_join(thread[i],NULL);
    }
    printf("stack content after threads iterations:\n");
    imprimirpila(pila);
    printf("stack length: %i \n",my_stack_len(pila));
    elements=my_stack_write(pila,argv[1]);  //Elementos de la pila
    if(elements>=0){
        printf("Written elements from stack to file: %d \n",elements);
    }else{
         printf("Error en write \n");
    }
    printf("Released bytes: %d \n", my_stack_purge(pila));
    puts("Bye from main");
    return 1;
}
