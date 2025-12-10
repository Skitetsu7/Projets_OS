#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define BUFFER_ACTIF_MEMBER 100
#define BUFFER_INACTIF_MEMBER 100
const char path[] = "tp5_base_de_donné.txt";
void print_BD(int file) {
  /*print the list of actif and inactif members
   */
  char line[BUFFER_SIZE]; // all initialization
  char actif_member[BUFFER_ACTIF_MEMBER][BUFFER_SIZE];
  char inactif_member[BUFFER_INACTIF_MEMBER][BUFFER_SIZE];
  char no_statut_member[BUFFER_INACTIF_MEMBER][BUFFER_SIZE];
  unsigned int index = 0;
  unsigned int actif_count = 0;
  unsigned int inactif_count = 0;
  unsigned int no_statut_count = 0;
  char ch;
  while (read(file, &ch, 1) != 0) {  // read the file char by char
    if (ch == '\n') {                // line end
      line[index] = '\0';            // string end
      if(strstr(line, "actif") != NULL){
        if (strstr(line, "inactif")) { // if inactif member
            strcpy(inactif_member[inactif_count],line);    // copy the line in inactif member
            inactif_count++; // increasse the number of inactif

        } 
        else {                                   // if actif member
            strcpy(actif_member[actif_count], line); // copy the line in actif
                                                    // member
            actif_count++;                           // increase the number of actif
        }
      }
    else{
        strcpy(no_statut_member[no_statut_count], line); // copy the line in no statut member
            no_statut_count++;                                            
    }
      index = 0; // reser index for the next line
    } 
    else {
      line[index] = ch; // add the char in line
      index++;
    }
  }
  if (index > 0) {
    line[index] = '\0';            // string end
    if(strstr(line, "actif") != NULL){
        if (strstr(line, "inactif")) { // if inactif member
            strcpy(inactif_member[inactif_count],line);    // copy the line in inactif member
            inactif_count++; // increasse the number of inactif

        } 
        else {                                   // if actif member
            strcpy(actif_member[actif_count], line); // copy the line in actif
                                                    // member
            actif_count++;                           // increase the number of actif
        }
      }
    else{
        strcpy(no_statut_member[no_statut_count], line); // copy the line in no statut member
            no_statut_count++;                                            
    }
  }
  printf("==============actif member=============\n"); // print actif member
  for (int i = 0; i < actif_count; i++) {
    printf("%s\n", actif_member[i]);
  }

  printf("\n ==============inactif member=============\n"); // print inactif member
  for (int i = 0; i < inactif_count; i++) {
    printf("%s\n", inactif_member[i]);
  }
  
  if (no_statut_count>0) {
    printf("\n ==============no statut member=============\n");
    for (int i = 0; i < no_statut_count; i++) {
        if (strlen(no_statut_member[i])>0){
            printf("%s\n", no_statut_member[i]);
          }
      }
  }
}


int suppression(const char file_path[]) {
  char email[50];
  char buffer[BUFFER_SIZE];
  bool found = false;
  int file = open(file_path, O_RDONLY), line_end = 0, right = 0;
  if (file == -1) {
    perror("Erreur d'ouverture du fichier");
    return -1;
  }
  int bytes_read = read(file, buffer, BUFFER_SIZE);
  if (bytes_read < 0) {
    perror("read");
    close(file);
    return EXIT_FAILURE;
  }
  close(file);
  printf("Donne l'adresse mail: \n");
  scanf("%s", email);     
  char *search = strstr(email, "@");
  for (int j = 0; j <= bytes_read; j++) { // loop on the file
    if (j == bytes_read || buffer[j] == '\n') { // stop each buffer by a line (j==bytes_real because at end we don't have \n)
            int line_start = j;    // start by the end of each line
            found = false;
            // search line of the email on the buffer
            for (int i = line_end; i < line_start; i++) {
                if(buffer[i]==email[0] && search!=NULL){  //check that the first character is correct and that @ is in the word
                    right = 0;
                    while (1) {
                        if (email[right] != buffer[i + right]) { //checks that the rest of the characters are the same
                            break;
                            }
                        right++; // advance in the buffer and email too
                        }
                    if (email[right] == '\0') {
                        char end_email = buffer[i + right];
                        if(end_email == ' ' || end_email == '\n'){ //check that the entire email is written before deleting it
                            found = true; // if the email is find return found
                            break;
                            }
                        }
                    }
            }

            if (found == true) {
                int len = line_start-line_end; // the end minus the bigging to know the lenght
                for (int k = line_end; k + len < bytes_read; k++) {
                    int norme = k + len + 1;
                    buffer[k] = buffer[norme]; // necessary to erase the space
                    }
                    bytes_read -= len + 1; // update
                    j = line_end - 1; // go back to the bigging of the line
                } 
            else {
                line_end = j + 1; //update 2.0
            }
        }
    }
    file = open(file_path, O_WRONLY | O_TRUNC); // necessary to write to the file to delete
    if (file == -1) {
        perror("write");
        return -1;
    }
    write(file, buffer, bytes_read);
    close(file);
    return 0;
}

