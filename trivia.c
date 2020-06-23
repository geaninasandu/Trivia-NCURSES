#include "helpers.h"
#include "menu.h"
#include "questions.h"
#include "game.h"

//
//#define BUFFER 200
//
///*definim o structura cu 2 campuri care stocheaza numele jucatorului si scorul
//acestuia pentru a le salva in fisierele scoreboard si leaderboard*/
//typedef struct {
//    char name_file[100];
//    int score_file;
//} Scorelist;
//

//
//int generate_random_answer(int correct_answer);
//
//int *hidden_answers(int correct_answer);
//
//void display_5050(WINDOW *, int, int, int);
//
//void display_skip_question(WINDOW *, int, int, int);
//
//WINDOW *display_helping_options(int game_h, int game_y);
//
//int start_new_game(int rows, int cols, char *player_name, char *question,
//                   int question_nr, int skip_question,
//                   int fifty_fifty) { //afiseaza ecranul de joc; aici m-am cam plictisit sa fac functii se pare
////aveam prea multi parametri de transmis, scz
//    //actualizeaza ora si data actuala
//    time_window(rows, cols, player_name);
//    int game_w = cols - 20;
//    int game_h = rows / 2;
//    int game_x = 10;
//    int game_y = (rows - game_h) * 2 / 3;
//    //creeaza fereastra jocului
//    WINDOW *new_game = newwin(game_h, game_w, game_y, game_x);
//    configure_window(new_game, game_h, game_w);
//    int question_x, i = 0, quest_len = 0, line_break;
//    int question_y = game_h / 4;
//    //determina lungimea intrebarii
//    while (question[i] != ';') {
//        quest_len++;
//        i++;
//    }
//    i = 0;
//    //coordonata de centrare pentru intrebare
//    question_x = (game_w - quest_len - 14) / 2;
//    //daca intrebarea este prea lunga pentru a fi afisata pe o singura linie
//    //gaseste ultimul cuvant care incape pe linie, apoi continua intrebarea
//    if (quest_len > game_w - 24) //pe linia urmatoare
//    {
//        for (i = game_w - 24; i > 0; i--)
//            if (question[i] == ' ') {
//                line_break = i; //pozitia din intrebare unde se da break
//                break;
//            }
//        question_x = (game_w - line_break - 14) / 2;
//        //numarul intrebarii
//        mvwprintw(new_game, question_y, question_x, "Question #%d:",
//                  question_nr);
//        //afiseaza fiecare caracter pana la line_break
//        for (i = 0; i < line_break; i++)
//            mvwprintw(new_game, question_y, question_x + 14 + i, "%c",
//                      question[i]);
//        i = line_break;
//        question_x = (game_w - quest_len + line_break) / 2;
//        //afiseaza restul intrebarii
//        while (question[i] != ';') {
//            mvwprintw(new_game, question_y + 1, question_x, "%c", question[i]);
//            question_x++;
//            i++;
//        }
//    } else {   //daca intrebarea incape pe o linie se afiseaza pur si simplu
//        mvwprintw(new_game, question_y, question_x, "Question #%d:",
//                  question_nr);
//        while (question[i] != ';') {
//            mvwprintw(new_game, question_y, question_x + 14, "%c", question[i]);
//            question_x++;
//            i++;
//        }
//    }
//    wrefresh(new_game);
//    int correct_answer, answer_right = 0;
//    int a_hidden = 0, b_hidden = 0, c_hidden = 0, d_hidden = 0;
//    int answer1_len, answer2_len, answer3_len;
//    /*afisam chenarul pentru varianta A de raspuns si continutul acesteia;
//    answer1_len ia valoarea lungimii sirului pana la sfarsitul raspunsului 1*/
//    //2 - culoarea chenarului (rosu), 10 - culoarea textului (albastru)
//    answer1_len = answer_1(new_game, game_h, game_w, quest_len, question, 2,
//                           10);
//    //analog varianta B,C,D
//    answer2_len = answer_2(new_game, game_h, game_w, answer1_len, question, 2,
//                           10);
//    answer3_len = answer_3(new_game, game_h, game_w, answer2_len, question, 2,
//                           10);
//    answer_4(new_game, game_h, game_w, answer3_len, question, 2, 10);
//    //determina numarul raspunsului corect
//    correct_answer = question[strlen(question) - 2] - '0';
//
//    int help_h = 3;
//    int help_w = cols * 2 / 3;
//    int help_y = game_h + game_y + 1;
//    int help_x = (cols - help_w) / 2;
//    //fereastra pentru casetele cu variante ajutatoare
//    WINDOW *help = newwin(help_h, help_w, help_y, help_x);

