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



// Fonction pour afficher la liste circulaire avec flèches
void print_list(struct circular* cycle) {
    if (cycle->head == NULL) {
        printf("Liste vide\n");
        return;
    }

    struct node* current = cycle->head;
    for (unsigned int i = 0; i < cycle->size; i++) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("(retour à la tête: %d)\n", cycle->head->value);
}

int main() {
    struct circular* c = mkcircular();

    // Premier test
    printf("Ajout 10\n");
    insert(c, 10);
    print_list(c);

    // Deuxième test
    printf("Ajout 20\n");
    insert(c, 20);
    print_list(c);

    // Troisième test
    printf("Ajout 30\n");
    insert(c, 30);
    print_list(c);

    return 0;
}

