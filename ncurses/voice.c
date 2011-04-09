#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define LEN(A) (sizeof(A) / sizeof(A[0]))

void print_in_middle(WINDOW *win, int starty, int startx,
		int width, char *string);
void clear_line(WINDOW *win, int starty);
void clear_win(WINDOW *win);
void draw(WINDOW *win, FILE *fpdsp, int starty);

float *sorted = NULL;
int sorted_len = 0;
unsigned char data[128];

int main()
{
	int ch;

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, 1);

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_RED, COLOR_CYAN);
	init_pair(3, COLOR_WHITE, COLOR_RED);

	FILE *fpdsp = fopen("/dev/dsp", "rb");
	sorted_len = getmaxx(stdscr) - 2;
	sorted = calloc(sizeof(float), sorted_len);

	for (;;){
		attron(COLOR_PAIR(1));
		clear_win(stdscr);
		attroff(COLOR_PAIR(1));

		attron(COLOR_PAIR(2));
		clear_line(stdscr, 0);
		print_in_middle(stdscr, 0, 0, -1, "============ VOICE ============");
		attroff(COLOR_PAIR(2));

		attron(COLOR_PAIR(1));
		clear_line(stdscr, 1);
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(3));
		draw(stdscr, fpdsp, 1);
		attroff(COLOR_PAIR(3));

		refresh();
		if (getch() == 'q') break;

		usleep((1000 * LEN(data) / 8000) * 1000);
	}

	fclose(fpdsp);
	free(sorted);

	endwin();

	return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx,
		int width, char *string)
{
	int length, x, y;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != -1)
		x = startx;
	if(starty != -1)
		y = starty;
	if(width == -1)
		width = getmaxx(win);

	length = strlen(string);
	x += (width - length) / 2;
	mvwprintw(win, y, x, "%s", string);
}

void clear_line(WINDOW *win, int starty)
{
	int temp, y;
	getyx(win, y, temp);
	if(starty != -1)
		y = starty;
	int width = getmaxx(win);
	while (width--)
		mvwprintw(win, y, width, " ");
}

void clear_win(WINDOW *win)
{
	int height = getmaxy(win);
	while (height--)
		clear_line(win, height);
}

void draw(WINDOW *win, FILE* fpdsp, int starty)
{
	int i;
	int width = getmaxx(win);
	int height = getmaxy(win);

	fread(data, sizeof(data[0]), LEN(data), fpdsp);

	for (i=0; i<sorted_len; i++){
		int j;
		int step = LEN(data) / sorted_len;
		sorted[i] = 0;
		for (j = i*step; j < (i+1)*step; j++)
			sorted[i] += (float)(data[j]-127) / step / ((2 << (sizeof(data[0]) * 8) - 1));
		j = sorted[i] * (height - 5) / 2 + height / 2 + 5;
		mvwprintw(win, j, 1 + i, " ");
	}

	float average = 0;
	for (i=0; i<LEN(data); i++) {
		data[i] -= 127;
		if (data[i] < 0) data[i] = -data[i];
		average += (float)data[i] 
				/ ((2 << (sizeof(data[0]) * 8) - 1)) / LEN(data);
	}
	i = average * width;
	while (i-- > 0)
		mvwprintw(win, starty, i, " ");
}

