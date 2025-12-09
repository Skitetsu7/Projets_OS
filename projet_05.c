#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int add(int argc,char const *argv[])
{
    
    int file = open(argv[1], O_WRONLY | O_APPEND);
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
                char name[20];
                char firstname[20];
                char email[50];
                char phonenumber[20];
                char status[10];
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
