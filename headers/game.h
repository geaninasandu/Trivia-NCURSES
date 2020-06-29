#ifndef GAME_H
#define GAME_H

#include "helpers.h"
#include "questions.h"
#include "scoreboard.h"

#define QUIT_TO_MENU -1
#define WRONG_ANSWER 10
#define RIGHT_ANSWER 11
#define SKIP_QUESTION 12

#define SCOREBOARD_END 0
#define MENU_END 1
#define QUIT_END 2

typedef struct {
    char *player_name;
    int score, right_answers, wrong_answers;
    int question_index;
    int used_5050, used_skip;
} Game;


int start_new_game(Game *game, Question *questions, int questions_number, const char *score_filename, int rows, int
cols);

int choose_answer(WINDOW *game_window, WINDOW *help_window, Question question, Game *game, char *hidden_answers, int
game_w, int game_h);

char *set_name_window(int rows, int cols);

char *insert_name(WINDOW *name_window, int text_box_x, int text_box_y, int cols);

void display_time_window(const char *player_name, int rows, int cols);

void display_score_panel(int score, int right_answers, int wrong_answers, int rows, int cols);

WINDOW *display_helping_options(const Game *game, int game_h, int game_y, int cols);

void highlight_answer(WINDOW *game_window, Question question, char chosen_answer, int game_w, int game_h, short
box_color, short text_color);

char *get_hidden_answers(char correct_answer);

int choose_end_option(WINDOW *end_window, int end_h, int end_w);

void display_end_options(WINDOW *end_window, int current_option, int end_h, int end_w);

int display_end_menu(const Game *game, int rows, int cols);

void clear_game(WINDOW *game_window, WINDOW *help_window);

#endif //GAME_H
