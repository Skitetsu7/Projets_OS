#include <signal.h> //signal
#include <stdio.h> //printf
#include <stdlib.h> // EXIT_SUCCESS, malloc ( . . ) , c a l l o c ( . . ) , NULL
#include <unistd.h>

// code du prof remasterisé
struct fifo {
    unsigned int size ;
    struct circular * head ;
};
// code du prof remasterisé
struct circular {
    unsigned int size ;
    struct circular * next ;
};

struct circular * mkcircular (){
    /*creates the circular linked list
    */
    struct circular * first = malloc (sizeof (struct circular));
    struct circular * last = malloc(sizeof(struct circular ));
    first->size=0; //initialisation à 0
    // on crée une boucle en liant le premier au dernière élément
    first->next  = last; 
    last->next   = first; 
    last->size   = 0; //initialisation à 0
    return last; //principe FIFO
}


void insert ( struct circular * cycle , signed int elt );
signed int extract ( struct circular * cycle );
struct node * rotateToEven ( struct circular * cycle );
struct node * rotateToOdd ( struct circular * cycle );


int main() {
    return EXIT_SUCCESS;
}
