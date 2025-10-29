#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// circular is the chained list
struct circular {
  signed int current_value;
  struct circular *next_value;
};

struct circular *mkcircular() {
  // make space to place our elements
  struct circular *cycle = malloc(sizeof(struct circular));
  assert(cycle != NULL);
  return cycle;
}

void insert(struct circular *cycle, signed int elt) {
  /*
   insert an elements in the circular chained_list
   ----------------------------------------------------
   struct circular cycle : is the last elements
   signed int elt : value that you want to add to the list
   */
  struct circular *new_element = malloc(sizeof(struct circular));
  assert(new_element != NULL);

  new_element->current_value = elt;

  // if it is the first element it must reference itself
  if (cycle->next_value == NULL) {
    new_element->next_value = new_element;
  } else {
    /*
     if it is not the first element we must
     attribuate the last link to the newest

     for exemple 1 <-> 2 with insert(3) will become

     1-->2
     ^   |
     |   v
     └-- 3
     */
    new_element->next_value = cycle->next_value;
  }
  cycle->next_value = new_element;
}

int main() {
  struct circular *list = mkcircular();
  
  insert(list, 1);
  printf("current : %d\tnext : %d\n", list->next_value->current_value,
         list->next_value->next_value->current_value);
  
  insert(list, 2);
  printf("current : %d\tnext : %d\n", list->next_value->current_value,
         list->next_value->next_value->current_value);
  
  insert(list, 3);
  printf("current : %d\tnext : %d\n", list->next_value->current_value,
         list->next_value->next_value->current_value);
  return EXIT_SUCCESS;
}
