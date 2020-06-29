#include "headers/game.h"

/**
 * Create the game window, containing the question and its answers; Perform the necessary actions to handle each user
 * input for every question; At each step, store the game state to a Game object;
 *
 * @return      a flag, representing the exit action chosen by the user;
 */
int start_new_game(Game *game, Question *questions, const int questions_number, const char *score_filename, const int
rows, const int cols) {
    const int game_w = cols - 20;
    const int game_h = rows / 2;
    const int game_x = 10;
    const int game_y = (rows - game_h) * 2 / 3;

    /* Create a new window */
    WINDOW *game_window = newwin(game_h, game_w, game_y, game_x);

    /* Show the helping options window */
    WINDOW *help_window = display_helping_options(game, game_h, game_y, cols);

    char hidden_answers[] = {'x', 'x'};

    /* Print each question and its answers, then check the input received from the user */
    for (int i = game->question_index; i < questions_number; ++i) {
        wclear(game_window);
        wrefresh(game_window);
        configure_window(game_window, game_h, game_w);

        game->question_index = i;

        /* Show the time and score panels */
        display_score_panel(game->score, game->right_answers, game->wrong_answers, rows, cols);
        display_time_window(game->player_name, rows, cols);

        /* Display the current question and its answers */
        display_question(game_window, questions[i].question, i + 1, game_h, game_w);
        display_all_answers(game_window, questions[i], game_w, game_h);

        /* Get the return flag of the answer function */
        int chosen_answer = choose_answer(game_window, help_window, questions[i], game, hidden_answers, game_w, game_h);

        switch (chosen_answer) {
            case RIGHT_ANSWER:
                /* If the answer was right, increase the score and the number of correct answers */
                game->score += 10;
                game->right_answers++;
                break;

            case WRONG_ANSWER:
                game->score -= 5;
                game->wrong_answers++;
                break;

            case SKIP_QUESTION:
                /* If the skip question option is used, go to the next question */
                continue;

            case QUIT_TO_MENU:
                /* If the 'q' key is pressed, clear the game window and go back to the menu by returning the right
                 * flag  */
                clear_game(game_window, help_window);
                return QUIT_TO_MENU;

            default:
                break;
        }

        /* Wait 1 second to display the next question */
        sleep(1);
    }

    /* After finishing the set of questions, clear the game window */
    clear_game(game_window, help_window);

    /* Refresh the two panels */
    display_score_panel(game->score, game->right_answers, game->wrong_answers, rows, cols);
    display_time_window(game->player_name, rows, cols);

    /* Add the new score entry to the score file */
    add_to_scoreboard(score_filename, game->player_name, game->score);

    /* Show the end menu and return the ending option chosen */
    return display_end_menu(game, rows, cols);
}

/**
 * Get input characters from the user and return the corresponding flags meant to change the game state;
 *
 * @param game_window                  the game window;
 * @param help_window               the helping options window;
 * @param question                  the current question;
 * @param game                      the struct storing the game state;
 * @param hidden_answers            the array of characters containing the answers hidden after 50-50;
 * @return                          a flag in the array {WRONG_ANSWER, RIGHT_ANSWER, SKIP_QUESTION};
 */
