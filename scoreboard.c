#include "headers/scoreboard.h"

/**
 * Add a line to the score file, containing the player name and their score;
 */
void add_to_scoreboard(const char *filename, const char *player_name, const int score) {
    FILE *score_file = fopen(filename, "a");
    fprintf(score_file, "%s;%d\n", player_name, score);
    fclose(score_file);
}

/**
 * Parse the score file and add the fields to an array of Score_entries; Sort the array by the score field;
 *
 * @return          the array of Score_entries;
 */
Score_entry *parse_scoreboard(char *filename) {
    Score_entry *scores = (Score_entry *) malloc(100 * sizeof(Score_entry));
    char entry[40], name[30];
    int entry_number = 0;

    FILE *scoreboard = fopen(filename, "r");

    /* Read each line of the file */
    while (fgets(entry, 100, scoreboard)) {
        int line_index = 0;
        int negative = FALSE;
        name[0] = '\0';

        /* Add each character until the ';' char to the name string */
        while (entry[line_index] != ';') {
            name[line_index] = entry[line_index];
            line_index++;
            name[line_index] = '\0';
        }

        /* Add the name to the .name field of the struct */
        scores[entry_number].name = strdup(name);
        line_index++;

        /* Get the score by converting the characters found to int; If the number is negative, multiply it by -1 */
        while (entry[line_index] != '\n' && entry[line_index]) {
            if (entry[line_index] == '-') {
                line_index++;
                negative = TRUE;
                continue;
            }

            scores[entry_number].score = scores[entry_number].score * 10 + (entry[line_index] - '0');
            line_index++;
        }

        if (negative == TRUE) {
            scores[entry_number].score = scores[entry_number].score * (-1);
        }

        /* Increase the number of score entries */
        entry_number++;
    }

    /* Sort the array */
    for (int i = 0; i < entry_number - 1; i++) {
        for (int j = 0; j < entry_number - i - 1; j++) {
            if (scores[j].score < scores[j + 1].score) {
                swap(&scores[j].score, &scores[j + 1].score);
                strcpy(name, scores[j].name);
                strcpy(scores[j].name, scores[j + 1].name);
                strcpy(scores[j + 1].name, name);
            }
        }
    }

    fclose(scoreboard);
    return scores;
}

/**
 * Create a new window, displaying the highest 10 scores in the array;
 * Return to the menu when a key is pressed;
 */
void display_scoreboard(Score_entry *scores, const int rows, const int cols) {
    const int scoreboard_w = cols / 2;
    const int scoreboard_h = rows / 2;
    const int scoreboard_x = (cols - scoreboard_w) / 2;
    const int scoreboard_y = (rows - scoreboard_h) * 2 / 3;

    WINDOW *scoreboard_window = newwin(scoreboard_h, scoreboard_w, scoreboard_y, scoreboard_x);
    configure_window(scoreboard_window, scoreboard_h, scoreboard_w);

    mvwprintw(scoreboard_window, scoreboard_h / 6, scoreboard_w / 2 - 5, "Scoreboard");

    /* Print the first 10 entries */
    for (int line_number = 0; line_number < 10; line_number++) {
        for (int j = 15; j <= scoreboard_w - 16; j++) {
            mvwprintw(scoreboard_window, scoreboard_h * 2 / 5 - 2 + line_number, j, ".");
        }

        mvwprintw(scoreboard_window, scoreboard_h * 2 / 5 - 2 + line_number, 15, "%2d. %s", line_number + 1,
                  scores[line_number].name);
        mvwprintw(scoreboard_window, scoreboard_h * 2 / 5 - 2 + line_number, scoreboard_w - 18, "%3dp",
                  scores[line_number].score);
    }

    mvwprintw(scoreboard_window, scoreboard_h - 2, scoreboard_w / 2 - 18, "Press any key to return to the menu!");
    wrefresh(scoreboard_window);

    if (wgetch(scoreboard_window))
        return;
}