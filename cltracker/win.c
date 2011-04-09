#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

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

int main()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, 1);

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLUE, COLOR_CYAN);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);

	win_t *win_main = create_win(getmaxy(stdscr), getmaxx(stdscr), 0, 0, "Clanjor Tracker");
	redraw_win(win_main);

	win_t *win1 = create_win(20, 20, 2, 2, "What?");
	win_t *win2 = create_win(20, 20, 4, 4, "What?");
	win_t *win3 = create_win(20, 20, 8, 5, "What?");

	for (;;){
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
	endwin();

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
	wattron(win, COLOR_PAIR(1));
	clear_win(win);
	wattroff(win, COLOR_PAIR(1));

	wattron(win, COLOR_PAIR(2)|A_BOLD);
	clear_line(win, 0);
	print_in_middle(win, 0, 0, 0, title);
	wattroff(win, COLOR_PAIR(2)|A_BOLD);

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

