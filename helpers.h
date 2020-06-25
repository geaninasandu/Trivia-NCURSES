#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define FALSE 0
#define TRUE 1
#define GREEN_GREEN 1
#define RED_RED 2
#define MAGENTA_MAGENTA 3
#define BLUE_BLUE 4
#define YELLOW_YELLOW 5
#define BLACK_BLACK 6
#define RED_BLUE 7
#define BLUE_RED 8
#define RED_BLACK 9
#define BLUE_GREEN 10
#define BLUE_YELLOW 11
#define BLUE_MAGENTA 12

void init_screen();

void configure_background(WINDOW *background, int rows, int cols);

void configure_window(WINDOW *window, int rows, int cols);

void draw_logo(WINDOW *background, int rows, int cols);

short rgb(int color_value);

void init_new_color(short COLOR, int red, int green, int blue);

void init_colors();

void swap(int *x, int *y);

#endif //HELPERS_H
