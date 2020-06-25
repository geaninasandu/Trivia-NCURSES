#include "menu.h"
#include "helpers.h"

/**
 * Create the menu window and return the chosen option
 *
 * @return      index of the selected option
 */
int display_menu(const int rows, const int cols) {
    const int menu_w = cols / 2;
    const int menu_h = rows / 2;
    const int menu_x = (cols - menu_w) / 2;
    const int menu_y = (rows - menu_h) * 2 / 3;

    /* Create a new window for the menu */
    WINDOW *menu = newwin(menu_h, menu_w, menu_y, menu_x);

    /* Configure menu background */
    configure_window(menu, menu_h, menu_w);
    wrefresh(menu);

    /* Return the chosen option */
    return choose_option(menu, menu_h, menu_w);
}

/**
 * Navigate between the menu options and return the selected one
 *
 * @return      the chosen option
 */
int choose_option(WINDOW *menu, int menu_h, int menu_w) {
    const char *instructions = "Use the arrow keys!";
    int current_option = 0, choice = -1;

    /* The program receives keyboard input until the enter key is pressed (while choice hasn't changed its -1 value) */
    while (choice == -1) {
        /* Display the options */
        display_options(menu, current_option, menu_h, menu_w);

        switch (wgetch(menu)) {
            /* If the Enter key was pressed, set the value of the choice to the index of the chosen option */
            case 10:
                choice = current_option;
                break;

            /* If the Down key was pressed, go to the next option, or to the first one if the selected option was the
             * last one */
            case KEY_DOWN:
                if (current_option != QUIT_GAME) {
                    current_option++;
                } else {
                    current_option = NEW_GAME;
                }
                break;

            case KEY_UP:
                if (current_option != NEW_GAME) {
                    current_option--;
                } else {
                    current_option = QUIT_GAME;
                }
                break;

            /* If any other key is pressed, display an instruction prompt */
            default:
                mvwprintw(menu, menu_h / 4 - 2, (menu_w - (int) strlen(instructions)) / 2, instructions);
                break;
        }
    }

    /* Return the chosen option index */
    return choice;
}

/**
 * Apply or remove highlighting of an option, depending on the chosen colors
 *
 * @param highlight_color       the color to be applied for the highlighting frame
 * @param text_color            the color of the option text
 * @param option                the option text
 * @param option_y              the y coordinate of the option frame
 */
void highlight_option(WINDOW *menu, short highlight_color, short text_color, const char *option, const int option_y,
                      const int cols) {
    const int option_x = (cols - (int) strlen(option)) / 2;

    /* Draw a frame of the highlighted option */
    wattron(menu, COLOR_PAIR(highlight_color));
    for (int i = 0; i < 3; ++i) {
        for (int j = 6; j < cols - 5; ++j) {
            mvwprintw(menu, option_y + i, j, "-");
        }
    }

    /* Write the option text */
    wattron(menu, COLOR_PAIR(text_color));
    mvwprintw(menu, option_y + 1, option_x, "%s", option);
}

/**
 * Display the menu options and highlight the selected one
 *
 * @param current_option    the index of the hovered option
 */
void display_options(WINDOW *menu, const int current_option, const int rows, const int cols) {
    const char *options[] = {"New Game", "Resume Game", "Leaderboard", "Quit"};
    int option_y = rows / 2 - 5;

    /* Get the index of the current hovered option and highlight it */
    /* Remove highlight after hovering another option */
    for (int i = 0; i < 4; i++) {
        if (current_option == i) {
            highlight_option(menu, RED_RED, BLUE_RED, options[i], option_y, cols);
        } else {
            highlight_option(menu, BLUE_BLUE, RED_BLUE, options[i], option_y, cols);
        }
        option_y += 3;
    }

    wrefresh(menu);
}