//    //afiseaza variantele 50-50 si skip question daca nu au mai fost folosite
//    if (fifty_fifty != 1) display_5050(help, cols, 2, 10);
//    if (skip_question != 1) display_skip_question(help, cols, 2, 10);
//    //utilizatorul trimite raspunsul de la tastatura si acesta este verificat
//    ANSWER:
//    switch (wgetch(new_game)) {
//        /*daca se apasa tasta a, timpul este actualizat iar caseta cu raspunsul
//        este colorata galben; programul are un delay de 1 secunda; dupa o sec
//        caseta este colorata verde daca raspunsul a fost cel corect si magenta
//        in caz contrar; variabila a_hidden verifica daca raspunsul A a fost
//        ascuns de optiunea 50-50; daca a fost ascuns, varianta A nu poate fi
//        aleasa; analog b, c, d*/
//        case 'a':
//            time_window(rows, cols, player_name);
//            if (!a_hidden) {
//                answer_1(new_game, game_h, game_w, quest_len, question, 5, 13);
//                sleep(1);
//                if (correct_answer == 1) {
//                    answer_1(new_game, game_h, game_w, quest_len, question, 4,
//                             14);
//                    answer_right = 1;
//                } else
//                    answer_1(new_game, game_h, game_w, quest_len, question, 3,
//                             15);
//            } else {
//                goto INVALID;
//                goto ANSWER;
//            }
//            break;
//        case 'b':
//            time_window(rows, cols, player_name);
//            if (!b_hidden) {
//                answer_2(new_game, game_h, game_w, answer1_len, question, 5,
//                         13);
//                sleep(1);
//                if (correct_answer == 2) {
//                    answer_2(new_game, game_h, game_w, answer1_len, question, 4,
//                             14);
//                    answer_right = 1;
//                } else
//                    answer_2(new_game, game_h, game_w, answer1_len, question, 3,
//                             15);
//            } else {
//                goto INVALID;
//                goto ANSWER;
//            }
//            break;
//        case 'c':
//            time_window(rows, cols, player_name);
//            if (!c_hidden) {
//                answer_3(new_game, game_h, game_w, answer2_len, question, 5,
//                         13);
//                sleep(1);
//                if (correct_answer == 3) {
//                    answer_3(new_game, game_h, game_w, answer2_len, question, 4,
//                             14);
//                    answer_right = 1;
//                } else
//                    answer_3(new_game, game_h, game_w, answer2_len, question, 3,
//                             15);
//            } else {
//                goto INVALID;
//                goto ANSWER;
//            }
//            break;
//        case 'd':
//            time_window(rows, cols, player_name);
//            if (!d_hidden) {
//                answer_4(new_game, game_h, game_w, answer3_len, question, 5,
//                         13);
//                sleep(1);
//                if (correct_answer == 4) {
//                    answer_4(new_game, game_h, game_w, answer3_len, question, 4,
//                             14);
//                    answer_right = 1;
//                } else
//                    answer_4(new_game, game_h, game_w, answer3_len, question, 3,
//                             15);
//            } else {
//                goto INVALID;
//                goto ANSWER;
//            }
//            break;
//        case 'q':
//            time_window(rows, cols, player_name);
//            //daca se apasa tasta Q, fereastra de joc si optiuni ajutatoare
//            werase(new_game); //este stearsa si se revine in main cu valoarea 2
//            werase(help);
//            wrefresh(help);
//            wrefresh(new_game);
//            return 2;
//            break;
//        case 'e':
//            time_window(rows, cols, player_name);
//            /*daca se apasa tasta e se alege varianta 50-50; daca nu a mai fost
//            folosita inainte, este ascunsa caseta si se apeleaza functia
//            hidden_answers care returneaza un vector cu cele doua raspunsuri
//            ascunse*/
//            if (fifty_fifty != 1) {
//                display_5050(help, cols, 1, 1);
//                fifty_fifty = 1;
//                int *hidden_answer = hidden_answers(correct_answer);
//                //gaseste raspunsurile care trebuie eliminate si le ascunde
//                if (hidden_answer[0] == 1 || hidden_answer[1] == 1) {
//                    a_hidden = 1;
//                    answer_1(new_game, game_h, game_w, quest_len, question, 1,
//                             1);
//                }
//                if (hidden_answer[0] == 2 || hidden_answer[1] == 2) {
//                    b_hidden = 1;
//                    answer_2(new_game, game_h, game_w, answer1_len, question, 1,
//                             1);
//                }
//                if (hidden_answer[0] == 3 || hidden_answer[1] == 3) {
//                    c_hidden = 1;
//                    answer_3(new_game, game_h, game_w, answer2_len, question, 1,
//                             1);
//                }
//                if (hidden_answer[0] == 4 || hidden_answer[1] == 4) {
//                    d_hidden = 1;
//                    answer_4(new_game, game_h, game_w, answer3_len, question, 1,
//                             1);
//                }
//                free(hidden_answer);
//            } //daca varianta a mai fost folosita nu poate di folosita din nou
//            else goto INVALID;
//            goto ANSWER;
//            break;
//        case 'f':
//            time_window(rows, cols, player_name);
//            //daca optiunea nu a mai fost folosita, ascunde caseta si sterge
//            if (skip_question != 1) //fereastra de pe ecran
//            { //se intoarce in main cu valoarea 3
//                display_skip_question(help, cols, 1, 1);
//                skip_question = 1;
//                werase(new_game);
//                wrefresh(new_game);
//                return 3;
//            } else {
//                goto INVALID;
//                goto ANSWER;
//            }
//            break;
//        INVALID:
//        default:
//            //daca se apasa o tasta invalida actualizeaza timpul si afiseaza
//            //un mesaj de instructiune apoi primeste o alta tasta
//            mvwprintw(new_game, game_h / 2 + 7, (game_w - 15) / 2,
//                      "Pick an answer!");
//            wrefresh(new_game);
//            time_window(rows, cols, player_name);
//            goto ANSWER;
//            break;
//    }
//    if (answer_right) {   //daca raspunsul este corect, sterge ecranul si asteapta o secunda
//        sleep(1); //pana trece la urmatoarea intrebare
//        werase(help);
//        wrefresh(help);
//        werase(new_game);
//        wrefresh(new_game);
//        //daca a fost folosita optiunea 50-50 returneaza 4
//        if (fifty_fifty) return 4;
//        //daca nu a fost folosita, returneaza 1
//        return 1;
//    } else {   //daca raspunsul ales nu este cel corect coloreaza cu verde caseta
//        switch (correct_answer) //raspunsului corect
//        {
//            case 1:
//                answer_1(new_game, game_h, game_w, quest_len, question, 4, 14);
//                break;
//            case 2:
//                answer_2(new_game, game_h, game_w, answer1_len, question, 4,
//                         14);
//                break;
//            case 3:
//                answer_3(new_game, game_h, game_w, answer2_len, question, 4,
//                         14);
//                break;
//            case 4:
//                answer_4(new_game, game_h, game_w, answer3_len, question, 4,
//                         14);
//                break;
//        }
//        sleep(1);
//        werase(help);
//        wrefresh(help);
//        werase(new_game);
//        wrefresh(new_game);
//        //daca a fost folosita optiunea 50-50 returneaza 5, daca nu 0
//        if (fifty_fifty) return 5;
//        return 0;
//    }
//}
//
//
//int generate_random_answer(int correct_answer) { /*genereaza un numar random de la 1 la 4 care reprezinta numarul raspunsului
//ramas dupa 50-50, pe langa raspunsul corect*/
//    int answer_left;
//    //seteaza timpul curent ca seed
//    srand(time(NULL));
//    RANDOM:
//    answer_left = (rand() % 4 + 1);
//    if (answer_left == correct_answer) goto RANDOM;
//    //returneaza raspunsul ramas
//    return answer_left;
//}
//
//int *hidden_answers(
//        int correct_answer) { //genereaza un vector de 2 numere care reprezinta cele 2 numere care trebuie
////ascunse dupa 50-50 si il returneaza
//    int answer_left = generate_random_answer(correct_answer);
//    int i, j = 0;
//    int *hidden_answer = malloc(2 * sizeof(int));
//    for (i = 1; i <= 4; i++)
//        if (i != correct_answer && i != answer_left) {
//            hidden_answer[j] = i;
//            j++;
//        }
//    return hidden_answer;
//}
//
//void end_options(WINDOW *end_win, int current_option, int end_h, int end_w);
//
//int choose_end_option(WINDOW *end_win, int end_h, int end_w);
//
//int end_game(int score, int right_answers, int wrong_answers,
//             char *player_name) { //creeaza o fereastra in care sunt afisate numele jucatorului, scorul
////obtinut de acesta, nr de raspunsuri corecte si gresite
//    //fereastra are de asemenea un meniu care permite jucatorului sa aleaga
//    int rows, cols; //urmatoarea optiune cu ajutorul sagetilor stanga-dreapta
//    getmaxyx(stdscr, rows, cols);
//    int end_w = cols / 2;
//    int end_h = rows / 2;
//    int end_x = (cols - end_w) / 2;
//    int end_y = (rows - end_h) * 2 / 3;
//    WINDOW *end_win = newwin(end_h, end_w, end_y, end_x);
//    configure_window(end_win, end_h, end_w);
//    mvwprintw(end_win, end_h * 1 / 5, (end_w - strlen(player_name) - 18) / 2,
//              "Congratulations, %s!", player_name);
//    mvwprintw(end_win, end_h * 1 / 3, (end_w - 14) / 2, "Your score: %d",
//              score);
//    mvwprintw(end_win, end_h * 1 / 3 + 1, (end_w - 17) / 2, "Right answers: %d",
//              right_answers);
//    mvwprintw(end_win, end_h * 1 / 3 + 2, (end_w - 17) / 2, "Wrong answers: %d",
//              wrong_answers);
//    wrefresh(end_win);
//
//    keypad(end_win, TRUE);
//    //variabila end_choice primeste numarul optiunii alese si il returneaza
//    int end_choice = choose_end_option(end_win, end_h, end_w);
//    return end_choice;
//}
//
//int choose_end_option(WINDOW *end_win, int end_h,
//                      int end_w) { //navigheaza intre optiunile meniului pana se alege o optiune
//    int current_option = 1, end_choice = 0;
//    //afiseaza optiunile meniului, optiunea highlightata fiind prima din lista
//    end_options(end_win, current_option, end_h, end_w);
//    while (end_choice == 0) { /*programul primeste input cat timp nu s-a ales niciuna dintre optiunile
//    meniului (nu s-a apasat enter)*/
//        switch (wgetch(end_win)) {
//            /*daca s-a apasat enter se iese din while si se salveza optiunea
//            aleasa in variabila choice*/
//            case 10:
//                end_choice = current_option;
//                break;
//            case KEY_RIGHT:
//                //daca s-a apasat sageata jos se trece la optiunea urmatoare
//                if (current_option != 3)
//                    current_option++;
//                    //daca optiunea curenta este ultima din lista trece la prima
//                else
//                    current_option = 1;
//                break;
//            case KEY_LEFT: //analog
//                if (current_option != 1)
//                    current_option--;
//                else
//                    current_option = 3;
//                break;
//            default:
//                mvwprintw(end_win, end_h * 3 / 4 - 1, (end_w - 19) / 2,
//                          "Use the arrow keys!");
//                wrefresh(end_win);
//                break;
//        }
//        //afiseaza si highlighteaza optiunile
//        end_options(end_win, current_option, end_h, end_w);
//    }
//    refresh();
//    return end_choice;
//}
//
//void end_options(WINDOW *end_win, int current_option, int end_h,
//                 int end_w) { //afiseaza in fereastra meniului optiunile si o highlighteaza pe cea curenta
//    //seteaza lista de optiuni ale meniului
//    char *options[] = {"Leaderboard", "Menu", "Quit"};
//    int option_x, option_y, i, j, k;
//    //coordonata y la care trebuie afisata prima optiune pentru a fi centrata
//    option_x = end_w / 16;
//    option_y = end_h * 3 / 4;
//    //verifica pe care dintre cele 3 optiuni trebuie sa o highlighteze
//    for (i = 1; i <= 3; i++) {
//        int text_x = option_x + (end_w / 4 - strlen(options[i - 1])) / 2;
//        if (current_option == i) {
//            //afiseaza chenarul pe 3 linii
//            for (j = 0; j < 3; j++)
//                for (k = 0; k < end_w / 4; k++)
//                    mvwprintw(end_win, option_y + j, option_x + k, "-");
//            //afiseaza textul la mijlocul chenarului
//            wattron(end_win, COLOR_PAIR(BLUE_GREEN));
//            mvwprintw(end_win, option_y + 1, text_x, "%s", options[i - 1]);
//        } else { /*celelalte optiuni din lista nu sunt highlightate, chenarul este
//        facut invizibil iar textul revine la atributele default*/
//            wattron(end_win, COLOR_PAIR(RED_RED));
//            for (j = 0; j < 3; j++)
//                for (k = 0; k < end_w / 4; k++)
//                    mvwprintw(end_win, option_y + j, option_x + k, "-");
//            mvwprintw(end_win, option_y + 1, text_x, "%s", options[i - 1]);
//        }
//        option_x = option_x + (end_w * 5 / 16);
//    }
//    wrefresh(end_win);
//}
//
//void swap(int *x, int *y) { //interschimbare intregi
//    int aux = *x;
//    *x = *y;
//    *y = aux;
//}
//
//void add_to_scorefile(char *player_name, int score,
//                      char *file_name) { //adauga in fisierul primit ca al doilea argument (scoreboard.txt) numele
////si scorul jucatorului
//    //deschide fisierul cu functia append
//    FILE *scorefile = fopen(file_name, "a");
//    Scorelist line;
//    strcpy(line.name_file, player_name);
//    line.score_file = score;
//    fprintf(scorefile, "%s;%d\n", line.name_file, line.score_file);
//    fclose(scorefile);
//}
//
//void add_to_leaderboard(char *file_name1,
//                        char *file_name2) { //sorteaza continutul fisierului scoreboard.txt si il scrie in leaderboard
//    char entry[100], aux[100];
//    Scorelist scores[100];
//    int i = 0, j, negative = 0, rows, cols;
//    //deschide scoreboard pentru citire si leaderboard pentru scriere
//    FILE *scorefile = fopen(file_name1, "r");
//    FILE *leaderboard = fopen(file_name2, "w");
//    getmaxyx(stdscr, rows, cols);
//    while (fgets(entry, 100, scorefile)) {
//        j = 0;
//        negative = 0;
//        scores[i].score_file = 0;
//        aux[0] = '\0';
//        //adauga numele jucatorului din fisier intr-un string aux
//        while (entry[j] != ';') {
//            aux[j] = entry[j];
//            j++;
//            aux[j] = '\0';
//        }
//        //copiaza numele intr-un vector de structuri
//        strcpy(scores[i].name_file, aux);
//        aux[0] = '\0';
//        j++;
//        //adauga scorul in vectorul de structuri
//        while (j != strlen(entry) - 1) {
//            if (entry[j] ==
//                '-') {   //daca numarul e negativ trece la urmatorul pas
//                j++;
//                negative = 1;
//                continue;
//            }
//            //adauga scorul, convertind din char in int
//            scores[i].score_file = scores[i].score_file * 10 + (entry[j] - '0');
//            j++;
//        } //daca numarul era negativ, inmultim scorul obtinut cu -1
//        if (negative == 1) scores[i].score_file = scores[i].score_file * (-1);
//        i++;
//    } //numarul de linii
//    int line_nr = i;
//    //sorteaza intrarile din fisier in ordine descrescatoare, interschimband
//    for (i = 0; i < line_nr - 1; i++) //scorul si numele
//        for (j = 0; j < line_nr - i - 1; j++)
//            if (scores[j].score_file < scores[j + 1].score_file) {
//                swap(&scores[j].score_file, &scores[j + 1].score_file);
//                strcpy(aux, scores[j].name_file);
//                strcpy(scores[j].name_file, scores[j + 1].name_file);
//                strcpy(scores[j + 1].name_file, aux);
//            }
//    //scrie in fisierul leaderboard.txt scorurile sortate
//    for (i = 0; i < line_nr; i++)
//        fprintf(leaderboard, "%s %d\n", scores[i].name_file,
//                scores[i].score_file);
//    //creeaza o fereastra de leaderboard in care sunt afisate primele 10 linii
//    int leader_w = cols / 2; //din fisierul leaderboard.txt
//    int leader_h = rows / 2;
//    int leader_x = (cols - leader_w) / 2;
//    int leader_y = (rows - leader_h) * 2 / 3;
//    WINDOW *leader_win = newwin(leader_h, leader_w, leader_y, leader_x);
//    configure_window(leader_win, leader_h, leader_w);
//    mvwprintw(leader_win, leader_h / 6, (leader_w - 11) / 2, "Leaderboard");
//    for (i = 0; i < 10; i++) {
//        for (j = 15; j <= leader_w - 16; j++)
//            mvwprintw(leader_win, leader_h * 2 / 5 - 2 + i, j, ".");
//        mvwprintw(leader_win, leader_h * 2 / 5 - 2 + i, 15, "%2d. %s", i + 1,
//                  scores[i].name_file);
//        mvwprintw(leader_win, leader_h * 2 / 5 - 2 + i, leader_w - 18, "%3dp",
//                  scores[i].score_file);
//    } //jocul revine la meniu prin apasarea oricarei taste
//    mvwprintw(leader_win, leader_h - 2, leader_w / 2 - 11,
//              "Press any key to exit!");
//    wrefresh(leader_win);
//    wgetch(leader_win);
//    fclose(scorefile);
//    fclose(leaderboard);
//}

