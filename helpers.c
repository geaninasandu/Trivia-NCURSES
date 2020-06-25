#include "helpers.h"

void init_screen() {
    initscr();
    noecho();
    cbreak();
    init_colors();
}

/**
 * Configure the background of a sub-window
 */
void configure_window(WINDOW *window, const int rows, const int cols) {
    const int center = cols / 2 - 1;

    /* Set the background color to blue */
    wbkgd(window, COLOR_PAIR(BLUE_BLUE));

    /* Create custom game border */
    wattron(window, COLOR_PAIR(RED_BLUE));
    wborder(window, ' ', ' ', '-', '-', '-', '-', '<', '>');
    mvwprintw(window, 0, center, " o ");
    mvwprintw(window, rows - 1, center, " o ");
    mvwprintw(window, 0, 0, "<<");
    mvwprintw(window, 0, cols - 2, ">>");
    mvwprintw(window, rows - 1, cols - 2, ">>");
    mvwprintw(window, rows - 1, 0, "<<");

    /* Allow keypress actions */
    keypad(window, true);

    /* Hide cursor */
    curs_set(0);
    refresh();
}

/**
 * Configure the background layout of the game
 * @param background        the game screen
 */
void configure_background(WINDOW *background, const int rows, const int cols) {
    /* Set the background color to blue */
    wbkgd(background, COLOR_PAIR(BLUE_BLUE));

    /* Create game border */
    wattron(background, COLOR_PAIR(RED_BLUE));
    wborder(background, ' ', ' ', '=', '=', '=', '=', '=', '=');

    /* Allow keypress actions */
    keypad(background, true);

    /* Hide cursor */
    curs_set(0);

    /* Draw the game logo */
    draw_logo(background, rows, cols);
    refresh();
}

/**
 * Draw the logo and a watermark
 */
void draw_logo(WINDOW *background, const int rows, const int cols) {
    const int logo_x = cols / 2 - 32;
    const int logo_y = rows / 6 - 3;

    const char *copyright = "(C) Geanina-Maria Sandu";
    const char *logo[] = {
            " ___________  _______    __  ___      ___  __          __",
            "(*     _   *)/*      \\  |* \\|*  \\    /*  ||* \\        /**\\",
            " )__/  \\\\__/|:        | ||  |\\   \\  //  / ||  |      /    \\",
            "    \\\\_ /   |_____/   ) |:  | \\\\  \\/. ./  |:  |     /' /\\  \\",
            "    |.  |    //      /  |.  |  \\.    //   |.  |    //  __'  \\",
            "    \\:  |   |:  __   \\  /\\  |\\  \\\\   /    /\\  |\\  /   /  \\\\  \\",
            "     \\__|   |__|  \\___)(__\\_|_)  \\__/    (__\\_|_)(___/    \\___)",
    };

    wattron(background, COLOR_PAIR(RED_BLUE));

    /* Display each line of the logo */
    for (int i = 0; i < 7; i++) {
        mvwprintw(background, logo_y + i, logo_x, logo[i]);
    }

    /* Display watermark */
    mvwprintw(background, rows - 2, cols - (int) strlen(copyright), copyright);
}

/**
 * Convert the RGB color value to ncurses supported format
 */
short rgb(int color_value) {
    const double rgb = 1000.0 / 255;

    return (short) (rgb * color_value);
}

/**
 * Initialize a custom color with the specified RGB values
 */
void init_new_color(short COLOR, int red, int green, int blue) {
    init_color(COLOR, rgb(red), rgb(green), rgb(blue));
}

/**
 * Enable coloring and initialize the color pairs
 */
void init_colors() {
    start_color();

    init_new_color(COLOR_BLUE, 21, 169, 225);
    init_new_color(COLOR_RED, 232, 49, 136);
    init_new_color(COLOR_YELLOW, 224, 214, 43);
    init_new_color(COLOR_MAGENTA, 224, 75, 31);
    init_new_color(COLOR_GREEN, 118, 224, 9);

    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(6, COLOR_BLACK, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLUE);
    init_pair(8, COLOR_BLUE, COLOR_RED);
    init_pair(9, COLOR_RED, COLOR_BLACK);
    init_pair(10, COLOR_BLUE, COLOR_GREEN);
    init_pair(11, COLOR_BLUE, COLOR_YELLOW);
    init_pair(12, COLOR_BLUE, COLOR_MAGENTA);

    refresh();
}

/**
 * Int values swapping
 */
void swap(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}

