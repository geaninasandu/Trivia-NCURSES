#include "headers/questions.h"

/**
 * Open the questions.txt file and parse its content, adding the questions to an array of Question structures;
 *
 * @param filename              the name of the file;
 * @param questions_number      variable passed by reference to return the size of the questions array;
 * @return                      the array of Questions;
 */
Question *parse_questions(char *filename, size_t *questions_number) {
    FILE *questions_file;
    size_t line_length = 0, array_length = 10;
    char *question_line = NULL;
    Question *questions;

    questions_file = fopen(filename, "r");
    if (!questions_file) {
        printf("[Error]: Could not open file %s.", filename);
        exit(1);
    }

    questions = malloc(array_length * sizeof(Question));

    while (getline(&question_line, &line_length, questions_file) != -1) {

        /* Parse the line and add a new entry in the routing table array */
        questions[*questions_number] = parse_question_line(question_line);
        (*questions_number)++;

        /* If the number of entries exceeds the allocated size, reallocate the array */
        if ((*questions_number) == array_length) {
            array_length *= 10;
            questions = (Question *) realloc(questions, array_length * sizeof(Question));
        }
    }

    return questions;
}

/**
 * Doubles the size allocated for a string if the index reached the allocated size;
 *
 * @param index             the index reached within the string;
 * @param length            the length allocated;
 * @return                  the new string
 */
char *reallocate_string(char *string, const size_t index, size_t *length) {
    if (index >= (*length)) {
        (*length) *= 2;
        string = (char *) realloc(string, (*length) * sizeof(char));
    }

    return string;
}

/**
 * Traverse the line of the question and add characters to their corresponding string, then add the strings to a
 * Question structure;
 *
 * @param question_line         the line to be parsed;
 * @return                      the Question object resulted;
 */
Question parse_question_line(const char *question_line) {
    Question question;
    int i = 0, j = 0, field = 0;
    size_t question_length = 100;
    size_t answer_length = 10;
    char *question_str = malloc(question_length * sizeof(char));
    char *a_str = malloc(answer_length * sizeof(char));
    char *b_str = malloc(answer_length * sizeof(char));
    char *c_str = malloc(answer_length * sizeof(char));
    char *d_str = malloc(answer_length * sizeof(char));

    /* Traverse the line and add characters to the required strings */
    while (question_line[i] != '\n' && question_line[i]) {
        switch (field) {
            case 0:
                question_str[j] = question_line[i];
                question_str[j + 1] = '\0';
                question_str = reallocate_string(question_str, j, &question_length);
                break;

            case 1:
                a_str[j] = question_line[i];
                a_str[j + 1] = '\0';
                a_str = reallocate_string(a_str, j, &answer_length);
                break;

            case 2:
                b_str[j] = question_line[i];
                b_str[j + 1] = '\0';
                b_str = reallocate_string(b_str, j, &answer_length);
                break;

            case 3:
                c_str[j] = question_line[i];
                c_str[j + 1] = '\0';
                c_str = reallocate_string(c_str, j, &answer_length);
                break;

            case 4:
                d_str[j] = question_line[i];
                d_str[j + 1] = '\0';
                d_str = reallocate_string(d_str, j, &answer_length);
                break;

            case 5:
                question.correct_answer = question_line[i];
                break;

            default:
                break;
        }

        /* If the next character is ';', increase the field number and jump to the character after it; Otherwise, go
         * to the next character */
        if (question_line[i + 1] == ';') {
            i += 2;
            j = 0;
            field++;
            answer_length = 10;
        } else {
            i++;
            j++;
        }
    }

    /* Copy the value of the strings to the structure members */
    question.question = strdup(question_str);
    question.answer_a = strdup(a_str);
    question.answer_b = strdup(b_str);
    question.answer_c = strdup(c_str);
    question.answer_d = strdup(d_str);

    /* Free the strings */
    free(question_str);
    free(a_str);
    free(b_str);
    free(c_str);
    free(d_str);

    return question;
}

/**
 * Display the text of the question to the middle of the screen; Split it if necessary;
 *
 * @param new_game              the game window;
 * @param question              the text of the question;
 * @param question_number       the number of the question;
 */
void display_question(WINDOW *new_game, char *question, int question_number, const int rows, const int cols) {
    int question_length = (int) strlen(question);
    const int question_y = rows / 4;

    wattron(new_game, COLOR_PAIR(RED_BLUE));

    /* Check if the question is short enough to be displayed on a single row;
     * If the question is too long, split it in two and display the parts */
    if (question_length < cols - 24) {
        const int question_x = (cols - question_length) / 2 - 7;

        /* Display the question */
        mvwprintw(new_game, question_y, question_x, "Question #%d: %s", question_number, question);
    } else {
        int line_break = 0;

        /* Find the index of the " " character, to avoid splitting a word */
        for (int i = cols - 24; i > 0; i--) {
            if (question[i] == ' ') {
                line_break = i;
                break;
            }
        }

        /* Get the first part of the question */
        char split_question[line_break + 2];
        strncpy(split_question, question, line_break);
        split_question[line_break] = '\0';

        /* Get the second row of the question */
        char *question_second = question + line_break;
        const int question_x = (cols - (int) strlen(question_second)) / 2;

        /* Display the two rows */
        mvwprintw(new_game, question_y, (cols - line_break) / 2 - 7, "Question #%d: %s", question_number,
                  split_question);
        mvwprintw(new_game, question_y + 1, question_x, "%s", question_second);
    }
}

/**
 * Show the 4 answer boxes to the window;
 */
void display_all_answers(WINDOW *new_game, const Question question, const int game_w, const int game_h) {
    const int left = game_w / 4 - 15;
    const int right = game_w / 4 - 15 + game_w / 2;
    const int top = game_h / 2;
    const int bottom = game_h / 2 + 4;

    display_answer(new_game, question.answer_a, 'A', RED_RED, BLUE_RED, left, top);
    display_answer(new_game, question.answer_b, 'B', RED_RED, BLUE_RED, right, top);
    display_answer(new_game, question.answer_c, 'C', RED_RED, BLUE_RED, left, bottom);
    display_answer(new_game, question.answer_d, 'D', RED_RED, BLUE_RED, right, bottom);
}

/**
 * Print an answer to the screen, coloring its box and text correspondingly;
 */
void display_answer(WINDOW *new_game, const char *answer, const char letter, const short box_color, const short
text_color, const int box_x, const int box_y) {

    /* Print a 3x30 box, containing the answer */
    wattron(new_game, COLOR_PAIR(box_color));
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 30; ++j) {
            mvwprintw(new_game, box_y + i, box_x + j, "-");
        }
    }

    /* Print the answer text */
    wattron(new_game, COLOR_PAIR(text_color));
    mvwprintw(new_game, box_y + 1, box_x + 2, "%c. %s", letter, answer);

    wrefresh(new_game);
}
