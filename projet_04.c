#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

void think(identifier me) {
  unsigned int delay = (rand() % 5) + 1;
  printf("[%ld] Starting to THINK ... (%u)\n", me, delay);
  sleep(delay);
}

void eat(identifier me) {
  unsigned int delay = (rand() % 5) + 1;
  printf("[%ld] Starting to EAT ... (%u)\n", me, delay);
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

void acquire(identifier me, identifier chopstick_id) {
  printf("[%ld] Picking up chopstick %ld\n", me, chopstick_id);
  waitOn[me] = chopstick_id;
  if (detect_deadlock() == PRESENT)
    printf("DEADLOCK !\n");
  pthread_mutex_lock(&chopsticks[chopstick_id]);
  waitOn[me] = NONE;
  heldBy[chopstick_id] = me;
  printf("[%ld] Acquired chopstick %ld\n", me, chopstick_id);
}

void release(identifier me, identifier chopstick_id) {
  printf("[%ld] Releasing chopstick %ld\n", me, chopstick_id);
  heldBy[chopstick_id] = NONE;
  pthread_mutex_unlock(&chopsticks[chopstick_id]);
}

void *philosophize(void *argument) {
  identifier me = (unsigned long)argument;
  while (1) {
    think(me);
    // Acquiring chopsticks
    if (me == PHILOSOPHER_COUNT-1){
      acquire(me, RIGHT(me));
      sleep(6); // Forcing deadlock occurrence
      acquire(me, LEFT(me));
      eat(me);
      // Releasing chopsticks
      release(me, RIGHT(me));
      release(me, LEFT(me));
    }
    else {
    acquire(me, LEFT(me));
    sleep(6); // Forcing deadlock occurrence
    acquire(me, RIGHT(me));
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
