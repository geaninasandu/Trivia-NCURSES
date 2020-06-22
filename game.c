#include "game.h"

int start_new_game(Question *questions, const int questions_number, const char *player_name, const int rows, const
int cols) {
    const int game_w = cols - 20;
    const int game_h = rows / 2;
    const int game_x = 10;
    const int game_y = (rows - game_h) * 2 / 3;

    int score = 0, right_answers = 0, wrong_answers = 0;

    WINDOW *new_game = newwin(game_h, game_w, game_y, game_x);

    /* Print each question and  */
    for (int i = 0; i < questions_number; ++i) {
        configure_window(new_game, game_h, game_w);

        /* Show the time and score panels */
        display_scoreboard(score, right_answers, wrong_answers, rows, cols);
        display_time_window(player_name, rows, cols);

        display_question(new_game, questions[i].question, i + 1, game_h, game_w);
        display_all_answers(new_game, questions[i], game_w, game_h);

        if (choose_answer(new_game, questions[i], game_w, game_h)) {
            score += 10;
            right_answers++;
        } else {
            score -= 5;
            wrong_answers++;
        }

        sleep(1);

        wclear(new_game);
        wrefresh(new_game);
    }
}

int choose_answer(WINDOW *new_game, const Question question, const int game_w, const int game_h) {
    const int left = game_w / 4 - 15;
    const int right = game_w / 4 - 15 + game_w / 2;
    const int top = game_h / 2;
    const int bottom = game_h / 2 + 4;

    char chosen_answer = wgetch(new_game);

    switch (chosen_answer) {
        case 'a':
            display_answer(new_game, question.answer_a, 'A', YELLOW_YELLOW, BLUE_YELLOW, left, top);
            sleep(1);

            if (chosen_answer == question.correct_answer) {
                display_answer(new_game, question.answer_a, 'A', GREEN_GREEN, BLUE_GREEN, left, top);
                return 1;
            } else {
                display_answer(new_game, question.answer_a, 'A', MAGENTA_MAGENTA, BLUE_MAGENTA, left, top);
                return 0;
            }

        case 'b':
            display_answer(new_game, question.answer_b, 'B', YELLOW_YELLOW, BLUE_YELLOW, right, top);
            sleep(1);

            if (chosen_answer == question.correct_answer) {
                display_answer(new_game, question.answer_b, 'B', GREEN_GREEN, BLUE_GREEN, right, top);
                return 1;
            } else {
                display_answer(new_game, question.answer_b, 'B', MAGENTA_MAGENTA, BLUE_MAGENTA, right, top);
                return 0;
            }

        case 'c':
            display_answer(new_game, question.answer_c, 'C', YELLOW_YELLOW, BLUE_YELLOW, left, bottom);
            sleep(1);

            if (chosen_answer == question.correct_answer) {
                display_answer(new_game, question.answer_c, 'C', GREEN_GREEN, BLUE_GREEN, left, bottom);
                return 1;
            } else {
                display_answer(new_game, question.answer_c, 'C', MAGENTA_MAGENTA, BLUE_MAGENTA, left, bottom);
                return 0;
            }

        case 'd':
            display_answer(new_game, question.answer_d, 'D', YELLOW_YELLOW, BLUE_YELLOW, right, bottom);
            sleep(1);

            if (chosen_answer == question.correct_answer) {
                display_answer(new_game, question.answer_d, 'D', GREEN_GREEN, BLUE_GREEN, right, bottom);
                return 1;
            } else {
                display_answer(new_game, question.answer_d, 'D', MAGENTA_MAGENTA, BLUE_MAGENTA, right, bottom);
                return 0;
            }
    }

    return 0;
}

/**
 * Get the current time and display it in the time window
 *
 * @param time_window      the time window
 */
void display_current_time(WINDOW *time_window) {
    struct tm *area;
    time_t t;

    t = time(NULL);
    area = localtime(&t);

    mvwprintw(time_window, 2, 1, "%s", asctime(area));
}

/**
 * Create a new window containing the current time and player name
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