int add(const char file_path[]) {
  char name[20];
  char firstname[20];
  char email[50];
  char phonenumber[20];
  char status[10];

  int file = open(file_path, O_WRONLY | O_APPEND);
  if (file == -1) {
    perror("Erreur d'ouverture du fichier");
    return -1;
  }

  write(file, "\n", 1); // to go on the next line

  printf(" Please, write your name : ");
  scanf("%s", name);
  write(file, name, strlen(name));
  write(file, " ", 1); // to separe by a space in the file

  printf(" Please, write your firstname : ");
  scanf("%s", firstname);
  write(file, firstname, strlen(firstname));
  write(file, " ", 1);

  printf(" Please, write your email : ");
  scanf("%s", email);
  write(file, email, strlen(email));
  write(file, " ", 1);

  printf(" Please, write your phone number : ");
  scanf("%s", phonenumber);
  write(file, phonenumber, strlen(phonenumber));
  write(file, " ", 1);

  printf(" Please, write your status (actif or inactif) : ");
  scanf("%s", status);
  while (1) {
    if (strcmp(status, "actif") == 0 || strcmp(status, "inactif") == 0) {
      break;

    } else {
      printf("Please give a value between (actif or inactif) : ");
      scanf("%s", status);
    }
  }
 
  write(file, status, strlen(status));

  close(file);
  return 0;
}


void reset_buffer(char *buffer, int size) {
  for (int i = 0; i < size; i++)
    buffer[i] = 0;
}

int search(int file, char *word) {
  // init var
  char buff[1024];
  char line[1024];
  char *search = strstr(word, "@");

  int i = 0;
  int k = 0;
  int somewhere = 0;
  long int bytes = 0;

  bytes = read(file, buff, sizeof(buff));

  printf("file size : %ld\n", bytes);

  while (i < bytes) {
    // fill line
    line[k] = buff[i];
    // end a line
    if (line[k] == '\n') {
      // end line with '\0'
      line[k] = 0;
      // look for word
      if (strstr(line, word) && search != NULL) {
        printf("%s\n", line);
        somewhere++;
      }
      // reset line
      reset_buffer(line, sizeof(line));
      k = -1;
    }
    k++;
    i++;
  }
  if (somewhere == 0)
    printf("Nothing mention %s\n", word);

  return EXIT_SUCCESS;
}

int init_search(const char file_path[]) {
  // init
  char word[1024];
  // ask useer
  printf("Research email :\n");
  scanf("%s", word);
  int file = open(file_path, O_RDONLY);
  if (file == -1) {
    printf("ERROR : Opening file failed;\n");
    return EXIT_FAILURE;
  }
  search(file, word);
  return EXIT_SUCCESS;
}

int main() {
  int choice;
  while (1) { // boucle infinie
    printf("\n===== MENU =====\n");
    printf("1. Display \n");
    printf("2. Search \n");
    printf("3. Delete \n");
    printf("4. Add \n");
    printf("0. Leave \n");
    printf("Give your choice : ");
    scanf("%d", &choice);
    printf("\n");
    switch (choice) {
    case 0:
      printf("This is the end\n");
      return 0;
    case 1:
      print_BD(open(path, O_RDONLY));
      break;
    case 2:
      init_search(path);
      break;
    case 3: {
      suppression(path);
      break;
    }
    case 4: {
      add(path);
      break;
    }
    default:
      printf("Invalid choice .\n");
      break;
    }
  }
}