int choose_answer(WINDOW *game_window, WINDOW *help_window, const Question question, Game *game, char *hidden_answers,
                  const int game_w, const int game_h) {

    char chosen_answer = (char) wgetch(game_window);

    switch (chosen_answer) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
            /* If the user chose one of the valid answer options (not hidden!), highlight the answer in yellow;
             * Wait a second, then check if the answer is right and highlight the chosen answer accordingly;
             * Return the corresponding flag */
            if (chosen_answer != hidden_answers[0] && chosen_answer != hidden_answers[1]) {
                highlight_answer(game_window, question, chosen_answer, game_w, game_h, YELLOW_YELLOW, BLUE_YELLOW);
                sleep(1);

                /* Highlight the correct answer in green */
                highlight_answer(game_window, question, question.correct_answer, game_w, game_h, GREEN_GREEN,
                                 BLUE_GREEN);

                /* If the chosen answer was wrong, highlight it in magenta */
                if (chosen_answer != question.correct_answer) {
                    highlight_answer(game_window, question, chosen_answer, game_w, game_h, MAGENTA_MAGENTA,
                                     BLUE_MAGENTA);
                    return WRONG_ANSWER;
                }

                return RIGHT_ANSWER;
            }

            /* If the answer was hidden display the invalid prompt and wait for another result */
            goto invalid;

        case 'e': /* 50-50 */
            /* If the user chose the 50-50 option for the first time, hide it and set it as used */
            if (!game->used_5050) {
                game->used_5050 = TRUE;

                display_answer(help_window, "50-50", 'E', BLUE_BLUE, BLUE_BLUE, 0, 0);
                wrefresh(help_window);

                /* Call a function that returns an array of two random chars, representing the answers that should be
                 * hidden */
                hidden_answers = get_hidden_answers(question.correct_answer);
                highlight_answer(game_window, question, hidden_answers[0], game_w, game_h, BLUE_BLUE, BLUE_BLUE);
                highlight_answer(game_window, question, hidden_answers[1], game_w, game_h, BLUE_BLUE, BLUE_BLUE);

                /* Call the function again recursively until provided with a valid answer */
                return choose_answer(game_window, help_window, question, game, hidden_answers, game_w, game_h);
            }

            goto invalid;

        case 'f': /* Skip the question */
            /* If the skip question option is used for the first time, hide it and set it as used */
            if (!game->used_skip) {
                game->used_skip = TRUE;

                display_answer(help_window, "Skip the question", 'F', BLUE_BLUE, BLUE_BLUE, 51, 0);
                wrefresh(help_window);

                return SKIP_QUESTION;
            }

            goto invalid;

        case 'q':
            return QUIT_TO_MENU;

        invalid:
        default:
            /* If the input provided in invalid, print a prompt and call the function recursively until the user
             * inserts a valid character */
            wattron(game_window, COLOR_PAIR(RED_BLUE));
            mvwprintw(game_window, game_h / 2 + 8, game_w / 2 - 10, "Pick a valid answer!");
            wrefresh(game_window);
            return choose_answer(game_window, help_window, question, game, hidden_answers, game_w, game_h);
    }
}

/**
 * Create a new window, asking for the name of the player;
 *
 * @return          the player name;
 */
char *set_name_window(const int rows, const int cols) {
    const int name_w = cols / 2;
    const int name_h = rows / 2;
    const int name_x = (cols - name_w) / 2;
    const int name_y = (rows - name_h) * 2 / 3;

    int text_box_x = 10;
    int text_box_y = name_h * 2 / 3;

    char *player_name;

    /* Create a new window requesting the user name */
    WINDOW *name_window = newwin(name_h, name_w, name_y, name_x);
    configure_window(name_window, name_h, name_w);

    mvwprintw(name_window, name_h / 3, name_w / 2 - 9, "Insert name here:");

    /* Print the text box */
    wattron(name_window, COLOR_PAIR(BLACK_BLACK));
    for (int i = text_box_x; i <= name_w - 10; i++)
        mvwprintw(name_window, text_box_y, i, " ");

    player_name = insert_name(name_window, text_box_x, text_box_y, cols);

    werase(name_window);
    wrefresh(name_window);
    delwin(name_window);

    return player_name;
}

/**
 * Handle the text insertion;
 *
 * @return          the inserted player name;
 */
char *insert_name(WINDOW *name_window, int text_box_x, int text_box_y, const int cols) {
    char *player_name = malloc(40 * sizeof(char));
    int character = 0;
    int index_char = 0;

    player_name[0] = '\0';

    /* Receive characters and add them to the name string until the Enter key is pressed */
    while (character != 10) {
        /* Display the cursor */
        wattron(name_window, COLOR_PAIR(RED_RED));
        mvwprintw(name_window, text_box_y, text_box_x, " ");

        /* Receive a character from the keyboard */
        character = wgetch(name_window);

        /* If the Backspace key is pressed, move the cursor backwards and decrement the string index */
        if (character == 127) {
            text_box_x--;

            wattron(name_window, COLOR_PAIR(RED_RED));
            mvwprintw(name_window, text_box_y, text_box_x, " ");

            wattron(name_window, COLOR_PAIR(BLACK_BLACK));
            mvwprintw(name_window, text_box_y, text_box_x, "  ");
            wrefresh(name_window);

            index_char -= 2;
            continue;
        }

        /* Display the character */
        wattron(name_window, COLOR_PAIR(RED_BLACK));
        mvwprintw(name_window, text_box_y, text_box_x, "%c", character);

        /* Add character to the string */
        player_name[index_char] = (char) character;
        index_char++;
        player_name[index_char] = '\0';

        /* Print message if the character limit is exceeded */
        if (text_box_x == 50) {
            const char *limit = "Character limit exceeded!";

            wattron(name_window, COLOR_PAIR(RED_BLUE));
            mvwprintw(name_window, text_box_y + 1, (cols - (int) strlen(limit)) / 2, limit);
            break;
        }

        text_box_x++;
    }

    return player_name;
}

