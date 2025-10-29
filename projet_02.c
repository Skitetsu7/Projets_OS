#include <stdio.h> //printf
#include <stdlib.h> // EXIT_SUCCESS, malloc ( . . ) , c a l l o c ( . . ) , NULL

struct node {  //structure of a node
    signed int value;
    struct node *next;
};

struct circular {  //structure of the main circular list
    unsigned int size;
    struct node *head;
};

struct circular* mkcircular() {  // create a new circular list
    struct circular *list = malloc(sizeof(struct circular)); //dynamic memory allocation
    list->size = 0; 
    list->head = NULL;
    return list;
}

struct node* mknode (signed int v) {   //create a new node
    struct node* nd = malloc ( sizeof(struct node)); //dynamic memory allocation of the new node
    nd -> value = v ;
    nd -> next = nd ;
    return nd ; 
}


void insert(struct circular* cycle, signed int elt) { //insert a new node in the list
    struct node* new_node = mknode(elt); //use mknode for create the new node

    if (cycle->head == NULL) {  // if empty list
        cycle->head = new_node;  // new node become head
        new_node->next = new_node;  // boucle on himself
    } else {   // list not empty
        struct node* predecessor = cycle->head; //begin at the head
        while (predecessor->next != cycle->head) { //find the last element
            predecessor = predecessor->next;
        }  // add new element
        predecessor->next = new_node; 
        new_node->next = cycle->head; 
    }

    cycle->size++; //increase the size of the list
}



int main() {    // main fonction for test
    struct circular* c = mkcircular();  

    insert(c, 10);
    insert(c, 20);
    insert(c, 30);

    printf("Taille de la liste : %u\n", c->size);

    // Affichage des valeurs pour vérifier la circularité
    struct node* current = c->head;
    for (int i = 0; i < c->size; i++) {
        printf("Valeur : %d\n", current->value);
        current = current->next;
    }

    // Vérification que le dernier revient bien sur le premier
    printf("Retour au début ? %s\n", (current == c->head) ? "Oui" : "Non");

    return 0;
}


