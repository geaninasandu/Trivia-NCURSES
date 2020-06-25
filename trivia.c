#include "helpers.h"
#include "menu.h"
#include "questions.h"
#include "game.h"

int main(int argc, char **argv) {
    size_t questions_number = 0;
    int rows, cols, choice;
    char *player_name;

    if (argc < 3) {
        printf("Format: ./trivia questions_file.txt scoreboard.txt");
        return 1;
    }

    init_screen();
    getmaxyx(stdscr, rows, cols);

    WINDOW *background = newwin(rows, cols, 0, 0);
    configure_background(background, rows, cols);
    wrefresh(background);

    Question *questions = parse_questions(argv[1], &questions_number);
    Game *game = NULL;
    Score_entry *scores;

    while (true) {
        wclear(background);
        configure_background(background, rows, cols);
        wrefresh(background);

        choice = display_menu(rows, cols);

        switch (choice) {
            case NEW_GAME:
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

                start_game:
                wrefresh(background);
                int game_state = start_new_game(game, questions, questions_number, argv[2], rows, cols);

                switch (game_state) {
                    case QUIT_TO_MENU:
                        continue;

                    case MENU_END:
                        game = NULL;
                        continue;

                    case SCOREBOARD_END:
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
                if (game == NULL) {
                    mvwprintw(background, rows / 6 + 6, (cols - 17) / 2, "Start a new game!");
                    wrefresh(background);
                    sleep(1);
                    continue;
                } else {
                    goto start_game;
                }

            case SCOREBOARD:
                scores = parse_scoreboard(argv[2]);
                display_scoreboard(scores, rows, cols);
                continue;

            quit_game:
            case QUIT_GAME:
                endwin();
                return 0;

            default:
                break;
        }
    }
}