#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define BUFFER_ACTIF_MEMBER 100
#define BUFFER_INACTIF_MEMBER 100
const char path[] = "tp5_base_de_données.txt";
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
            strcpy(actif_member[actif_count], line); // copy the line in actif  member
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
    return EXIT_FAILURE;
  }
  int bytes_read = read(file, buffer, BUFFER_SIZE);
  if (bytes_read < 0) {
    perror("read");
    close(file);
    return EXIT_FAILURE;
  }
  close(file);
  printf("provides a (full) email address to delete: ");
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
        return EXIT_FAILURE;
    }
    write(file, buffer, bytes_read);
    close(file);
    return 0;
}

void reset_buffer(char *buffer, int size) {
  for (int i = 0; i < size; i++)
    buffer[i] = 0;
}

int mix_char(char *receiver, char *sended, int start){
  int i = 0;
  while(sended[i]){
    receiver[start] = sended[i];
    start++;
    i++;
  }
  receiver[start++] = ' ';
  return start;
}

int valid_digit(char* word){
  int i = 0;
  while(word[i]){
    if ('0'<=word[i]&& word[i]<='9')
    {i++;}
    else{return 0;}
  }
  return i;
}

int len_str(char *buff) {
  int len = 0;
  while (buff[len] != 0) {
    len++;
  }
  return len;
}

int search_str(char *word, char *sentence, char separator, char line_end) {
  int i = 0;
  int j = 0;
  int state[3];
  int occurrence = 0;
  while (i < len_str(sentence) ) {
    //printf("sentence[%d] = %c == word[%d] %c\n",i,sentence[i],j,word[j]);
    if (sentence[i] == word[j]) {
      j++;
      
      if (j == len_str(word)) {
        if (separator =='*'){
          occurrence++;
          j=0;
        }
        else {
          state[0] = !(j - i - 1); // start
          state[1] = sentence[i + 1] == line_end ||
                    sentence[i + 1] == separator; // end or separator after
          state[2] = sentence[i - j] == separator; // separator before

          if ((state[0] && state[1])||(state[2]&&state[1])) {
            occurrence++;
          }
          state[0] = 0;
          state[1] = 0;
          state[2] = 0;
        }
      }
    } else {
      j = 0;
    }
    i++;
  }
  return occurrence;
}

int email_invalid(int fb,char *email){
  char content[BUFFER_SIZE];
  if(1!=search_str("@",email,'*',0)){
    return 1;
  };
  //printf("okay sur le nombre de @\n");
  int bytes_read = read(fb, content, BUFFER_SIZE);
  if (bytes_read < 0) {
        perror("read");
        close(fb);
        return EXIT_FAILURE;
  }
  content[bytes_read] = '\0'; // close the buffer

  if (search_str(email, content, ' ','\n')){
    return 1;
  }
  return 0;
}

void receive_data(char word_tab[5][1024],int fb){
  printf("Please, write your name : ");
  scanf("%s",word_tab[0]);

  printf("Please, write your firstname : ");
  scanf("%s",word_tab[1]);

  //a phone number = 10 digit
  while (strlen(word_tab[2])!=10 || !valid_digit(word_tab[2]) ){
    printf("Please, write your Phone number : ");
    scanf("%s",word_tab[2]);
  }
  // an email adress
  while (email_invalid(fb,word_tab[3])){
    printf("Please, write your email who don't already exist : ");
    scanf("%s",word_tab[3]);
  }

  while(!search_str(word_tab[4],"actif inactif\n",' ','\n')){
    printf("Please, write your status (actif or inactif) : ");
    scanf("%s",word_tab[4]);
  }
    
}

int add(const char file_path[]) {
  char buffer[BUFFER_SIZE];
  const int input = 5;
  const int max_len = 1024;
  
  char full[BUFFER_SIZE]; //receiver
  char usr_input[input][max_len]; //tab de tab
  
  int file = open(file_path, O_RDWR | O_APPEND);
  if (file == -1) {
    perror("Erreur d'ouverture du fichier");
    return -1;
  }

  receive_data(usr_input,file);
  
  int start_index =1;
  //tu loop pour concatainer petit à petit
  full[0] = '\n';
  for (int i = 0; i< input;i++){
    start_index = mix_char(full,usr_input[i],start_index);
  }
  full[len_str(full)] = '\n';

  write(file,full,len_str(full));
  close(file);
  reset_buffer(full,1024);
  return 0;
}
int take_input(char *usrin, char asked) {
  for (int i = 0; i < len_str(usrin); i++) {
    if (usrin[i] == asked) {
      return 1;
    }
  }
  return 0;
}

int init_search(const char file_path[]) {
  // init
  char word[256];
  char line[1024];
  char buff[1024];
  int i = 0;
  int j = 0;
  unsigned int bytes;
  int occurrence = 0;
  // ask useer
  printf("Research email :\n");
  reset_buffer(word, 256);

  // wait for a decent input
  while (!take_input(word, '@') && len_str(word) < 256) {
    printf("please enter an email (or @ to cancel):");
    scanf("%s", word);
  }
  int file = open(file_path, O_RDONLY);
  if (file == -1) {
    printf("ERROR : Opening file failed;\n");
    return EXIT_FAILURE;
  }
  /*
  Avoid buffer overflow read work a certain way, it fill our buffer
  with the file content but if it arrive at the end, he will not fill
  anymore our buffer. So  we have to do 2 while, the first one is 
  avoyding the buffer overflow becauses when we use read, read keep 
  an index to mark where it stopped. So when he read 1024 bytes, 
  it launch the second while where we run through the line and fill the line
  buffer until it reach the end of the line
  */
  while ((bytes = read(file, buff, sizeof(buff))) > 0)
  {
    while (i < bytes) {
        line[j] = buff[i];
        if (line[j] == '\n') {
          occurrence += search_str(word, line, ' ', ';');
          if(search_str(word, line, ' ', ';')){
            printf("%s",line);
          }
          reset_buffer(line, len_str(line));
          j = -1;
        }
        j++;
        i++;
      }
      i=0;
      printf("occurence :%d\n", occurrence);
  }
      close(file);
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
      printf("Invalid choice .");
      break;
    }
  }
}