/**
 * Get the current time and display it in the time window
 */
void display_current_time(WINDOW *time_window) {
    struct tm *area;
    time_t t;

    t = time(NULL);
    area = localtime(&t);

    mvwprintw(time_window, 2, 1, "%s", asctime(area));
}

/**
 * Create a new window containing the current time and player name;
 */
void display_time_window(const char *player_name, const int rows, const int cols) {
    const int time_h = 7;
    const int time_w = 26;
    const int time_x = cols / 4 - time_w / 2 - 15;
    const int time_y = rows / 6 - 3;

    /* Create a new window */
    WINDOW *time_window = newwin(time_h, time_w, time_y, time_x);
    configure_window(time_window, time_h, time_w);

    /* Display the time and the player name */
    display_current_time(time_window);
    mvwprintw(time_window, 4, 1, "Player: %s", player_name);
    wrefresh(time_window);
}

/**
 * Display a panel containing the score and the number of wrong and right answers;
 */
void display_score_panel(const int score, const int right_answers, const int wrong_answers, const int rows, const int
cols) {
    const int score_h = 7;
    const int score_w = 26;
    const int score_x = cols * 3 / 4 + score_w / 2 - 11;
    const int score_y = rows / 6 - 3;

    WINDOW *score_window = newwin(score_h, score_w, score_y, score_x);
    configure_window(score_window, score_h, score_w);

    mvwprintw(score_window, 2, 2, "Score: %dp", score);
    mvwprintw(score_window, 3, 2, "Correct answers: %d", right_answers);
    mvwprintw(score_window, 4, 2, "Wrong answers: %d", wrong_answers);
    wrefresh(score_window);
}

/**
 * Show the "50-50" and "Skip question" helping options;
 */
WINDOW *display_helping_options(const Game *game, const int game_h, const int game_y, const int cols) {
    const int help_h = 3;
    const int help_w = 80;
    const int help_y = game_h + game_y + 1;
    const int help_x = (cols - help_w) / 2;

    /* Configure the window and erase its border */
    WINDOW *help_window = newwin(help_h, help_w, help_y, help_x);
    configure_window(help_window, help_h, help_w);
    werase(help_window);

    /* Show the two options */
    if (!game->used_5050) {
        display_answer(help_window, "50-50", 'E', RED_RED, BLUE_RED, 0, 0);
    }

    if (!game->used_skip) {
        display_answer(help_window, "Skip the question", 'F', RED_RED, BLUE_RED, 51, 0);
    }

    wrefresh(help_window);

    return help_window;
}

/**
 * Return an array of two random characters (not containing the correct answer), representing the answers hidden after
 * choosing the 50-50 option;
 */
char *get_hidden_answers(char correct_answer) {
    const char answers[] = {'a', 'b', 'c', 'd'};
    char *hidden_answers = malloc(2 * sizeof(char));
    int answer_left_index = rand() % 4;
    int j = 0;

    /* Generate a random integer between 0 - 3 (the other answer that will remain shown, besides the correct one),
     * until the number is different from the index of the correct answer */
    srand(time(0));
    while (answers[answer_left_index] == correct_answer) {
        answer_left_index = rand() % 4;
    }

    /* Add the remaining characters in the hidden array */
    for (int i = 0; i < 4; ++i) {
        if (answers[i] != correct_answer && answers[i] != answers[answer_left_index]) {
            hidden_answers[j] = answers[i];
            j++;
        }
    }

    return hidden_answers;
}

/**
 * Checks the character received as the chosen answer and highlight the corresponding box in the specified colors;
 */
