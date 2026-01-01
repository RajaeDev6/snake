#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define CELL 10
#define MAX_SNAKE_SIZE (CELL * CELL)
#define INITIAL_HEIGHT 20
#define INITIAL_WIDTH 50
#define INITIAL_SNAKE_X 2
#define INITIAL_SNAKE_Y 2
#define INITIAL_SPEED 120000  // microseconds per frame
#define BORDER 1             // border width

typedef struct {
    int x;
    int y;
} SnakeSegment;

typedef struct {
	int x;
	int y;
} Apple;

void InitGame() {
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    refresh();

}

WINDOW *InitWin(int height, int width, int y, int x) {
    WINDOW *win = newwin(height, width, y, x);
    nodelay(win, true);
    keypad(win, true);
	return win;
}

void RandomAppleSpawn(Apple *a, int width, int height) {
	a->x = (rand() % (width - 2 * BORDER)) + BORDER;
	a->y = (rand() % (height - 2 * BORDER)) + BORDER;
}

void RespawnApple(Apple *apple, SnakeSegment *snake, int snake_len, int width, int height) {

	int conflict;
	do {
		conflict = 0;
		RandomAppleSpawn(apple, width, height);
		for (int i = 0; i < snake_len; i++) {
			if (snake[i].x == apple->x && snake[i].y == apple->y) {
                conflict = 1;
                break;
            }
		}
	}
	while (conflict);

}

void DrawApple(WINDOW* win, Apple apple){
    mvwaddch(win, apple.y, apple.x, 'x');
}


int AppleCollision(Apple *apple, SnakeSegment *snake) {
	return snake[0].x == apple->x && snake[0].y == apple->y;
}


int BorderCollision(SnakeSegment *snake, int width, int height) {
	if (snake[0].x < BORDER || snake[0].x >= width - BORDER ||
		snake[0].y < BORDER || snake[0].y >= height - BORDER) {
		return 1;
	}
	return 0;
}

int SelfCollision(SnakeSegment *snake, int snake_len) {
	for (int i = 1; i < snake_len; i++) {
		if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
			return 1;
		}
	}
	return 0;
}

void UpdateSnakePos(SnakeSegment *snake, int snake_len, int dx, int dy) {

	for (int i = snake_len - 1; i > 0; i--) {
		snake[i] = snake[i - 1];
	}

	// move head
	snake[0].x += dx;
	snake[0].y += dy;
}

void GrowSnake(int *snake_len) {
	if (*snake_len < MAX_SNAKE_SIZE) {
		(*snake_len)++;
	}
}

void DrawSnake(WINDOW* win, SnakeSegment *snake, int snake_len) {

	for (int i = 0; i < snake_len; i++) {
		mvwaddch(win, snake[i].y, snake[i].x, '0');
	}
}

int main() {

	InitGame();

    const int height = INITIAL_HEIGHT;
    const int width = INITIAL_WIDTH;

	WINDOW *win = InitWin(height, width, 6, 40);

    // snake setup
    SnakeSegment snake[MAX_SNAKE_SIZE];
    int snake_len = 1;
    snake[0].x = INITIAL_SNAKE_X;
    snake[0].y = INITIAL_SNAKE_Y;

    int dx = 1;  // initial movement to the right
    int dy = 0;

    // Apple setup
	Apple apple;
	RandomAppleSpawn(&apple,width, height);

    int running = 1;
    const useconds_t FPS = INITIAL_SPEED;


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

		UpdateSnakePos(snake, snake_len, dx, dy);

		// check collision with borders
		if (BorderCollision(snake, width, height)) break;

        // check collision with self
		if (SelfCollision(snake, snake_len)) running = 0;

        // check apple collision

		if (AppleCollision(&apple, snake)) {
			GrowSnake(&snake_len);
			RespawnApple(&apple, snake, snake_len, width, height);
		}

        // Update 
        werase(win);
        box(win, 0, 0);

		DrawApple(win, apple);
		DrawSnake(win, snake, snake_len);

        wrefresh(win);
        usleep(FPS);
    }

    endwin();
    printf("Game Over! Your score: %d\n", snake_len - 1);
    return 0;
}

