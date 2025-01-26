#include <ncurses.h>
#include <string>
#include <vector>
#include <random>
using namespace std;

void make_snak(int& snakR, int& snakC, int rows, vector<pair<int,int>>& rowCol) {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dis(0, rows - 1);

    snakR = dis(rng);
    snakC = dis(rng);
    for (const auto& p : rowCol) {
        if (p.first == snakR && p.second == snakC) {
            make_snak(snakR, snakC, rows, rowCol);
        }
    }
}

int main() {
    int back = 1;
    int rows = 15;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // Неблокирующий ввод
    keypad(stdscr, TRUE); // Включить управление клавишами-стрелками

    vector<vector<string>> polje(rows, vector<string>(rows, "."));
    vector<pair<int,int>> rowCol;

    int row = 0;
    int col = 0;
    int prevRow;
    int prevCol;

    int difficult = 200;
    vector<int> times;
    int diffName = 0;

    bool gotSnack = true;
    int snakR;
    int snakC;
    bool duplicate = false;

    bool hit = false;

    int direction = 4;
    // 1- forward, 2- back, 3- left, 4- right

    while (true) {
        napms(difficult);

        if (rowCol.size() == rows * rows) {
            break;
        }
        if (gotSnack) {
            make_snak(snakR, snakC, rows, rowCol);
            gotSnack = false;
        } 

        clear(); 

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                printw("%s ", polje[i][j].c_str());  
            }
            printw("\n"); 
        }

        int ch = getch();
        if (ch == 'w' && direction != 2) direction = 1;
        else if (ch == 's' && direction != 1) direction = 2;
        else if (ch == 'a' && direction != 4) direction = 3; 
        else if (ch == 'd' && direction != 3) direction = 4;

        if (direction == 1 && row > 0) {
            row--;
        } else if (direction == 1 && row == 0) {
            row = rows - 1;
        }

        else if (direction == 2 && row < rows - 1) {
            row++;
        } else if (direction == 2 && row == rows - 1) {
            row = 0;
        }

        else if (direction == 3 && col > 0) {
            col--;
        } else if (direction == 3 && col == 0) {
            col = rows - 1;
        }
        else if (direction == 4 && col < rows - 1) {
            col++;
        } else if (direction == 4 && col == rows - 1) {
            col = 0;
        }

        duplicate = false;
        for (const auto& p : rowCol) {
            if (p.first == row && p.second == col) {
                duplicate = true;
            }
        }

        if (row == snakR && col == snakC) {
            gotSnack = true;
            back++;
        }
        if (duplicate) {
            break;
        }
        if (rowCol.size() > back) {
            rowCol.erase(rowCol.begin());
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                if (!(polje[i][j] == ".")) {
                    polje[i][j] = ".";
                }
            }
        }

        polje[snakR][snakC] = "S"; 

        for (const auto& p : rowCol) {
            polje[p.first][p.second] = "d";
        }
        if (!duplicate) {
            rowCol.push_back(make_pair(row, col));
        }
        if (back % 5 == 0) {
            bool changed = false;
            for (const auto p : times) {
                if (p == back) {
                    changed = true;
                }
            }
            if (!changed) {
                difficult -= 20;
                times.push_back(back);
                diffName = times.size();
            }
        }
        
        polje[row][col] = "D";

        refresh();
    }

    endwin();


    return 0;
}