void highlight_answer(WINDOW *game_window, const Question question, const char chosen_answer, const int game_w, const
int game_h, const short box_color, const short text_color) {
    const int left = game_w / 4 - 15;
    const int right = game_w / 4 - 15 + game_w / 2;
    const int top = game_h / 2;
    const int bottom = game_h / 2 + 4;

    switch (chosen_answer) {
        case 'a':
            display_answer(game_window, question.answer_a, 'A', box_color, text_color, left, top);
            break;

        case 'b':
            display_answer(game_window, question.answer_b, 'B', box_color, text_color, right, top);
            break;

        case 'c':
            display_answer(game_window, question.answer_c, 'C', box_color, text_color, left, bottom);
            break;

        case 'd':
            display_answer(game_window, question.answer_d, 'D', box_color, text_color, right, bottom);
            break;

        default:
            break;
    }
}

/**
 * Create a new window, displaying the final score and the options for continuing or exiting the game;
 *
 * @return          the exit choice;
 */
int display_end_menu(const Game *game, const int rows, const int cols) {
    const int end_w = cols * 2 / 3;
    const int end_h = rows / 2;
    const int end_x = (cols - end_w) / 2;
    const int end_y = (rows - end_h) * 2 / 3;

    /* Create a new window and show the score and number of answers */
    WINDOW *end_window = newwin(end_h, end_w, end_y, end_x);
    configure_window(end_window, end_h, end_w);

    mvwprintw(end_window, end_h * 1 / 5, (end_w - (int) strlen(game->player_name)) / 2 - 9, "Congratulations, %s!",
              game->player_name);
    mvwprintw(end_window, end_h * 1 / 3, end_w / 2 - 7, "Your score: %d", game->score);
    mvwprintw(end_window, end_h * 1 / 3 + 1, (end_w - 17) / 2, "Right answers: %d", game->right_answers);
    mvwprintw(end_window, end_h * 1 / 3 + 2, (end_w - 17) / 2, "Wrong answers: %d", game->wrong_answers);
    wrefresh(end_window);

    return choose_end_option(end_window, end_h, end_w);
}

/**
 * Handle the arrow keys actions for navigating through the options;
 */
int choose_end_option(WINDOW *end_window, const int end_h, const int end_w) {
    int current_option = 0, end_choice = -1;

    display_end_options(end_window, current_option, end_h, end_w);
    while (end_choice == -1) {

        switch (wgetch(end_window)) {
            case 10:
                end_choice = current_option;
                break;

            case KEY_RIGHT:
                if (current_option != QUIT_END) {
                    current_option++;
                } else {
                    current_option = SCOREBOARD_END;
                }
                break;

            case KEY_LEFT:
                if (current_option != SCOREBOARD_END) {
                    current_option--;
                } else {
                    current_option = QUIT_END;
                }
                break;

            default:
                mvwprintw(end_window, end_h * 3 / 4 - 2, end_w / 2 - 10, "Use the arrow keys!");
                wrefresh(end_window);
                break;
        }

        display_end_options(end_window, current_option, end_h, end_w);
    }

    return end_choice;
}

/**
 * Highlight the active option with the provided color parameters;
 */
void highlight_end_option(WINDOW *end_window, short highlight_color, short text_color, const char *option,
                          const int option_x, const int rows, const int cols) {
    const int option_y = rows * 3 / 4;
    const int text_x = cols / 8 - (int) strlen(option) / 2;

    /* Draw a frame of the highlighted option */
    wattron(end_window, COLOR_PAIR(highlight_color));
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < cols / 4; ++j) {
            mvwprintw(end_window, option_y + i, option_x + j, "-");
        }
    }

    /* Write the option text */
    wattron(end_window, COLOR_PAIR(text_color));
    mvwprintw(end_window, option_y + 1, option_x + text_x, "%s", option);
}

/**
 * Navigate between the options and highlight the active and inactive ones accordingly;
 */
void display_end_options(WINDOW *end_window, int current_option, int end_h, int end_w) {
    const char *options[] = {"Scoreboard", "Menu", "Quit"};

    int option_x = 2;

    for (int i = 0; i < 3; i++) {
        if (current_option == i) {
            highlight_end_option(end_window, GREEN_GREEN, BLUE_GREEN, options[i], option_x, end_h, end_w);
        } else {
            highlight_end_option(end_window, RED_RED, BLUE_RED, options[i], option_x, end_h, end_w);
        }

        option_x += (end_w * 3 / 8 - 2);
    }

    wrefresh(end_window);
}

/**
 * Clear the two game windows;
 */
void clear_game(WINDOW *game_window, WINDOW *help_window) {
    wclear(game_window);
    wrefresh(game_window);
    wclear(help_window);
    wrefresh(help_window);
}