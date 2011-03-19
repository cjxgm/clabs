/** ex9-1.c
 * 
 **
 * eXerigumo Clanjor
 **
 * Under GPLv3 and KING Principle
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct _pos
{
	int x, y;
} pos_t;

char *map;
#define MAP(mx, my) (map[(my)*(size.x)+(mx)])

pos_t dir[8] = {
	{1, 0},
	{1, 1},
	{0, 1},
	{-1, 1},
	{-1, 0},
	{-1, -1},
	{0, -1},
	{1, -1},
}, size, sp, ep;

#define STACK_SIZE 30
pos_t stack[STACK_SIZE];
int stack_pos = 0;
#define PUSH(x) stack[stack_pos++] = x
#define POP stack[stack_pos--]
#define HEAD stack[stack_pos]
#define PRINT_STACK \
{\
	int i;\
	for (i=0; i<stack_pos; i++)\
		printf("(%d, %d)%s", stack[i].x, stack[i].y,\
			(i==stack_pos-1?"\n":"  "));\
	printf("\n");\
}

// prototype
void init_resource();
void load_res();
void work();
int walkable(pos_t p);

int main(int argc, char *argv[])
{
	init_resource();
	load_res();

	work();

	return 0;
}

void init_resource()
{
	FILE *fp = fopen("./data/ex9-1.in", "w");
	if (fp == NULL){
		printf("f ck!\n");
		exit(1);
	}
	//          w h
	fprintf(fp, "8 8\n");
	//           S      E      // S[tart] E[nd]
	fprintf(fp, "*001101-\n");
	fprintf(fp, "10110110\n");
	fprintf(fp, "01001001\n");
	fprintf(fp, "00110101\n");
	fprintf(fp, "01000110\n");
	fprintf(fp, "01111101\n");
	fprintf(fp, "00111011\n");
	fprintf(fp, "11000000\n");
	fclose(fp);
}

void load_res()
{
	FILE *fp = fopen("./data/ex9-1.in", "r");
	if (fp == NULL){
		printf("f ck!\n");
		exit(1);
	}
	fscanf(fp, "%d %d\n", &size.x, &size.y);
	map = malloc(size.x*size.y);

	int x, y;
	for (y=0; y<size.y; y++){
		for (x=0; x<size.x; x++){
			MAP(x, y) = getc(fp);
			if (MAP(x, y) == '*'){
				sp.x = x;
				sp.y = y;
				MAP(x, y) == '0';
			} else if (MAP(x, y) == '-'){
				ep.x = x;
				ep.y = y;
				MAP(x, y) == '0';
			}
		}
		getc(fp); // == '\n'
	}

	fclose(fp);
}

void work()
{
	if (sp.x == ep.x && sp.y == ep.y){
		PUSH(sp);
		PRINT_STACK;
		POP;
		return;
	}

	int i;
	for (i=0; i<8; i++){
		PRINT_STACK;
		pos_t newpos = { sp.x+dir[i].x, sp.y+dir[i].y };
		if (walkable(newpos)){
			MAP(sp.x, sp.y) = 'P';
			PUSH(sp);
			sp = newpos;
			work();
			sp = POP;
			MAP(sp.x, sp.y) = '0';
		}
	}
}

int walkable(pos_t p)
{
	if (p.x < 0 || p.y < 0 ||
		p.x >= size.x || p.y >= size.y ||
		MAP(p.x, p.y) == '1' || MAP(p.x, p.y) == 'P')
		return 0;
	return 1;
}

