#ifndef MENU_H
#define MENU_H

#include "helpers.h"

#define NEW_GAME 0
#define RESUME_GAME 1
#define SCOREBOARD 2
#define QUIT_GAME 3


int display_menu(int rows, int cols);

int choose_option(WINDOW *menu, int menu_h, int menu_w);

void display_options(WINDOW *menu, int current_option, int rows, int cols);


#endif //MENU_H
