#include <ncurses.h>

void print_3d_text(int row, int col, const char* text, int num) {
    for (int i = 0; i < num; i++) {
        mvprintw(row + i, col + i, text);
    }
}

int main() {
    initscr();          // Инициализация ncurses
    start_color();      // Включаем использование цветов

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));  // Включаем основной яркий цвет

    print_3d_text(0, 25, "HIGH RISERS", 24);

    refresh();  // Обновляем экран
    getch();    // Ожидаем нажатие клавиши
    endwin();   // Завершаем работу с ncurses

    return 0;
}
