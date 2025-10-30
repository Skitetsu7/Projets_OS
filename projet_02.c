#include <stdio.h>  //printf
#include <stdlib.h> // EXIT_SUCCESS, malloc ( . . ) , c a l l o c ( . . ) , NULL

struct node { // structure of a node
  signed int value;
  struct node *next;
};

struct circular { // structure of the main circular list
  unsigned int size;
  struct node *head;
};

struct circular *mkcircular() { // create a new circular list
  struct circular *list = malloc(sizeof(struct circular)); // dynamic memory allocation
  list->size = 0;
  list->head = NULL;
  return list;
}

struct node *mknode(signed int v) { // create a new node
  struct node *nd = malloc(sizeof(struct node)); // dynamic memory allocation of the new node
  nd->value = v;
  nd->next = nd;
  return nd;
}

void insert(struct circular *cycle, signed int elt) {          // insert a new node in the list
  struct node *new_node = mknode(elt); // use mknode for create the new node

  if (cycle->head == NULL) {                   // if empty list
    cycle->head = new_node;                    // new node become head
    new_node->next = new_node;                 // boucle on himself
  } else {                                     // list not empty
    struct node *predecessor = cycle->head;    // begin at the head
    while (predecessor->next != cycle->head) { // find the last element
      predecessor = predecessor->next;
    } // add new element
    predecessor->next = new_node;
    new_node->next = cycle->head;
  }

  cycle->size++; // increase the size of the list
}

signed int extract(struct circular *cycle) {
  /*remove the first element, in other words, the element with the very first
   address, and transform the second element into the first.
  */
  struct node *predecessor = cycle->head;
  struct node *new_node = cycle->head;
  signed int result = predecessor->value;
  if (cycle->head == NULL) {
    printf("no value, empty file \n");
    return 1;
  } else if (cycle->head->next == cycle->head) {
    cycle->head = NULL; // If only one element remains, the list is cleared
  } else {
    while (predecessor->next != cycle->head) { // find the last element
      predecessor = predecessor->next;
    }
    predecessor->next = new_node->next; // once the last element is found, it is connected to the second one
    cycle->head = new_node->next; // the second element becomes the head of the thread
  }
  if (cycle->size > 0) {
    cycle->size--;
  }
  return result;
}

struct node *rotateToEven(struct circular *cycle) {
  /*Cette fonction doit parcourir la liste et dès qu'elle tombe sur un élément
   * pair , doit renvoyer cette élément, sinon faire une rotation*/
  if (cycle->size == 0) { // si la liste est vide on return NULL
    printf("The list is empty");
    return NULL;
  }

  unsigned int steps = 0; // initialise steps
  printf("The list is : ");
  print_list(cycle);
  //  1er rotation obligatoire
  cycle->head = cycle->head->next;
  printf("The list after rotation is :");
  print_list(cycle);
  // je dois implémenter step de 1 car head est mtn sur le 2 ème élément et step
  // doit avoir la même valeur que head
  steps++;
  // boucle pour parcourir toute la liste jusqu'a que j'ai un élément pair
  while (steps < cycle->size) {
    // si l'élément est pair on s'arrette ici
    if ((cycle->head->value % 2) == 0) {
      printf("The final list is : ");
      print_list(cycle);
      return cycle->head; // on retourne le pointeur vers cet élément
    }
    // si c'est impair je continue à tourner
    cycle->head = cycle->head->next;
    printf("The list after rotation is : ");
    print_list(cycle);
    steps++;
  }
  // Si on a parcouru toute la liste sans avoir d'éléments pair , on return NULL
  return NULL;
}

struct node *rotateToOdd(struct circular *cycle) {
  /*Cette fonction doit parcourir la liste et dès qu'elle tombe sur un élément
   * impair , doit renvoyer cette élément, sinon faire une rotation*/
  if (cycle->size == 0) { // si la liste est vide on return NULL
    printf("The list is empty");
    return NULL;
  }

  unsigned int steps = 0; // initialise steps
  printf("The list is : ");
  print_list(cycle);
  //  1er rotation obligatoire
  cycle->head = cycle->head->next;
  printf("The list after rotation is :");
  print_list(cycle);
  // je dois implémenter step de 1 car head est mtn sur le 2 ème élément et step
  // doit avoir la même valeur que head
  steps++;
  // boucle pour parcourir toute la liste jusqu'a que j'ai un élément impair
  while (steps < cycle->size) {
    // si l'élément est impair on s'arrette ici
    if ((cycle->head->value % 2) != 0) {
      printf("The final list is : ");
      print_list(cycle);
      return cycle->head; // on retourne le pointeur vers cet élément
    }
    // si c'est pair je continue à tourner
    cycle->head = cycle->head->next;
    printf("The list after rotation is : ");
    print_list(cycle);
    steps++;
  }
  // Si on a parcouru toute la liste sans avoir d'éléments impair , on return
  // NULL
  return NULL;
}

void print_list(struct circular *cycle) {
  /*The function displays the circular linked list in FIFO mode
  */
  if (cycle->head == NULL) {
    printf("Empty list\n");
    return;
  }
  struct node * predecessor = cycle->head; // begin at the head
  unsigned int i= 0;
  while(i<cycle->size){
    printf("%d -> ", predecessor->value); // displays a list
    predecessor = predecessor->next; //continuation to the next element
    i++;
  }
  printf("(return to the head: %d)\n", cycle->head->value); //serves to remind us that the list is circular by showing the element at the top
}

int main() {
  struct circular *c = mkcircular();
  int choix;
  signed int choix_bis;
  while(1){
    printf("1) Type '1' if you want to display the linked list \n2) Type '2' if you want to insert an element at the end of the list \n3) Type '3' if you want to remove the element at the beginning of the list \n4) Type '4' if you want to reverse the list until it reaches an even number \n5) Type '5' if you want to reverse the list until it reaches an odd number\n ");
    scanf("%d", &choix);
    switch(choix){
      case 1:
        print_list(c);
        break;
      case 2:
        printf("provide an integer to add to the end of the list: \n");
        scanf("%d", &choix_bis);
        insert(c, choix_bis);
        print_list(c);
        break;
      case 3:{
        signed int extract_value = extract(c);
        printf("%d\n", extract_value);
        print_list(c);
        break;
      }
      case 4:
        rotateToEven(c);
        break;
      case 5:
        rotateToOdd(c);
        break;
      default:
        printf("invalid choice, start again\n");
    }
  }
    return EXIT_SUCCESS;
}
