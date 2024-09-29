#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void start_matrix(int ***matrix, int n, int m);
void output_matrix(int **matrix, int n, int m);
void input_matrix(int ***matrix, int n, int m);
void game(int ***matrix, int n, int m);

int main() {
    int **matrix = NULL, n = 25, m = 80, t = 100000;
    char key = 'a';
    start_matrix(&matrix, n, m);
    input_matrix(&matrix, n, m);
    if (freopen("/dev/tty", "r", stdin) == NULL) exit(EXIT_FAILURE);
    initscr();
    nodelay(stdscr, TRUE);
    while (key != 'q') {
        clear();
        usleep(t);
        output_matrix(matrix, n, m);
        game(&matrix, n, m);
        key = getch();
        if (key == '+') t -= 10000;
        if (key == '-') t += 10000;
    }
    endwin();
    free(matrix);
    return 0;
}

void input_matrix(int ***matrix, int n, int m) {
    char c;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            scanf("%c", &c);
            if (c == '.') (*matrix)[i][j] = 0;
            if (c == 'x') (*matrix)[i][j] = 1;
        }
}

void output_matrix(int **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (j == m - 1) {
                if (matrix[i][j] == 0)
                    printw("%c\n", ' ');
                else
                    printw("%c\n", 'x');
            } else {
                if (matrix[i][j] == 0)
                    printw("%c", ' ');
                else
                    printw("%c", 'x');
            }
        }
    }
}

void start_matrix(int ***matrix, int n, int m) {
    (*matrix) = malloc(m * n * sizeof(int) + n * sizeof(int *));
    int *ptr = (int *)((*matrix) + n);
    for (int i = 0; i < n; i++) (*matrix)[i] = ptr + m * i;
}

void game(int ***matrix, int n, int m) {
    int **tmp = NULL;
    start_matrix(&tmp, n, m);
    int sum;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int a = i - 1, b = j - 1, c = i + 1, d = j + 1;
            if (i == 0) a = n - 1;
            if (j == 0) b = m - 1;
            if (i == n - 1) c = 0;
            if (j == m - 1) d = 0;
            sum = ((*matrix)[a][b] + (*matrix)[a][j] + (*matrix)[a][d] + (*matrix)[i][b] + (*matrix)[i][d] +
                   (*matrix)[c][b] + (*matrix)[c][j] + (*matrix)[c][d]);
            if ((*matrix)[i][j] == 1) {
                if ((sum == 2) || (sum == 3))
                    tmp[i][j] = 1;
                else
                    tmp[i][j] = 0;
            } else {
                if (sum == 3)
                    tmp[i][j] = 1;
                else
                    tmp[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            (*matrix)[i][j] = tmp[i][j];
        }
    }
    free(tmp);
}