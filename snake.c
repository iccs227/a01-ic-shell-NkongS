#include "snake.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10

void set_raw_mode(int enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void play_snake() {
    int x = WIDTH / 2, y = HEIGHT / 2;
    int food_x = rand() % WIDTH, food_y = rand() % HEIGHT;
    int score = 0;
    char dir = 'd';
    int running = 1;

    set_raw_mode(1);
    printf("\033[2J");

    while (running) {
        printf("\033[H");
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (i == y && j == x)
                    printf("O");
                else if (i == food_y && j == food_x)
                    printf("*");
                else
                    printf(" ");
            }
            printf("\n");
        }
        printf("Score: %d (w/a/s/d to move, q to quit)\n", score);

        usleep(150000);
        if (kbhit()) {
            char c = getchar();
            if (c == 'w' || c == 'a' || c == 's' || c == 'd')
                dir = c;
            if (c == 'q') running = 0;
        }

        if (dir == 'w') y--;
        if (dir == 's') y++;
        if (dir == 'a') x--;
        if (dir == 'd') x++;

        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) running = 0;
        if (x == food_x && y == food_y) {
            score++;
            food_x = rand() % WIDTH;
            food_y = rand() % HEIGHT;
        }
    }
    set_raw_mode(0);
    printf("\nGame over! Final score: %d\n", score);
}