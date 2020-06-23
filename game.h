#ifndef GAME_H
#define GAME_H

#include "helpers.h"
#include "questions.h"

#define INVALID -1
#define WRONG_ANSWER 0
#define RIGHT_ANSWER 1
#define SKIP_QUESTION 2
#define FIFTY_FIFTY 3

typedef struct {
    char *player_name;
    int score, right_answers, wrong_answers;
    int question_index;
    int used_5050, used_skip;
} Game;

Game start_new_game(Game game, Question *questions, int questions_number, const char *player_name, int rows, int cols);

int choose_answer(WINDOW *new_game, Question question, Game game, int game_w, int game_h);

void display_time_window(const char *player_name, int rows, int cols);

void display_scoreboard(int score, int right_answers, int wrong_answers, int rows, int cols);

WINDOW *display_helping_options(int game_h, int game_y,  int cols);

void highlight_answer(WINDOW *new_game, Question question, char chosen_answer, int game_w, int game_h, short
box_color, short text_color);

void print_invalid_message(WINDOW *new_game, int game_h, int game_w);

char *get_hidden_answers(char correct_answer);

#endif //GAME_H
