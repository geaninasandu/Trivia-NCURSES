#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "helpers.h"

typedef struct {
    char *name;
    int score;
} Score_entry;


void add_to_scoreboard(const char *filename, const char *player_name, int score);

Score_entry *parse_scoreboard(char *filename);

void display_scoreboard(Score_entry *scores, int rows, int cols);


#endif //SCOREBOARD_H