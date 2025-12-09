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
