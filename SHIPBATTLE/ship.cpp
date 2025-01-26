#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <utility>
#include <ncurses.h>
using namespace std;

void makePolje(int rows, vector<pair<int, int>>& rowCol) {
    vector<vector<string> > polje(rows, vector<string>(rows, "."));
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dis(1, rows - 1);

    int placedZ = 0;
    while (placedZ < 4) {
        int r = dis(rng);
        int c = dis(rng);

        bool canPlace = true;
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // направления вверх, вниз, влево, вправо

        for (int i = 0; i < 4; i++) {
            int nr = r + directions[i][0];
            int nc = c + directions[i][1];
            if (nr >= 1 && nr < rows && nc >= 1 && nc < rows && polje[nr][nc] == "Z") {
                canPlace = false;
                break;
            }
        }

        if (canPlace && polje[r][c] == ".") {
            polje[r][c] = "Z";
            rowCol.push_back(make_pair(r, c));
            placedZ++;
        }
    }

    bool placedDZ = false;
    while (!placedDZ) {
        int r = dis(rng);
        int c = dis(rng);

        int directions[2][2] = {{0, 1}, {1, 0}}; // вправо или вниз
        for (int i = 0; i < 2; i++) {
            int nr = r + directions[i][0];
            int nc = c + directions[i][1];

            if (nr >= 1 && nr < rows && nc >= 1 && nc < rows && polje[r][c] == "." && polje[nr][nc] == ".") {
                bool canPlace = true;
                int neighborDirections[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
                for (int j = 0; j < 4; j++) {
                    int adj1r = r + neighborDirections[j][0];
                    int adj1c = c + neighborDirections[j][1];
                    int adj2r = nr + neighborDirections[j][0];
                    int adj2c = nc + neighborDirections[j][1];
                    if ((adj1r >= 1 && adj1r < rows && adj1c >= 1 && adj1c < rows && polje[adj1r][adj1c] == "Z") ||
                        (adj2r >= 1 && adj2r < rows && adj2c >= 1 && adj2c < rows && polje[adj2r][adj2c] == "Z")) {
                        canPlace = false;
                        break;
                    }
                }
                if (canPlace) {
                    polje[r][c] = "Z";
                    rowCol.push_back(make_pair(r, c));
                    polje[nr][nc] = "Z";
                    rowCol.push_back(make_pair(nr, nc));
                    placedDZ = true;
                    break;
                }
            }
        }
    }
}
bool shot(vector<pair<int, int>>& rowCol, int col, int row) {
    bool got = false;
    for (const auto& p : rowCol) {
        if (p.first == row && p.second == col) {
            got = true;
            break;
        }
    }
    return got;
}
void printPolje(vector<vector<string>>&polje, int rows) {
    char alph[21] = "ABCDEFGHIJKLMNOPQRST";
    for (int i = 0; i < rows - 1; i++) {
        polje[0][i + 1] = string(1, alph[i]); // буквы на верхней стороне
    }

    for (int i = 1; i < rows; i++) { // номера на левой стороне
        polje[i][0] = to_string(i);
    }

    for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                printw("%s ", polje[i][j].c_str());  
            }
            printw("\n"); 
    }  
}
void gameComp(vector<pair<int, int>>& rowCol, int rows) {
    initscr();
    cbreak();
    noecho();

    rows++;
    int destroyed = 0;
    int row = 0;
    int col = 0;

    vector<vector<string>> polje(rows, vector<string>(rows, "."));
    vector<pair<int,int>> pickedList;
    bool picked = false;

    while (destroyed < 6) {
        int ch;

        clear();
        printPolje(polje, rows);
        picked = false;
        
        ch = getch();

        if (ch == 'w' && row > 1) {
            row--;
        } else if (ch == 's' && row < rows) {
            row++;
        } else if (ch == 'a' && col > 1) {
            col--;
        } else if (ch == 'd' && col < rows) {
            col++;
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                if (polje[i][j] == "N") {
                    polje[i][j] = ".";
                }
            }
        }

        if (polje[row][col] != "0" && polje[row][col] != "X") {
            polje[row][col] = "N"; 
        }
        if (ch == 10) {
            bool got = shot(rowCol, col, row);
            if (got) {
            for (auto i: pickedList) {
                if (i.first == row && i.second == col) {
                    picked = true;
                    break;
                }
            }
            if (!picked) {
                polje[row][col] = "X";
                pickedList.push_back(make_pair(row,col));
                destroyed++;
            }
            } else {
                for (auto i: pickedList) {
                    if (i.first == row && i.second == col) {
                        picked = true;
                        break;
                    }
                }
                if (!picked) {
                    polje[row][col] = "0";
                    pickedList.push_back(make_pair(row,col));
                }
            }
        }
        refresh();
    }
}
void computer() {
    int rows;
    vector<pair<int, int>> rowCol;
    cout << "Enter the number:";
    cout << endl;
    cin >> rows;
    if (rows >= 5 && rows <= 20) {    
        makePolje(rows, rowCol);
        gameComp(rowCol, rows);
    } else { 
        computer();
    }
}

int main() {
    // computer();
    vector<pair<int, int>> rowCol;
    makePolje(5, rowCol);
    gameComp(rowCol, 5);
    return 0;
}