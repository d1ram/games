#include <ncurses.h>
#include <string>
#include <vector>
#include <random>
using namespace std;

void make_border(vector<int>& side) {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dis(0, 2);
    int s = dis(rng);

    side.insert(side.begin(), s);
}
void make_border_game(vector<int>& side) {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dis(0, 2);
    int s = dis(rng);

    side.insert(side.begin(), s); 
    side.pop_back();    
}
void game(string bor, string main_hero_left, string main_hero_right, string pole, int& score) {
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    int rows = 20;
    int columns = 10;
    int start_row = 10;

    int direction = 2;
    int col = columns / 2; // Позиция персонажа по центру
    vector<int> side;

    vector<vector<string>> polje(rows, vector<string>(columns, "."));

    for (int i = 0; i < rows; i++) {
        make_border(side);
    }
    side[start_row] = 2;

    int floor = 0;
    string hero = main_hero_right;

    bool has_border_left;
    bool has_border_right;
    bool has_two_borders;

    bkgd(COLOR_PAIR(2));
    attron(A_BOLD);

    // Получаем центр экрана
    int center_row = LINES / 2 - rows / 2; // Центрируем по вертикали
    int center_col = COLS / 2 - columns / 2; // Центрируем по горизонтали

    while (true) {
        int ch;

        napms(75);

        clear(); 

        ch = getch();

        if (ch == 'w') {
            make_border_game(side);
            floor++;
        }

        for (int i = 0; i < rows; i++) { // print polje
            for (int j = 0; j < columns; j++) {
                mvprintw(center_row + i, center_col + j * 2, "%s ", polje[i][j].c_str());  
            }
            printw("\n"); 
        }

        // Обновление полей
        for (int i = 0; i < rows; i++) { // whole polje
            for (int j = 0; j < columns; j++) {
                polje[i][j] = pole;
            }
        }
        for (int i = 0; i < rows; i++) { // borders
            if (side[i] == 0) {
                polje[i][0] = bor;
            } else if (side[i] == 1){
                polje[i][columns - 1] = bor;
            } else if (side[i] == 2){
                polje[i][0] = bor;
                polje[i][columns - 1] = bor;
            }
        }

        has_border_left = false;
        has_border_right = false;
        has_two_borders = false;

        if (polje[start_row][0] == bor && !(polje[start_row][columns - 1] == bor)) {
            has_border_left = true;
        } else if (polje[start_row][columns - 1] == bor && !(polje[start_row][0] == bor)) {
            has_border_right = true;
        } else if (polje[start_row][columns - 1] == bor && polje[start_row][0] == bor) {
            has_two_borders = true;
        }

        // direction(1) == left, direction(2) == right;
        if (direction == 1 && col <= 1 && has_border_left || direction == 1 && col <= 1 && has_two_borders) {
            direction = 2; 
        } else if (direction == 2 && col >= columns - 2 && has_border_right || direction == 2 && col >= columns - 2 && has_two_borders) {
            direction = 1;
        } 

        if (direction == 1 && col <= 1 && !has_border_left || direction == 2 && col >= columns - 2 && !has_border_right ) break;
        if (direction == 1 && col <= 1 && !has_two_borders || direction == 2 && col >= columns - 2 && !has_two_borders ) break;

        if (direction == 1) {
            hero = main_hero_left;
            col--;
        } else if (direction == 2) {
            hero = main_hero_right;
            col++;
        }

        polje[start_row][col] = hero;

        // printw("col: %d, direction: %d\n", col, direction);
        mvprintw(center_row + rows, center_col, "Score: %d. Total record: %d\n", floor, score);
        if (floor % 10 == 0 && floor > score) {
            mvprintw(center_row + rows + 1, center_col, "(#####  %d. New record! #####)", floor);
        } else if (floor % 10 == 0) {
            mvprintw(center_row + rows + 1, center_col, "(#####  %d  #####)", floor);
        }
        refresh(); 
    }
    if (floor > score) {
        score = floor;
    }
    attroff(COLOR_PAIR(4));
    attroff(A_BOLD);
    nodelay(stdscr, FALSE);
    keypad(stdscr, FALSE);
}

