
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TPInfo
{
	unsigned long time;
	signed short y;
	unsigned short op;
}
TPInfo;

float map(float v, float vf, float vt, float df, float dt)
{
	return (v-vf) / (vt-vf) * (dt-df) + df;
}

int main(int argc, char * argv[])
{
	FILE * fp = fopen(argv[1], "r");
	TPInfo tpi;
	int x = 0;
	int y = 0;
	int last_x = 0;
	int last_y = 0;
	int moving = 0;
	int dr = 0;
	long tbegin = 0;

	while (1) {
		if (!fread(&tpi, sizeof(tpi), 1, fp)) {
			fclose(fp);
			fp = fopen(argv[1], "r");
			fread(&tpi, sizeof(tpi), 1, fp);
		}
		/*printf("%u\t  %6hd\t(%hx)\n", tpi.time,
					tpi.y, tpi.op);*/
		if (tpi.op == 0x0002) {	// x
				last_x = x;
				x = map(tpi.y, -32767, 5800, 0, 255);
		}
		else if (tpi.op == 0x0102) { // y
				last_y = y;
				y = map(tpi.y, -32767, 16000, 0, 255);
		}
		else if (tpi.op == 0x0301) { // up
			int t = time(NULL) - tbegin;
			if (!moving) {
				if (t) printf("!!! > RMB\n");
				else   printf("!!! < LMB\n");
			}
			moving = 0;
		}
		/*printf(">>> %c %4d %4d <--- %4d %4d\n",
				(moving ? 'M' : ' '), x, y, last_x, last_y);*/
		int a = abs(x-last_x) + abs(y-last_y);
		if (a > 10) {
			if (!moving) {
				dr = 0;
				if (x && y)
					tbegin = time(NULL);
			}
		}
		else {
			dr += a;
			if (dr > 10 && moving == 0) moving = 1;
			if (moving) {
				printf("!!! %+2d %+2d\n", x-last_x, y-last_y);
			}
		}
	}

	return 0;
}

