#include <signal.h> //signal
#include <stdio.h> //printf
#include <stdlib.h> //exit_success
#include <unistd.h> //alarm
//define variable
#define False 0
#define board_size 100
int tab[board_size];
int hit = 0;

void capitalize_letter() {
  /*
  this function is use for capitalize the input
  */
  
  for (int j = 0; j < hit; j++) {
    if (tab[j] != '\n' && tab[j] >= 'a' && tab[j] <= 'z') {
      // print the input in capitals letters
      tab[j] -= 32;
    }
  printf("%c", tab[j]);
  }
}

void handler_alarm(int sig){
  /*
  This function capture the alarm, print and reinitialize the buffer
  */
  // If nothing was input on keybord the code end
  if (hit == 0) {
    printf("\n nothing was input\n");
    exit(EXIT_SUCCESS);
  }
  capitalize_letter();
  printf("\nYou have only five second \n");
  // reinitialize the buffer and recall alarm
  hit = 0;
  alarm(5);
}

int main() { //main function; call the others function
  int s;
  signal(SIGALRM, handler_alarm);
  printf("You have only five second to write: \n");
  alarm(5);
  while (!False) {
    s = getchar();
    if (s != '\n' && hit < board_size - 1) {
      tab[hit++] = s;
    }
  }
  return EXIT_FAILURE;
}
