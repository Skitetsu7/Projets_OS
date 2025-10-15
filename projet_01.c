#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// exo groupe
#define taille_du_tableau 100
int tab[taille_du_tableau];
int frappe = 0;

void accumule_frappe() {
  // si il y a aucune frappe on quitte le code
  if (frappe == 0) {
    printf("\n Rien n'a été tapé depuis 5 secondes\n");
    exit(0);
  }
  for (int j = 0; j < frappe; j++) {
    // sert à manipuler m sans affecter la tab
    tab[j];
    if (tab[j] != '\n' && tab[j] >= 'a' && tab[j] <= 'z') {
      tab[j] -= 32;
    }
  printf("%c", tab[j]);
  }
  // sert à faire l'affichage des lettres tapées et de les remettre en majuscules
}

void alarm_handler(int sig){
  accumule_frappe();
  printf("\n");
  printf("écrit fdp t'as 5sec \n");
  // pour réinitialiser le buffer et remmettre le timer
  frappe = 0;
  alarm(5);
}

int main() {
  int s, i = 0;
  signal(SIGALRM, alarm_handler);
  printf("vous aves 5sec pour écrire une phrase: \n");
  alarm(5);
  while (1) {
    s = getchar();
    if (s != '\n' && frappe < taille_du_tableau - 1) {
      tab[frappe++] = s;
    }
  }
  return 0;
}
