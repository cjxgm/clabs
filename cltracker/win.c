#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct _win {
	WINDOW *win;
	const char *title;
} win_t;

void print_in_middle(WINDOW *win, int starty, int startx,
		int width, const char *string);
void clear_line(WINDOW *win, int starty);
void clear_win(WINDOW *win);
void draw_win(WINDOW *win, const char *title);
win_t *create_win(int h, int w, int y, int x, const char *title);
void redraw_win(win_t *win);

void draw_volumn_bar(WINDOW *win, unsigned char vol);
void* volumn_thread(void*);

FILE *dspin;
unsigned char volumn = 127;

int main()
{
	printf("Trying to open /dev/dsp ...\n");
	while ((dspin = fopen("/dev/dsp", "r")) == NULL);
	printf("Done.\n");

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, 1);

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);			// Window
	init_pair(2, COLOR_BLUE, COLOR_CYAN);			// Title
	init_pair(3, COLOR_BLACK, COLOR_WHITE);			// Border

	init_pair(4, COLOR_WHITE, COLOR_BLUE);			// Volumn (Safe)
	init_pair(5, COLOR_WHITE, COLOR_YELLOW);		// Volumn (Warning)
	init_pair(6, COLOR_WHITE, COLOR_RED);			// Volumn (Noisy)

	win_t *win_main = create_win(getmaxy(stdscr), getmaxx(stdscr), 0, 0, "Clanjor Tracker");
	redraw_win(win_main);

	win_t *win1 = create_win(20, 20, 2, 2, "Window 1?");
	win_t *win2 = create_win(10, 40, 4, 4, "OK, Win2");
	win_t *win3 = create_win(12, 30, 8, 5, "So, the 3rd one.");

	WINDOW *win_vol_bar = newwin(1, 16, 
			getmaxy(stdscr)-1,	getmaxx(stdscr)-1-16);

	pthread_t th_volumn;
	pthread_create(&th_volumn, NULL, &volumn_thread, NULL);

	for (;;) {
		// Redraw volumn bar
		draw_volumn_bar(win_vol_bar, volumn);
		wrefresh(win_vol_bar);

		// Process key
		switch (getch()) {
			case 'q': goto _end;
			case ' ':
					  redraw_win(win_main);
					  break;
			case '1': 
					  redraw_win(win_main);
					  redraw_win(win1);
					  break;
			case '2': 
					  redraw_win(win_main);
					  redraw_win(win2);
					  break;
			case '3': 
					  redraw_win(win_main);
					  redraw_win(win3);
					  break;
		}
	}

_end:
	pthread_cancel(th_volumn);
	endwin();
	fclose(dspin);

	return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx,
		int width, const char *string)
{
	int length;
	float temp;

	if(width == 0)
		width = getmaxx(win);

	length = strlen(string);
	temp = (width - length)/ 2;
	startx += (int)temp;
	mvwprintw(win, starty, startx, "%s", string);
	refresh();
}

void clear_line(WINDOW *win, int starty)
{
	int temp;
	int width = getmaxx(win);
	while (width--)
		mvwprintw(win, starty, width, " ");
	refresh();
}

void clear_win(WINDOW *win)
{
	int height = getmaxy(win);
	while (height--)
		clear_line(win, height);
}

void draw_win(WINDOW *win, const char *title)
{
	wattron(win, COLOR_PAIR(1)|A_BLINK);
	clear_win(win);
	wattroff(win, COLOR_PAIR(1)|A_BLINK);

	wattron(win, COLOR_PAIR(2));
	clear_line(win, 0);
	//wattron(win, A_BOLD);
	print_in_middle(win, 0, 0, 0, title);
	//wattroff(win, A_BOLD);
	wattroff(win, COLOR_PAIR(2));

	int width = getmaxx(win);
	int height = getmaxy(win);
	int i;

	wattron(win, COLOR_PAIR(3));
	for (i=1; i<height; i++) {
		mvwprintw(win, i, 0, " ");
		mvwprintw(win, i, width-1, " ");
	}
	// Bottom
	for (i=0; i<width; i++)
		mvwprintw(win, height-1, i, " ");
	wattroff(win, COLOR_PAIR(3));
}

win_t *create_win(int h, int w, int y, int x, const char *title)
{
	win_t *win = malloc(sizeof(win_t));
	win->title = title;
	win->win = newwin(h, w, y, x);
	return win;
}

void redraw_win(win_t *win)
{
	draw_win(win->win, win->title);
	wrefresh(win->win);
}

void draw_volumn_bar(WINDOW *win, unsigned char vol)
{
	vol = (vol > 127 ? vol - 127 : 127 - vol);
	vol /= 127 / 15;
	int startx = getmaxx(win) - 16;
	int y = getmaxy(win) - 1;
	int i;

	for (i=0; i<16; i++) {
		int attr = 0;
		char ch[2] = "=";

		if (i<10) attr = COLOR_PAIR(4);
		else if (i<14) attr = COLOR_PAIR(5);
		else attr = COLOR_PAIR(6);

		if (i < vol) {
			attr |= A_BLINK | A_BOLD;
			ch[0] = '=';
		}

		wattron(win, attr);
		mvwprintw(win, y, startx + i, ch);
		wattroff(win, attr);
	}
}

void* volumn_thread(void* never_use)
{
	unsigned char vols[2048] = {0};
	for (;;) {
		char maxvol = 0xFF;
		fread(vols, 2048, 1, dspin);

		int i;
		for (i=0; i<2048; i++)
		if (vols[i] - 127 > maxvol)
			maxvol = vols[i] - 127;

		volumn = maxvol + 127;
	}
}

