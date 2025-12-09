
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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
            if (strstr(line, "actif")){  //if actif member
                strcpy(actif_member[actif_count], line);  //copy the line in actif member
                actif_count++;   //increase the number of actif
            }
            else if (strstr(line, "inactif")){  //if inactif member
                strcpy(inactif_member[inactif_count], line);  //copy the line in inactif member
                inactif_count++;  //increasse the number of inactif
            }
            index = 0; //reser index for the next line
        }
        else {
            line[index] = ch; //add the char in line
        }
    }
    printf("==============actif member=============\n");   //print actif member
    for(int i = 0; i< actif_count; i++){
        printf("%s\n", actif_member[i]);
    }
    printf("==============inactif member=============\n");  //print inactif member
    for(int i = 0; i<inactif_count; i++){
        printf("%s\n", inactif_member[i]);
    }
}




int add(int argc,char const *argv[])
{
    
    int file = open(argv[1], O_WRONLY | O_APPEND);
    if (file == -1) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }
    char name[20];
    char firstname[20];
    char email[50];
    char phonenumber[20];
    char status[10];

    

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

int main() {
    int choice;
    while (1) {   // boucle infinie
        printf("\n===== MENU =====\n");
        printf("1. Display \n");
        printf("2. Search \n");
        printf("3. Delete \n");
        printf("4. Add \n");
        printf("0. Leave \n");
        printf("Your choice  : ");
        scanf("%d", &choix);

        switch (choice) {
            case 1:
                break;
            case 2:
                break;
            case 3: {
                break;
            }

            case 4: {
                
                add(argc,argv);
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
