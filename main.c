#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define CELL 10
#define MAX_SNAKE_SIZE (CELL * CELL)
#define INITIAL_HEIGHT 20
#define INITIAL_WIDTH 49
#define INITIAL_SNAKE_X 2
#define INITIAL_SNAKE_Y 2
#define INITIAL_SPEED 120000  // microseconds per frame
#define BORDER 1              // border width

typedef struct {
    int x;
    int y;
} Segment;

int main() {
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    refresh();

    const int height = INITIAL_HEIGHT;
    const int width = INITIAL_WIDTH;

    // snake setup
    Segment snake[MAX_SNAKE_SIZE];
    int snake_len = 1;
    snake[0].x = INITIAL_SNAKE_X;
    snake[0].y = INITIAL_SNAKE_Y;

    int dx = 1;  // initial movement to the right
    int dy = 0;

    // Apple setup
    int apple_x = (rand() % (width - 2 * BORDER)) + BORDER;
    int apple_y = (rand() % (height - 2 * BORDER)) + BORDER;

    int running = 1;
    const useconds_t FPS = INITIAL_SPEED;

    WINDOW *win = newwin(height, width, 5, 10);
    nodelay(win, true);
    keypad(win, true);

    while (running) {
        // Input
        int c = wgetch(win);
        switch (c) {
            case 's':  
                if (dy == 0) { dx = 0; dy = 1; }
                break;
            case 'w':  
                if (dy == 0) { dx = 0; dy = -1; }
                break;
            case 'd':  
                if (dx == 0) { dx = 1; dy = 0; }
                break;
            case 'a':  
                if (dx == 0) { dx = -1; dy = 0; }
                break;
            case 'q':
                running = 0;
                break;
        }

        // move body: from tail to head
        for (int i = snake_len - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // move head
        snake[0].x += dx;
        snake[0].y += dy;

        // check collision with borders
        if (snake[0].x < BORDER || snake[0].x >= width - BORDER ||
            snake[0].y < BORDER || snake[0].y >= height - BORDER) {
            break;
        }

        // check collision with self
        for (int i = 1; i < snake_len; i++) {
            if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
                running = 0;
            }
        }

        // check apple collision
        if (snake[0].x == apple_x && snake[0].y == apple_y) {
            if (snake_len < MAX_SNAKE_SIZE) {
                snake_len++;
            }
            // place new apple in free space
            int conflict;
            do {
                conflict = 0;
                apple_x = (rand() % (width - 2 * BORDER)) + BORDER;
                apple_y = (rand() % (height - 2 * BORDER)) + BORDER;
                for (int i = 0; i < snake_len; i++) {
                    if (snake[i].x == apple_x && snake[i].y == apple_y) {
                        conflict = 1;
                        break;
                    }
                }
            } while (conflict);
        }

        // Update 
        werase(win);
        box(win, 0, 0);

        mvwaddch(win, apple_y, apple_x, 'x');

        for (int i = 0; i < snake_len; i++) {
            mvwaddch(win, snake[i].y, snake[i].x, '0');
        }

        wrefresh(win);
        usleep(FPS);
    }

    endwin();
    printf("Game Over! Your score: %d\n", snake_len - 1);
    return 0;
}

