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

char *set_name_window(const int rows, const int cols) {
    const int name_w = cols / 2;
    const int name_h = rows / 2;
    const int name_x = (cols - name_w) / 2;
    const int name_y = (rows - name_h) * 2 / 3;

    int text_box_x = 10;
    int text_box_y = name_h * 2 / 3;

    char *player_name;

    /* Create a new window requesting the user name */
    WINDOW *name = newwin(name_h, name_w, name_y, name_x);
    configure_window(name, name_h, name_w);

    mvwprintw(name, name_h / 3, name_w / 2 - 9, "Insert name here:");

    /* Print the text box */
    wattron(name, COLOR_PAIR(BLACK_BLACK));
    for (int i = text_box_x; i <= name_w - 10; i++)
        mvwprintw(name, text_box_y, i, " ");

    player_name = insert_name(name, text_box_x, text_box_y, cols);

    werase(name);
    wrefresh(name);
    delwin(name);

    return player_name;
}

char *insert_name(WINDOW *name, int text_box_x, int text_box_y, const int cols) {
    char *player_name = malloc(40 * sizeof(char));
    int character = 0;
    int index_char = 0;

    player_name[0] = '\0';

    /* Receive characters and add them to the name string until the Enter key is pressed */
    while (character != 10) {
        /* Display the cursor */
        wattron(name, COLOR_PAIR(RED_RED));
        mvwprintw(name, text_box_y, text_box_x, " ");

        /* Receive a character from the keyboard */
        character = wgetch(name);

        /* Display the character */
        wattron(name, COLOR_PAIR(RED_BLACK));
        mvwprintw(name, text_box_y, text_box_x, "%c", character);

        /* Add character to the string */
        player_name[index_char] = (char) character;
        index_char++;
        player_name[index_char] = '\0';

        /* Print message if the character limit is exceeded */
        if (text_box_x == 50) {
            const char *limit = "Character limit exceeded!";

            wattron(name, COLOR_PAIR(RED_BLUE));
            mvwprintw(name, text_box_y + 1, (cols - (int) strlen(limit)) / 2, limit);
            break;
        }

        /* If the Backspace key is pressed, move the cursor backwards and decrement the string index */
        if (character == 127) {
            text_box_x--;

            wattron(name, COLOR_PAIR(RED_RED));
            mvwprintw(name, text_box_y, text_box_x, " ");

            wattron(name, COLOR_PAIR(BLACK_BLACK));
            mvwprintw(name, text_box_y, text_box_x + 1, "  ");

            index_char -= 2;
        } else {
            text_box_x++;
        }
    }

    return player_name;
}