int main(int argc, char **argv) {
    size_t questions_number = 0;
    int rows, cols, choice;
    char *player_name;

    if (argc < 4) {
        printf("Format: ./trivia questions_file.txt scoreboard.txt leaderboard.txt");
        return 1;
    }

    init_screen();
    getmaxyx(stdscr, rows, cols);

    WINDOW *background = newwin(rows, cols, 0, 0);
    configure_background(background, rows, cols);
    wrefresh(background);

    Question *questions = parse_questions(argv[1], &questions_number);
    choice = display_menu(rows, cols);

    switch (choice) {
        case 0:
            player_name = set_name_window(rows, cols);
            player_name[strlen(player_name) - 1] = '\0';

            Game game;
            game = (Game) {
                    .player_name = strdup(player_name),
                    .score = 0,
                    .wrong_answers = 0,
                    .right_answers = 0,
                    .used_5050 = FALSE,
                    .used_skip = FALSE,
            };

            start_new_game(game, questions, questions_number, player_name, rows, cols);

            wrefresh(background);

    }

    //apeleaza functii in functie de optiunea din meniu aleasa
//    MENU:
//    int choice = display_menu(rows, cols);
//    int game = 0, score, end_choice;
//    //daca jucatorul a ales optiunea new game din meniu incepe un joc nou
//    NEW_GAME:
//    if (choice == 0) {
//        //primeste numele introdus de jucator si afiseaza un mesaj pt acesta
//        char *player_name = set_name_window(rows, cols);
//        player_name[strlen(player_name) - 1] = '\0';
//        mvwprintw(background, (rows / 3 + 6) / 2 + 2,
//                  (cols - 15 - strlen(player_name)) / 2,
//                  "---Welcome, %s!---", player_name);
//        wrefresh(background);
//
//        //deschide fisierul questions_file.txt primit ca prim argument si citeste
//        FILE *questions; //intrebarile din acesta
//        questions = fopen(argv[1], "r");
//        //daca fisierul nu poate fi deschis genereaza eroare si returneaza 1
//        if (!questions) {
//            printf("[Eroare]: Fisierul %s nu poate fi deschis.", argv[1]);
//            return 1;

//
//        //aloca dinamic spatiu pentru intrebare
//        char *question = malloc(200 * sizeof(char));
//        int question_nr = 1, right, right_answers = 0;
//        int wrong_answers = 0, skip_question = 0, fifty_fifty = 0;
//        game = 1;
//        score = 0;
//        //citeste intrebarile pana la sfarsitul fisierului
//        while (fgets(question, BUFFER, questions)) {
//            //verifica daca fiecare caracter este printabil si daca nu, trece
//            RESUME_GAME:
//            printable = 1; //la linia urmatoare
//            for (i = 0; i < strlen(question) - 1; i++)
//                if (!(isprint(question[i])))
//                    printable = 0;
//            //daca este printabil, afiseaza fereastra cu scorul
//            if (printable == 1) {
//                scoreboard(score, right_answers, wrong_answers);
//                //right primeste valoarea returnata de functia start_new_game
//                right = start_new_game(rows, cols, player_name, question,
//                                       question_nr, skip_question, fifty_fifty);
//                switch (right) {   //daca se returneaza 0 atunci raspunsul dat este gresit
//                    case 0:
//                        wrong_answers++;
//                        score -= 5; //scadem scorul, crestem nr de rasp gresite
//                        break;
//                        //daca se returneaza 1 atunci raspunsul dat este corect
//                    case 1:
//                        right_answers++;
//                        score += 10; //crestem scorul si nr de rasp corecte
//                        break;
//                    case 2: //daca se returneaza 2 => jucatorul a apasat Q
//                        //afiseaza meniul si primeste numarul optiunii alese
//                    MENU2:
//                        choice = display_menu(rows, cols);
//                        if (choice == 1) goto NEW_GAME; //daca 1 incepe joc nou
//                        else if (choice == 2)
//                            goto RESUME_GAME; //2 - resume game
//                        else if (choice == 3) goto END; //3 - iese din joc
//                        else //4 - afiseaza leaderboard-ul iar dupa ce se iese
//                        { //din leaderboard se intoarce din nou la meniu
//                            add_to_leaderboard(argv[2], argv[3]);
//                            goto MENU2;
//                        }
//                        break;
//                    case 3: //daca se returneaza 3 inseamna ca a fost folosita
//                        skip_question = 1; //optiunea skip; setam variabila cu
//                        break; //1 pt a nu mai putea folosi optiunea
//                    case 4: //daca se returneaza 4 => raspunsul e corect si a
//                        right_answers++; //fost folosit 50-50
//                        score += 10;
//                        fifty_fifty = 1; //atribuie 1 pt a nu o mai folosi
//                        break;
//                    case 5: //daca se returneaza 4 => raspunsul e gresit si a
//                        wrong_answers++; //fost folosit 50-50
//                        score -= 5;
//                        fifty_fifty = 1;
//                }
//                //creste numarul intrebarii
//                question_nr++;
//            }
//        }
//        //dupa ce s-au citit toate intrebarile, se afiseaza ecranul de final
//        //care contine scorul obtinut si meniul acestuia
//        end_choice = end_game(score, right_answers, wrong_answers, player_name);
//        //se adauga scorul si numele in fisierul scoreboard
//        add_to_scorefile(player_name, score, argv[2]);
//        free(question);
//        //verifica numarul optiunii alese din meniul de la final
//        if (end_choice ==
//            1) {   //daca se alege prima optiune, se afiseaza leaderboardul
//            add_to_leaderboard(argv[2], argv[3]);
//            goto MENU;
//        } //a doua optiune => se intoarce la meniu
//        else if (end_choice == 2) goto MENU;
//        else goto END; //ultima optiune => quit
//    } else if (choice ==
//               1) {   //daca jucatorul alege optiunea resume game, dar nu s-a inceput niciun
//        //joc nou initial, afiseaza mesaj si asteapta pana se alege o optiune
//        RESUME:
//        if (game == 0) //valida
//        {
//            mvwprintw(background, (rows + 2) / 6 + 5, (cols - 17) / 2,
//                      "Start a new game!");
//            wrefresh(background);
//            choice = display_menu(rows, cols);
//            if (choice == 1) goto NEW_GAME;
//            else if (choice == 2) goto RESUME;
//            else if (choice == 3) goto END;
//            else {
//                add_to_leaderboard(argv[2], argv[3]);
//                goto MENU;
//            }
//            getch();
//        }
//    } //daca se alege optiunea 3 (quit), jocul se incheie
//    else if (choice == 3) goto END;
//    else {   //daca se alege ultima optiune, afiseaza leaderboard-ul
//        add_to_leaderboard(argv[2], argv[3]);
//        goto MENU;
//    }
//    END:
//    endwin();
//    return 0;
}