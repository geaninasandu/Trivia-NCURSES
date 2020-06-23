#include "game.h"

Game start_new_game(Game game, Question *questions, const int questions_number, const char *player_name, const int
rows, const int cols) {
    const int game_w = cols - 20;
    const int game_h = rows / 2;
    const int game_x = 10;
    const int game_y = (rows - game_h) * 2 / 3;

    /* Create a new window */
    WINDOW *new_game = newwin(game_h, game_w, game_y, game_x);

    /* Show the helping options window */
    WINDOW *help_window = display_helping_options(game_h, game_y, cols);

    /* Print each question and its answers, then  */
    for (int i = game.question_index; i < questions_number; ++i) {
        wclear(new_game);
        wrefresh(new_game);
        configure_window(new_game, game_h, game_w);

        game.question_index = i;

        /* Show the time and score panels */
        display_scoreboard(game.score, game.right_answers, game.wrong_answers, rows, cols);
        display_time_window(player_name, rows, cols);

        /* Display the current question and its answers */
        display_question(new_game, questions[i].question, i + 1, game_h, game_w);
        display_all_answers(new_game, questions[i], game_w, game_h);

        int chosen_answer = choose_answer(new_game, questions[i], game, game_w, game_h);

        switch (chosen_answer) {
            case RIGHT_ANSWER:
                game.score += 10;
                game.right_answers++;
                break;

            case WRONG_ANSWER:
                game.score -= 5;
                game.wrong_answers++;
                break;

            case SKIP_QUESTION:
                game.used_skip = TRUE;
                display_answer(help_window, "Skip the question", 'F', BLUE_BLUE, BLUE_BLUE, 51, 0);
                wrefresh(help_window);
                continue;

            case FIFTY_FIFTY:
                game.used_5050 = TRUE;
                break;

        }

        sleep(1);
    }
}

int choose_answer(WINDOW *new_game, const Question question, Game game, const int game_w, const int game_h) {
    char *hidden_answers = NULL;
    char chosen_answer = wgetch(new_game);

    switch (chosen_answer) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
            highlight_answer(new_game, question, chosen_answer, game_w, game_h, YELLOW_YELLOW, BLUE_YELLOW);
            sleep(1);

            highlight_answer(new_game, question, question.correct_answer, game_w, game_h, GREEN_GREEN, BLUE_GREEN);

            if (chosen_answer != question.correct_answer) {
                highlight_answer(new_game, question, chosen_answer, game_w, game_h, MAGENTA_MAGENTA, BLUE_MAGENTA);
                return WRONG_ANSWER;
            }

            return RIGHT_ANSWER;

        case 'e':
            hidden_answers = get_hidden_answers(question.correct_answer);
            highlight_answer(new_game, question, hidden_answers[0], game_w, game_h, BLUE_BLUE, BLUE_BLUE);
            highlight_answer(new_game, question, hidden_answers[1], game_w, game_h, BLUE_BLUE, BLUE_BLUE);
            return FIFTY_FIFTY;

        case 'f':
            if (!game.used_skip) {
                return SKIP_QUESTION;
            }

            print_invalid_message(new_game, game_h, game_w);
            return choose_answer(new_game, question, game, game_w, game_h);

        default:
            print_invalid_message(new_game, game_h, game_w);
            return choose_answer(new_game, question, game, game_w, game_h);
    }
}

void print_invalid_message(WINDOW *new_game, const int game_h, const int game_w) {
    wattron(new_game, COLOR_PAIR(RED_BLUE));
    mvwprintw(new_game, game_h / 2 + 8, game_w / 2 - 10, "Pick a valid answer!");
    wrefresh(new_game);
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

char *get_hidden_answers(char correct_answer) {
    const char answers[] = {'a', 'b', 'c', 'd'};
    char *hidden_answers = malloc(2 * sizeof(char));
    int answer_left_index = rand() % 4;
    int j = 0;

    srand(time(0));
    while (answers[answer_left_index] == correct_answer) {
        answer_left_index = rand() % 4;
    }

    for (int i = 0; i < 4; ++i) {
        if (answers[i] != correct_answer && answers[i] != answers[answer_left_index]) {
            hidden_answers[j] = answers[i];
            j++;
        }
    }

    return hidden_answers;
}

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
