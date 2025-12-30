#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

int main() {
	srand(time(NULL));

	initscr();
	/* setlocale(LC_ALL, "en_US.UTF-8"); */
	cbreak();
	noecho();
	curs_set(0);

	refresh();   
	int Height = 20;
	int Width = 49;

	int snake_x = 1;
	int snake_y = 1;

	WINDOW *win = newwin(Height, Width, 5, 10);
	nodelay(win, true);

	int dx = 0;
	int dy = 0;
	int running = 1;

	int apple_x = rand() % Width - 2;
	int apple_y = rand() % Height - 2;

	int FPS = 100000;

	while (running) {
		char c = wgetch(win);
		
		snake_x += dx;
		snake_y += dy;

		switch (c) {
			case 'j':
				dy = 1;
				dx = 0;
				FPS = 150500;
				break;
			case 'k':
				dy = -1;
				dx = 0;
				FPS = 150500;
				break;
				break;
			case 'l':
				dx = 1;
				dy = 0;
				FPS = 100000;
				break;
				break;
			case 'h':
				dx = -1;
				dy = 0;
				FPS = 100000;
				break;
				break;
			case 'q':
				running = 0;
				break;
		}

		if (snake_y == apple_y && snake_x == apple_x) {
			apple_y = rand() % Height - 2;
			apple_x = rand() % Width - 2;
		}

		werase(win);
		box(win, 0, 0);

		mvwaddch(win, apple_y, apple_x, 'x');
		mvwaddch(win, snake_y, snake_x, '0');

		usleep(FPS);
	}

	endwin();
}
