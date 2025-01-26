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

int main() {
    int rows = 20;
    int columns = 10;
    int start_row = 10;
    
    string bor = "I";
    string main_hero = "X";
    string pole = " ";

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE); 

    int direction = 2;
    int col = 1;

    vector<int> side;

    vector<vector<string>> polje(rows, vector<string>(columns, "."));

    for (int i = 0; i < rows; i++) {
        make_border(side);
    }
    side[start_row] = 2;

    int floor = 0;

    bool has_border_left;
    bool has_border_right;
    bool has_two_borders;

    int left_side = 0;
    int right_side = 0;
     
    // direction(1) == left, direction(2) == right;
    while (true) {
        int ch;

        napms(100);

        clear(); 

        ch = getch();

        main_hero = "o";
        if (ch == 'w') {
            make_border_game(side);
            main_hero = "O";
            floor++;
        }

        for (int i = 0; i < rows; i++) { // print polje
            for (int j = 0; j < columns; j++) {
                printw("%s ", polje[i][j].c_str());  
            }
            printw("\n"); 
        }
        for (int i = 0; i < rows; i++) { // whole polje
            for (int j = 0; j < columns; j++) {
                polje[i][j] = pole;
            }
        }

        for (int i = 0; i < rows; i++) { // borders
            if (side[i] == 0) {
                polje[i][0] = bor;
                left_side++;
            } else if (side[i] == 1){
                polje[i][columns - 1] = bor;
                right_side++;
            } else if (side[i] == 2){
                polje[i][0] = bor;
                polje[i][columns - 1] = bor;
                right_side = 0;
                left_side = 0;
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
            col--;
        } else if (direction == 2) {
            col++;
        }

        polje[start_row][col] = main_hero; 

        // printw("col: %d, direction: %d\n", col, direction);
        printw("Score: %d\n", floor);
        if (floor % 10 == 0) {
            printw("(#####  %d  #####)", floor);
        }

        refresh(); 
    }
    endwin();
}