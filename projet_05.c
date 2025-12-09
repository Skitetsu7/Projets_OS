#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER_SIZE 1024
#define BUFFER_ACTIF_MEMBER 100
#define BUFFER_INACTIF_MEMBER 100

void print_BD(int file){
    /*print the list of actif and inactif members
    */
    char line[BUFFER_SIZE];   //all initialization
    char actif_member[BUFFER_ACTIF_MEMBER][BUFFER_SIZE];
    char inactif_member[BUFFER_INACTIF_MEMBER][BUFFER_SIZE];
    unsigned int index = 0;
    unsigned int line_number = 1;
    unsigned int actif_count = 0;
    unsigned int inactif_count = 0;
    char ch ;


    while ( read ( file , &ch , 1) != 0){ //read the file char by char
        if (ch == '\n'){  //line end
            line [ index ] = '\0';  //string end
            if (strstr(line, "inactif")){  //if inactif member
                strcpy(inactif_member[inactif_count], line);  //copy the line in inactif member
                inactif_count++;  //increasse the number of inactif

            }
            else {  //if actif member
                strcpy(actif_member[actif_count], line);  //copy the line in actif member
                actif_count++;   //increase the number of actif
            }
             
            index = 0; //reser index for the next line
        }
        else {
            line[index] = ch; //add the char in line
            index++;
        }
    }
    printf("==============actif member=============\n");   //print actif member
    for(int i = 0; i< actif_count; i++){
        printf("%s\n", actif_member[i]);

    }
    
    printf("\n ==============inactif member=============\n");  //print inactif member
    for(int i = 0; i<inactif_count; i++){
        printf("%s\n", inactif_member[i]);
    }
}




int suppression(int argc,char const *argv[]){
    char email[50];
    char buffer[BUFFER_SIZE];
    bool found = false;
    int file = open(argv[0], O_RDONLY), line_end= 0, right = 0;
    if (file == -1) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }
    int bytes_read = read(file,buffer,BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
        close(file);
        return EXIT_FAILURE;
    }
    close(file);
    printf("Donne l'adresse mail: \n");
    scanf("%s", email);
    for(int j=0; j<=bytes_read; j++){//loop on the file
        if(j==bytes_read || buffer[j]=='\n'){//stop each buffer by a line (j==bytes_read because at end we don't have \n)
            int line_start=j; // start by the end of each line
            found=false;
            // search line of the email on the buffer
            for(int i=line_end; i<line_start; i++){
                right = 0;
                while(1){
                    if(email[right]!=buffer[i+right]){
                        break;
                    }
                    right++;
                }
                if(email[right]=='\0'){
                    found=true; // if the email is find
                    break;
                    }

            }  
            if(found==true){
                int len=line_start-line_end; // the end minus the bigging to know the lenght
                for(int k=line_end; k+len<bytes_read; k++){
                    int norme=k+len+1;
                    buffer[k]=buffer[norme]; //necessary to erase the space
                    }
                bytes_read-=len+1; //more forward
                j=line_end-1;
                }
            line_end=j+1;
            }
        }
    file = open(argv[0], O_WRONLY | O_TRUNC); // necessary to write to the file to delete
    if (file == -1) {
        perror("write");
        return -1;
    }
    write(file,buffer,bytes_read);
    close(file);
    return 0;
    }


int add(int argc,char const *argv[])
{   char name[20];
    char firstname[20];
    char email[50];
    char phonenumber[20];
    char status[10];
    
    int file = open(argv[0], O_WRONLY | O_APPEND);
    if (file == -1) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    

    write(file, "\n", 1); // to go on the next line

    printf(" Please, write your name : ");
    scanf("%s", name);
    for (int i = 0; name[i] != '\0'; i++)
        write(file, &name[i], 1);
    write(file, " ", 1);   // to separe by a space in the file 


    printf(" Please, write your firstname : ");
    scanf("%s", firstname);
    for (int i = 0; firstname[i] != '\0'; i++)
        write(file, &firstname[i], 1);
    write(file, " ", 1);

    
    printf(" Please, write your email : ");
    scanf("%s", email);
    for (int i = 0; email[i] != '\0'; i++)
        write(file, &email[i], 1);
    write(file, " ", 1);

    printf(" Please, write your phone number : ");
    scanf("%s", phonenumber);
    for (int i = 0; phonenumber[i] != '\0'; i++)
        write(file, &phonenumber[i], 1);
    write(file, " ", 1);

    printf(" Please, write your status (actif or inactif) : ");
    scanf("%s", status);
    for (int i = 0; status[i] != '\0'; i++)
        write(file, &status[i], 1);
    
    close(file);
    return 0;
}

int check_word(char buff[], int s_buff, char word[], int s_word) {
  int i = 0;
  while (i < s_word) {
    if (buff[i] != word[i]) {
      return 0;
    }
    i++;
  }
  return 1;
}

int word_in_line(char line[], int line_i, char word[], int word_i) {
  int i = 0;
  int j = 0;
  char current[256];
  while (i < line_i) {
    if (j == 0) {

      while (j < sizeof(current)) {
        current[j] = 0;
        j++;
      }
      j = 0;
    }
    current[j] = line[i];

    if (current[j] == ' ' || current[j] == '\0') {
      current[j] = '\0';
      j = -1;
    }
    if (j == -1 && check_word(current, sizeof(current), word, word_i)) {
      printf("line :\n %s \n", line);
    }
    j++;
    i++;
  }
  return 0;
}

int main() {
    int choice;
    while (1) {   // boucle infinie
        printf("\n===== MENU =====\n");
        printf("1. Display \n");
        printf("2. Search \n");
        printf("3. Delete \n");
        printf("4. Add \n");
        printf("0. Leave \n");
        printf("Your choice : ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
                print_BD(open("tp5_base_de_donné.txt", O_RDONLY));
                break;
            case 2:
                break;
            case 3: {
                supression(1, (const char *[]){"tp5_base_de_donné.txt"});
                break;
            }
            case 4: {
                add(1, (const char *[]){"tp5_base_de_donné.txt"});
                break;
            }
            case 0:
                printf("This is the end\n");
                return 0;
            default:
                printf("Invalid choice .\n");
                break;
        }
    }
}
