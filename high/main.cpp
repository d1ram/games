#include <ncurses.h>
#include <string>
#include <vector>
#include <random>
using namespace std;

void make_border(vector<int>& borders, vector<int>& side) {
    random_device rd;
    mt19937 rng(rd());

    uniform_int_distribution<int> dis2(0, 2);
    int s = dis2(rng);

    borders.insert(borders.begin(), k);
    side.insert(side.begin(), s);
}
void make_border_game(vector<int>& borders, vector<int>& side) {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dis(0, 1);
    int k = dis(rng);
    uniform_int_distribution<int> dis2(0, 2);
    int s = dis2(rng);

    borders.insert(borders.begin(), k); 
    borders.pop_back();    
    side.insert(side.begin(), s); 
    side.pop_back();    
}

int main() {
    int rows = 20;
    int columns = 10;
    int start_row = 10;
    
    string bor = "I";
    string main_hero = "X";

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE); 

    int direction = 2;
    int col = 1;

    vector<int> borders;
    vector<int> side;

    vector<vector<string>> polje(rows, vector<string>(columns, "."));

    for (int i = 0; i < rows; i++) {
        make_border(borders, side);
    }
    borders[start_row] = 1;
    side[start_row] = 2;

    int floor = 11;

    bool has_border_left;
    bool has_border_right;
    bool has_two_borders;

    int in_a_row = 0;
    int left_side = 0;
    int right_side = 0;
     
    // direction(1) == left, direction(2) == right;
    while (true) {
        int ch;
        has_border_left = false;
        has_border_right = false;
        has_two_borders = false;

        napms(100);

        clear(); 

        ch = getch();

        if (ch == 'w') {
            make_border_game(borders, side);
        }

        for (int i = 0; i < rows; i++) { // print polje
            for (int j = 0; j < columns; j++) {
                printw("%s ", polje[i][j].c_str());  
            }
            printw("\n"); 
        }
        for (int i = 0; i < rows; i++) { // whole polje
            for (int j = 0; j < columns; j++) {
                polje[i][j] = ".";
            }
        }

        in_a_row = 0;
        left_side = 0;
        right_side = 0;
        for (int i = 0; i < rows; i++) { // borders
            if (in_a_row == 1) {
                borders[i] = 1;
                if (side[i] == 0) {
                    polje[i][0] = bor;
                    left_side++;
                    right_side = 0;
                } else {
                    polje[i][columns - 1] = bor;
                    right_side++;
                    left_side = 0;
                }
                in_a_row = 0;
            } else if (borders[i] == 1) {
                if (left_side == 2 || right_side == 2) {
                    if (left_side == 2) {
                        side[i] = 1;
                        left_side = 0;
                        continue;
                    } else if (right_side == 2) {
                        side[i] = 0;
                        right_side = 0;
                        continue;
                    }
                }
                if (side[i] == 0) {
                    polje[i][0] = bor;
                    left_side++;
                } else if (side[i] == 1){
                    polje[i][columns - 1] = bor;
                    right_side++;
                } else {
                    polje[i][0] = bor;
                    polje[i][columns - 1] = bor;
                    right_side = 0;
                    left_side = 0;
                }
                in_a_row = 0;
            } else if (borders[i] == 0) {
                in_a_row++;
            }
        }

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

        printw("col: %d, direction: %d\n", col, direction);

        refresh(); 
    }
    endwin();
}