void settings(string& main_hero_left ,string& main_hero_right, string& bor, string& pole) {
    string main_text = "Change character skin";
    string border_text = "Change border skin";
    string pole_text = "Change pole skin";
    string default_text = "Reset to default settings";
    string exit_text = "Back to menu";
    int picked = 0;

    while(true){
        int ch;

        clear();

        if (picked == 0) {
            main_text.append(" <==");
        } else if (picked == 1) {
            border_text.append(" <==");
        } else if (picked == 2) {
            pole_text.append(" <==");
        } else if (picked == 3) {
            default_text.append(" <==");
        } else if (picked == 4) {
            exit_text.append(" <==");
        }

        printw("%s\n", main_text.c_str());
        printw("%s\n", border_text.c_str());
        printw("%s\n", pole_text.c_str());
        printw("%s\n", default_text.c_str());
        printw("%s\n", exit_text.c_str());

        ch = getch();

        if (ch == 'w' && picked > 0) {
            picked--;
        } else if (ch == 's' && picked < 4) {
            picked++;
        }

        if (ch == 10 && picked == 0) {
            printw("Press the char on keyboard \n(LEFT SIDE)\n");
            main_hero_left = getch();
            printw("Press the char on keyboard \n(RIGHT SIDE)\n");
            main_hero_right = getch();
        } else if (ch == 10 && picked == 1) {
            printw("Press the char on keyboard");
            bor = getch();
        } else if (ch == 10 && picked == 2) {
            printw("Press the char on keyboard");
            pole = getch();
        } else if (ch == 10 && picked == 3) {
            if (main_hero_left == "<" && main_hero_right == ">" && bor == "|" && pole == " "){
                attron(COLOR_PAIR(4));
                printw("Already reseted!");
                attroff(COLOR_PAIR(4));
                int d = getch();
            } else {
                attron(COLOR_PAIR(1));
                printw("Reset completed!");
                attroff(COLOR_PAIR(1));

                int d = getch();

                main_hero_left = "<";
                main_hero_right = ">";
                bor = "|";
                pole = " ";
            }
            
        } else if (ch == 10 && picked == 4) {
            break;
        }

        main_text = "Change character skin";
        border_text = "Change border skin";
        pole_text = "Change pole skin";
        default_text = "Reset to default settings";
        exit_text = "Back to menu";
    }
}
void print_3d_text(int row, int col, const char* text, int num) {
    attron(COLOR_PAIR(2));
    attron(A_BOLD);
    for (int i = 0; i < num; i++) {
        mvprintw(row + i, col + i, text);
    }
    attroff(COLOR_PAIR(2));
    attroff(A_BOLD);
}
void menu() {
    string game_text = "Game";
    string settings_text = "Settings";
    string exit_text = "Exit";

    string main_hero_left = "<";
    string main_hero_right = ">";
    string bor = "|";
    string pole = " ";

    int score;
    
    int picked = 0;

    while (true) {
        bkgd(COLOR_PAIR(3));
        clear();

        game_text = "Game";
        settings_text = "Settings";
        exit_text = "Exit";

        if (picked == 0) {
            game_text.append(" <==");
        } else if (picked == 1) {
            settings_text.append(" <==");
        } else if (picked == 2) {
            exit_text.append(" <==");
        }

        printw("%s\n", game_text.c_str());
        printw("%s\n", settings_text.c_str());
        printw("%s\n", exit_text.c_str());
        printw("Record: %d\n", score);
        print_3d_text(0, 25, "HIGH RISERS", 24);

        int ch = getch();

        if (ch == 'w' && picked > 0) {
            picked--;
        } else if (ch == 's' && picked < 2) {
            picked++;
        }

        if (ch == 10 && picked == 0) {
            game(bor, main_hero_left, main_hero_right, pole, score);
        } else if (ch == 10 && picked == 1) {
            settings(main_hero_left, main_hero_right, bor, pole);
        } else if (ch == 10 && picked == 2) {
            break;
        }

        refresh();
    }

    endwin();
}

int main() {
    initscr();
    cbreak();
    noecho();

    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    init_pair(2, COLOR_BLACK, COLOR_CYAN);

    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    init_pair(4, COLOR_RED, COLOR_BLACK);

    menu();
}