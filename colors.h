#pragma once

// Text Font Color
#define normal "\033[0m"
#define black  "\033[38;5;0m"
#define Red    "\033[38;5;1m"
#define Green  "\033[38;5;2m"
#define Yellow "\033[38;5;3m"
#define Blue   "\033[38;5;4m"
#define Purple "\033[38;5;5m"
#define Cyan   "\033[38;5;6m"
#define grey   "\033[38;5;7m"
#define Grey   "\033[38;5;8m"
#define red    "\033[38;5;9m"
#define green  "\033[38;5;10m"
#define yellow "\033[38;5;11m"
#define blue   "\033[38;5;12m"
#define purple "\033[38;5;13m"
#define cyan   "\033[38;5;14m"

// BackGround Color
#define Bblack  "\033[48;5;0m"
#define BRed    "\033[48;5;1m"
#define BGreen  "\033[48;5;2m"
#define BYellow "\033[48;5;3m"
#define BBlue   "\033[48;5;4m"
#define BPurple "\033[48;5;5m"
#define BCyan   "\033[48;5;6m"
#define Bgrey   "\033[48;5;7m"
#define BGrey   "\033[48;5;8m"
#define Bred    "\033[48;5;9m"
#define Bgreen  "\033[48;5;10m"
#define Byellow "\033[48;5;11m"
#define Bblue   "\033[48;5;12m"
#define Bpurple "\033[48;5;13m"
#define Bcyan   "\033[48;5;14m"

/*
Use it like this
printf("%s%sHello %s%s World %s \n",
  red,Bcyan,
  black,Bred,
  normal);
*/
