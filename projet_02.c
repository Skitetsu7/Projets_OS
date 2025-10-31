#include <stdio.h>  //printf
#include <stdlib.h> // EXIT_SUCCESS, malloc(), calloc(), NULL

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

struct node *rotateToEven(struct circular *cycle) {
  /* Run trough all element of a list and it return the first even's element else
  it does a rotation*/
  if (cycle->size == 0) { // list empty -> return NULL
    printf("The list is empty");
    return NULL;
  }

  unsigned int steps = 0; // initialise steps
  printf("The list is : ");
  print_list(cycle);
  //  first rotation
  cycle->head = cycle->head->next;
  printf("The list after rotation is :");
  print_list(cycle);
  // I must increment step by one because head is on the second element
  // must have the same value as head
  steps++;
  // loop until we found an even element
  while (steps < cycle->size) {
    // break point if even
    if ((cycle->head->value % 2) == 0) {
      printf("The final list is : ");
      print_list(cycle);
      return cycle->head; // return pointer
    }
    // if odd we rotate
    cycle->head = cycle->head->next;
    printf("The list after rotation is : ");
    print_list(cycle);
    steps++;
  }
  // if there is no even element -> return nul
  return NULL;
}

struct node *rotateToOdd(struct circular *cycle) {
  /* Run trough all element of a list and it return the first odd's element else
  it does a rotation*/
  if (cycle->size == 0) { // list empty -> return NULL
    printf("The list is empty");
    return NULL;
  }

  unsigned int steps = 0; // initialise steps
  printf("The list is : ");
  print_list(cycle);
  //  first rotation
  cycle->head = cycle->head->next;
  printf("The list after rotation is :");
  print_list(cycle);
  // I must increment step by one because head is on the second element
  // must have the same value as head
  steps++;
  // loop until we found an odd element
  while (steps < cycle->size) {
    // break point if odd
    if ((cycle->head->value % 2) != 0) {
      printf("The final list is : ");
      print_list(cycle);
      return cycle->head; // return pointer
    }
    // if even continue rotate
    cycle->head = cycle->head->next;
    printf("The list after rotation is : ");
    print_list(cycle);
    steps++;
  }
  // if there is no even element -> return nul
  return NULL;
}


int main() {
  struct circular *c = mkcircular();
  int choix;
  signed int choix_bis;
  while(1){
    printf("1) Type '1' if you want to display the linked list \n2) Type '2' if you want to insert an element at the end of the list \n3) Type '3' if you want to remove the element at the beginning of the list \n4) Type '4' if you want to reverse the list until it reaches an even number \n5) Type '5' if you want to reverse the list until it reaches an odd number\n ");
    printf("Give your choice: ");
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
