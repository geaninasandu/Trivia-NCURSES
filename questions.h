#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "helpers.h"

typedef struct {
    char *question;
    char *answer_a, *answer_b, *answer_c, *answer_d;
    char correct_answer;
} Question;

Question parse_question_line(const char *question_line);

Question *parse_questions(char *filename, size_t *questions_number);

void display_question(WINDOW *new_game, char *question, int question_number, int rows, int cols);

void display_all_answers(WINDOW *new_game, Question question, int game_w, int game_h);

void display_answer(WINDOW *new_game, const char *answer, char letter, short box_color, short text_color, int box_x,
                    int box_y);

#endif //QUESTIONS_H
