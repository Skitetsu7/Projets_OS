#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

void randomchar(char *out, int size){
    int len = size - 1;
    for(int i = 0; i < len; i++){
        out[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    out[len] = '\0';
}


void randomasciinumber(char *out, int size){
    int i = 0;

    while(i< size){
        out[i] = rand()%10+48;
        i++;
    }
    out[size-1]=0;
}

int mix_chr(char *receiver, char *sended, int start){
  int i = 0;
  while(sended[i]){
    receiver[start] = sended[i];
    start++;
    i++;
  }
  receiver[start++] = ' ';
  return start;
}

int main(){
    srand(time(NULL));
    int i = 0;
    while(i<5000){
    
    
    char name[256];
    randomchar(name,sizeof(name));

    char firstname[256];
    randomchar(firstname,sizeof(firstname)); 

    char phonenumber[11];
    randomasciinumber(phonenumber,sizeof(phonenumber));       

    char email[256];
    randomchar(email,sizeof(email));
    email[rand()%255] = '@';
    
    char status[7];
    randomchar(status,sizeof(status));

    char full[256*3+18]; //receiver
    full[0]=0;
    int start = 0;
    start = mix_chr(full,name,start);
    start = mix_chr(full,firstname,start);
    start = mix_chr(full,phonenumber,start);
    start = mix_chr(full,email,start);
    start = mix_chr(full,status,start);


    full[start-2] = '\n';
    full[start-1] = 0;
    //printf("%s\n",full);
    int fd =open("tp5_base_de_données.txt",O_WRONLY | O_APPEND);
    write(fd,full,strlen(full));
    close(fd);
    i++;
    }
}
