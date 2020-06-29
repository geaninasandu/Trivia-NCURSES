#include "headers/helpers.h"
#include "headers/menu.h"
#include "headers/questions.h"
#include "headers/game.h"

int main(int argc, char **argv) {
    size_t questions_number = 0;
    int rows, cols, choice;
    char *player_name;

    /* Display the format for running the game if the number of arguments is not matched */
    if (argc != 3) {
        printf("Format: ./trivia questions_file.txt scoreboard.txt");
        return 1;
    }

    /* Initialize the screen and the background of the game */
    init_screen();
    getmaxyx(stdscr, rows, cols);

    WINDOW *background = newwin(rows, cols, 0, 0);
    configure_background(background, rows, cols);
    wrefresh(background);

    /* Parse the file containing the questions and save them in an array of struct Question */
    Question *questions = parse_questions(argv[1], &questions_number);
    Game *game = NULL;
    Score_entry *scores;

    /* Display the menu options and handle the user choice until the QUIT GAME option is chosen */
    while (true) {
        wclear(background);
        configure_background(background, rows, cols);
        wrefresh(background);

        choice = display_menu(rows, cols);

        switch (choice) {
            case NEW_GAME:
                /* If the new game option is chosen, ask the player for their name and create an default Game object,
                 * storing the initial game state */
                player_name = set_name_window(rows, cols);
                player_name[strlen(player_name) - 1] = '\0';

                game = malloc(sizeof(Game));
                game->player_name = strdup(player_name);
                game->score = 0;
                game->wrong_answers = 0;
                game->right_answers = 0;
                game->used_5050 = FALSE;
                game->used_skip = FALSE;
                game->question_index = 0;

                /* Call the start_new_game function, which opens the game window and performs the necessary actions
                 * to handle the game; The function returns a flag, representing the state of the game */
            start_game:
                wrefresh(background);
                int game_flag = start_new_game(game, questions, questions_number, argv[2], rows, cols);

                switch (game_flag) {
                    case QUIT_TO_MENU:
                        /* If the 'q' key was pressed, go back to the menu window by continuing the loop */
                        continue;

                    case MENU_END:
                        /* If the user chose the "Menu" option after finishing the game, reset the game state variable
                         * and continue the loop */
                        game = NULL;
                        continue;

                    case SCOREBOARD_END:
                        /* If the player chose the "Scoreboard" end option, clear the windows and show the score
                         * window */
                        game = NULL;
                        wclear(background);
                        configure_background(background, rows, cols);
                        wrefresh(background);
                        scores = parse_scoreboard(argv[2]);
                        display_scoreboard(scores, rows, cols);
                        continue;

                    case QUIT_END:
                        goto quit_game;

                    default:
                        break;
                }
                break;

            case RESUME_GAME:
                /* If the player chose the "Resume" option, check if the game state contains any data; if it doesn't,
                 * display a message, otherwise continue the game with the current state */
                if (!game) {
                    mvwprintw(background, rows / 6 + 6, (cols - 17) / 2, "Start a new game!");
                    wrefresh(background);
                    sleep(1);
                    continue;
                } else {
                    goto start_game;
                }

            case SCOREBOARD:
                /* Display the scoreboard */
                scores = parse_scoreboard(argv[2]);
                display_scoreboard(scores, rows, cols);
                continue;

            quit_game:
            case QUIT_GAME:
                /* If the "Quit" option is chosen, free the allocated variables and exit the game */
                if (game) {
                    free(game);
                }

                if (scores) {
                    free(scores);
                }

                free(questions);
                endwin();
                return 0;

            default:
                break;
        }
    }
}