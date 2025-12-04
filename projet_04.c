#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "colors.h"

#define PHILOSOPHER_COUNT 5
#define NONE (-1)
#define ABSENT 0
#define PRESENT 1

typedef signed long identifier;
typedef pthread_mutex_t mutex;

#define LEFT(i) ((i) % PHILOSOPHER_COUNT)
#define RIGHT(i) ((i + 1) % PHILOSOPHER_COUNT)

pthread_t philosophers[PHILOSOPHER_COUNT];
mutex chopsticks[PHILOSOPHER_COUNT];
// Is a given philosopher waiting on a chopstick or NONE ?
identifier waitOn[PHILOSOPHER_COUNT];
// Is a given chopstick held by a philosopher or NONE ?e
identifier heldBy[PHILOSOPHER_COUNT];

const char*couleur(identifier me){
    int identifiant_philosophe= me%PHILOSOPHER_COUNT;
    // assigning a color to each philosopher
    switch(identifiant_philosophe){
        case 0:
        return Red;
        case 1:
        return Blue;
        case 2:
        return Yellow;
        case 3:
        return Green;
        case 4:
        return Purple;
    return normal;
    }
}

void think(identifier me) {
  unsigned int delay = (rand() % 5) + 1;
  printf("%s[%ld] Starting to THINK ... (%u)\n",couleur(me), me, delay);
  sleep(delay);
}

void eat(identifier me) {
  unsigned int delay = (rand() % 5) + 1;
  // baguette detenue
  int droite= (heldBy[RIGHT(me)] == me), gauche= (heldBy[LEFT(me)] == me);
  printf("%s[%ld] Starting to EAT ... (%u): ",couleur(me), me, delay);
  if(gauche && droite){
    printf("LEFT(%ld) - RIGHT(%ld)\n", LEFT(me), RIGHT(me));
  }
  else{
    printf("%s Il ne peut pas manger normalement\n", grey);
  }
  sleep(delay);
}

int detect_deadlock() {
  /* A deadlock in this problem has a very specific shape and involves ALL
          philosophers.In order to check whether it has arisen,
      we count the number of times that from the current philosopher(currentP)
          it happens to waitOn a chopstick which is heldBy the next philosopher,
      updating currentP to that next philosopher.Once the count reaches
          PHILOSOPHER_COUNT,
      we’ve gone around the table and confirmed the deadlock is
          present.Otherwise,
      we’ll stop before, thus confirming there is no deadlock.
*/ 
unsigned int count = 0;
  identifier current_philosopher = 0;
  identifier current_chopstick = waitOn[current_philosopher];
  // il y a une baguette il y a un philomec on est pas à la fin 
  while (current_chopstick != NONE && heldBy[current_chopstick] != NONE &&
         count < PHILOSOPHER_COUNT) {
    
    current_philosopher = heldBy[current_chopstick];
    current_chopstick = waitOn[current_philosopher];
    count++;
  }
  return (count == PHILOSOPHER_COUNT) ? PRESENT : ABSENT;
}



void release(identifier me, identifier chopstick_id) {
  printf("%s[%ld] Releasing chopstick %ld\n", couleur(me), me, chopstick_id);
  heldBy[chopstick_id] = NONE;
  pthread_mutex_unlock(&chopsticks[chopstick_id]);
}

int acquire(identifier me, identifier chopstick_id) {

  printf("%s[%ld] Picking up chopstick %ld\n",couleur(me), me, chopstick_id);

  waitOn[me] = chopstick_id;

  if (pthread_mutex_trylock(&chopsticks[chopstick_id])){
    waitOn[me] = NONE;
    //printf("RIGHT :if (%ld == %d && %d == %d)\n",chopstick_id,RIGHT(me), heldBy[LEFT(me)],me);
    if (chopstick_id == RIGHT(me) && heldBy[LEFT(me)]==me)
    {release(me, LEFT(me));}
    //printf("LEFT :if (%ld == %d && %d == %d)\n",chopstick_id,LEFT(me),heldBy[RIGHT(me)],me);
    if (chopstick_id == LEFT(me) && heldBy[RIGHT(me)]==me)
    {release(me,RIGHT(me));}
  
    return -1;
  }

  if (detect_deadlock() == PRESENT)
    printf("DEADLOCK !\n");


  waitOn[me] = NONE;
  heldBy[chopstick_id] = me;
  printf("%s[%ld] Acquired chopstick %ld\n",couleur(me), me, chopstick_id);
  return 0;
  
}

void *philosophize(void *argument) {
  identifier me = (unsigned long)argument;
  int l,r;

  while (1) {
    l = -1;
    r = -1;
    think(me);
    // Acquiring chopsticks

    if (l){
      l = acquire(me, LEFT(me));}
    sleep(6); // Forcing deadlock occurrence
    if (r){
      r = acquire(me, RIGHT(me));}
  
    if(!l&&!r){
    eat(me);
    // Releasing chopsticks
    release(me, LEFT(me));
    release(me, RIGHT(me));
    }
  }
}

int main(int argc, char *argv[]) {
  srand(time(0));
  for (identifier i = 0; i < PHILOSOPHER_COUNT; i++) {
    waitOn[i] = NONE;
    heldBy[i] = NONE;
    pthread_mutex_init(&chopsticks[i], NULL);
  }
  for (identifier p = 0; p < PHILOSOPHER_COUNT; p++)
    pthread_create(&philosophers[p], NULL, philosophize, (void *)p);
  for (identifier p = 0; p < PHILOSOPHER_COUNT; p++)
    pthread_join(philosophers[p], NULL);
  for (unsigned int b = 0; b < PHILOSOPHER_COUNT; b++)
    pthread_mutex_destroy(&chopsticks[b]);
  return EXIT_SUCCESS;
}
