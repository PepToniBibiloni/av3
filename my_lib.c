//Josep Antoni Bibiloni Fontirroig
//Mohamed Rida Chellak El Ouahabi
//Antoni Socias Forteza
#include <pthread.h>
#include <stdio.h>
#include "my_lib.h"

/*
 * Funció:  imprimirpila
 * --------------------
 * 
 * Función para imprimir pila
 */
void imprimirpila(struct my_stack *pila){
    struct my_stack_node *node=pila->top;  
    struct my_stack_node *aux=NULL; 
    int *data = (int *) malloc(sizeof(int));
    while (node != NULL){
        aux= node->next;
        data=node->data;
        printf("%i\n",*data);
        node= aux;
    }
}

/*
 * Funció:  my_stack_init
 * --------------------
 * 
 * Reserva espaci per una variable de tipus struct my_stack, que contendrá 
 * el punter al node superior de la pila i la mida de les dades, e inicializa 
 * els valors
 * 
 *  size: Mida de les dades de la pila
 *
 *  returns: punter de la pila inicialitzada
 */

struct my_stack *my_stack_init (int size){
   //Cream espai per la pila
   struct my_stack *pila = malloc(sizeof(struct my_stack));
   //Inicialitzam la pila
   pila->top= NULL;
   pila->size = size;

   return pila;
}

/*
 * Funció:  my_stack_push
 * --------------------
 * 
 * Inserta un nou node a la pila. El punter a les dades de aquest node estan per parametre.
 * Paràmetres: punter pila allà on insertarem les dades,punter a les dades que s'inseriran
 * a la pila 
 * 
 *  *stack: Punter de la pila
 *  *data: Punter de les dades
 *
 *  returns: 0 si ha anat bé i -1 si ha fallat
 */

int my_stack_push (struct my_stack *stack, void *data) {
    
    struct my_stack_node *node;
    //Si la pila ni el node existeix llavors:
    if((stack !=NULL)&&(data != NULL)){
        //Reservam memòria per el node i feim les asignacions.
        node = (struct my_stack_node*)malloc(sizeof(struct my_stack_node));
        if (node == NULL)
        {
            return -1;
        }else
        {
            node->data = data; //Allotjam les dades al node
            if (stack->top != NULL) //Colocam el node al principi mirant si esta buida o no
            {
                node->next = stack->top;
                stack->top = node;
            }else{
                stack->top = node;
                node->next = NULL;
            }
            return 0;
        }
    }else
    {
        return -1;  //Retornam -1 si ha fallat
    }
}


/*
 * Funció:  my_stack_pop
 * --------------------
 * 
 * Elimina el node superior dels elements de la pila. Retorna el punter a les dades del element eliminat. 
 * 
 *  *stack: Punter de la pila
 *
 *  returns: les dades del node o NULL si ha anat malament
 */

void *my_stack_pop (struct my_stack *stack){
    struct my_stack_node *node=NULL;
    //Si no existeix el node superior retorna NULL.
    if (stack->top == NULL)
    {
        return NULL;
    }else
    {
        node = stack->top; //Agafam el node superior de la pila
        stack->top = node->next; //Feim que la pila apunti al segon node
        return node->data;
        free(node); //Alliberam espai
        node = NULL;
    }
}

/*
 * Funció:  my_stack_len 
 * --------------------
 * 
 * Recorr la pila i retorna el nombre de nodes totals que te la pila
 * 
 *  *stack: Punter de la pila
 *
 *  returns: Nombre de nodes
 */

int my_stack_len (struct my_stack *stack){
    
    struct my_stack_node *node=stack->top; //Cream el punter que apunti al node superior.
    int num=0;
    while (node != NULL)
    {
        node= node->next;
        num++;
    }
    return num;
    
}
/*
 * Funció:  my_stack_purge
 * --------------------
 * 
 * Recorr tota la pila alliberant els espais que previament havien estat reservat
 * per cada una de les dades i nodes. Al final tambe allibera l'espai
 * en memòria que ocupa la pila.
 * 
 *  *stack: Punter de la pila
 *
 *  returns: Bytes alliberats
 */

int my_stack_purge (struct my_stack *stack){
    struct my_stack_node *node=stack->top;  //Apuntam al primer node
    struct my_stack_node *aux=NULL; //Node auxiliar
    int contador=0;
    //Si la pila està inicialitzada llavors
    while (node != NULL)
    {
        aux= node->next; //Avançam al següent node.
        contador += stack->size; 
        contador += sizeof(struct my_stack_node); //Afegim la mida del struct del node
        node= aux;
    }
    //Alliberam tots l'espai ocupat per node,aux i stack
    free(node);
    free(aux);
    free(stack);
    contador += sizeof(struct my_stack);  //Afegim la mida del struct de la pila
    return contador;

}

/*
 * Funció:  my_stack_write
 * --------------------
 * 
 * Escriu les dades que hi ha dina la pila dins el fitxer indicat per filename
 * 
 *  *stack: Punter de la pila
 *  *filename: Fitxer
 *
 *  returns: Retorna la quantitats de nodes que hi ha o -1 si hi ha algun error 
 */

int my_stack_write (struct my_stack *stack, char *filename){
    //Obrim el fitxer
    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file == -1){    
        return -1; //En cas d'error retornam -1
    }
    int nodes = 0;
    //Escrivim al fitxer la mida dels nodes
    if (write(file, &(stack->size), sizeof(int)) != sizeof(int)){
        return -1; //En cas d'error retornam -1
    };
    
    //Utilitzam la nostra funció recursiva per girar la pila
    my_stack_turn(stack, stack->top, &nodes,file);

    //Tancam el fitxer
    if (close(file) == -1){
        return -1;
    }
    return nodes;
}

/*
 * Funció:  my_stack_turn
 * --------------------
 * 
 * Funció recursiva per poder girar l'ordre de la pila, on anam cridant a la mateixa funció
 * cada vegada amb el node següent fins que arribam al darrer node 
 * 
 *  *stack: Punter de la pila
 *  *node: Punter del node
 *  *nodes: Contador de nodes
 *  *file: Fitxer
 *
 *  returns:  
 */

void my_stack_turn(struct my_stack *stack, struct my_stack_node *node, int *nodes, int file){
    if(node != NULL){
        my_stack_turn(stack, node->next, nodes,file );
        
        if(write(file, node->data, stack->size) == stack->size){
            (*nodes)+=1;
        }
    }
}

/*
 * Funció:  my_stack_read
 * --------------------
 * 
 * Funció de lectura que llegeix les dades de dins el fitxer i les fica dins la pila. 
 * 
 *  *file: Filename
 *
 *  returns: Punter de la pila i null si hi ha error
 */

struct my_stack *my_stack_read (char *filename){
    int file = open(filename, O_RDONLY);
    int mida = 0;
    if (read(file, &mida, sizeof(int)) != sizeof(int)){
        return NULL;
    };
    struct my_stack *stack= my_stack_init(mida);
    void *data = malloc(mida); //Guardam espai per les dades. 
    //Llegim les dades fins trobar un error
    while(read(file,data,mida)>0){
        my_stack_push(stack,data);
        data = malloc(mida);
    }
    free(data); //Alliberam l'espai extra del bucle
    data = NULL;
    close(file); // Tancam l'arxiu
    return stack;
}
