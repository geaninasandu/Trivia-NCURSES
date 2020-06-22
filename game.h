#ifndef GAME_H
#define GAME_H

#include "helpers.h"
#include "questions.h"

int start_new_game(Question *questions, int questions_number, const char
*player_name, int rows, int cols);

int choose_answer(WINDOW *new_game, Question question, int game_w, int game_h);

void display_time_window(const char *player_name, int rows, int cols);

void display_scoreboard(int score, int right_answers, int wrong_answers, int
rows, int cols);

#endif //GAME_H
