#include "game.h"

Game *start_new_game(Game *game, Question *questions, const int questions_number, const char *player_name, const int
rows, const int cols) {
    const int game_w = cols - 20;
    const int game_h = rows / 2;
    const int game_x = 10;
    const int game_y = (rows - game_h) * 2 / 3;

    /* Create a new window */
    WINDOW *new_game = newwin(game_h, game_w, game_y, game_x);

    /* Show the helping options window */
    WINDOW *help_window = display_helping_options(game_h, game_y, cols);

    char hidden_answers[] = {'x', 'x'};

    /* Print each question and its answers, then check the input received from the user */
    for (int i = game->question_index; i < questions_number; ++i) {
        wclear(new_game);
        wrefresh(new_game);
        configure_window(new_game, game_h, game_w);

        game->question_index = i;

        /* Show the time and score panels */
        display_scoreboard(game->score, game->right_answers, game->wrong_answers, rows, cols);
        display_time_window(player_name, rows, cols);

        /* Display the current question and its answers */
        display_question(new_game, questions[i].question, i + 1, game_h, game_w);
        display_all_answers(new_game, questions[i], game_w, game_h);

        /* Get the return flag of the answer function */
        int chosen_answer = choose_answer(new_game, help_window, questions[i], game, hidden_answers, game_w, game_h);

        switch (chosen_answer) {
            case RIGHT_ANSWER:
                game->score += 10;
                game->right_answers++;
                break;

            case WRONG_ANSWER:
                game->score -= 5;
                game->wrong_answers++;
                break;

            case SKIP_QUESTION:
                continue;

            case QUIT:
                return game;

            default:
                break;
        }

        sleep(1);
    }

    return game;
}

/**
 * Get input characters from the user and return the corresponding flags meant to change the game state;
 *
 * @param new_game                  the game window;
 * @param help_window               the helping options window;
 * @param question                  the current question;
 * @param game                      the struct storing the game state;
 * @param hidden_answers            the array of characters containing the answers hidden after 50-50;
 * @return                          a flag in the array {WRONG_ANSWER, RIGHT_ANSWER, SKIP_QUESTION};
 */
int choose_answer(WINDOW *new_game, WINDOW *help_window, const Question question, Game *game, char *hidden_answers,
        const int game_w, const int game_h) {

    char chosen_answer = (char) wgetch(new_game);

    switch (chosen_answer) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
            /* If the user chose one of the valid answer options (not hidden!), highlight the answer in yellow;
             * Wait a second, then check if the answer is right and highlight the chosen answer accordingly;
             * Return the corresponding flag */
            if (chosen_answer != hidden_answers[0] && chosen_answer != hidden_answers[1]) {
                highlight_answer(new_game, question, chosen_answer, game_w, game_h, YELLOW_YELLOW, BLUE_YELLOW);
                sleep(1);

                /* Highlight the correct answer in green */
                highlight_answer(new_game, question, question.correct_answer, game_w, game_h, GREEN_GREEN, BLUE_GREEN);

                /* If the chosen answer was wrong, highlight it in magenta */
                if (chosen_answer != question.correct_answer) {
                    highlight_answer(new_game, question, chosen_answer, game_w, game_h, MAGENTA_MAGENTA, BLUE_MAGENTA);
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
                highlight_answer(new_game, question, hidden_answers[0], game_w, game_h, BLUE_BLUE, BLUE_BLUE);
                highlight_answer(new_game, question, hidden_answers[1], game_w, game_h, BLUE_BLUE, BLUE_BLUE);

                /* Call the function again recursively until provided with a valid answer */
                return choose_answer(new_game, help_window, question, game, hidden_answers, game_w, game_h);
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
            return QUIT;

        invalid:
        default:
            /* If the input provided in invalid, print a prompt and call the function recursively until the user
             * inserts a valid character */
            wattron(new_game, COLOR_PAIR(RED_BLUE));
            mvwprintw(new_game, game_h / 2 + 8, game_w / 2 - 10, "Pick a valid answer!");
            wrefresh(new_game);
            return choose_answer(new_game, help_window, question, game, hidden_answers, game_w, game_h);
    }
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
void display_scoreboard(const int score, const int right_answers, const int wrong_answers, const int rows, const int
cols) {
    const int score_h = 7;
    const int score_w = 26;
    const int score_x = cols * 3 / 4 + score_w / 2 - 11;
    const int score_y = rows / 6 - 3;

    WINDOW *score_win = newwin(score_h, score_w, score_y, score_x);
    configure_window(score_win, score_h, score_w);

    mvwprintw(score_win, 2, 2, "Score: %dp", score);
    mvwprintw(score_win, 3, 2, "Correct answers: %d", right_answers);
    mvwprintw(score_win, 4, 2, "Wrong answers: %d", wrong_answers);
    wrefresh(score_win);
}

/**
 * Show the "50-50" and "Skip question" helping options;
 */
WINDOW *display_helping_options(const int game_h, const int game_y, const int cols) {
    const int help_h = 3;
    const int help_w = 80;
    const int help_y = game_h + game_y + 1;
    const int help_x = (cols - help_w) / 2;

    /* Configure the window and erase its border */
    WINDOW *help_window = newwin(help_h, help_w, help_y, help_x);
    configure_window(help_window, help_h, help_w);
    werase(help_window);

    /* Show the two options */
    display_answer(help_window, "50-50", 'E', RED_RED, BLUE_RED, 0, 0);
    display_answer(help_window, "Skip the question", 'F', RED_RED, BLUE_RED, 51, 0);
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
void highlight_answer(WINDOW *new_game, const Question question, const char chosen_answer, const int game_w, const int
game_h, const short box_color, const short text_color) {
    const int left = game_w / 4 - 15;
    const int right = game_w / 4 - 15 + game_w / 2;
    const int top = game_h / 2;
    const int bottom = game_h / 2 + 4;

    switch (chosen_answer) {
        case 'a':
            display_answer(new_game, question.answer_a, 'A', box_color, text_color, left, top);
            break;

        case 'b':
            display_answer(new_game, question.answer_b, 'B', box_color, text_color, right, top);
            break;

        case 'c':
            display_answer(new_game, question.answer_c, 'C', box_color, text_color, left, bottom);
            break;

        case 'd':
            display_answer(new_game, question.answer_d, 'D', box_color, text_color, right, bottom);
            break;

        default:
            break;
    }
}
