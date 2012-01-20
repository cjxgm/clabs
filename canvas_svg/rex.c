/****
 * compile: gcc -o rex rex.c
 * usage:   ./rex > coo.js
 */

#include <stdio.h>
#include <regex.h>

static void proc(const char * s);

float		tr_x, tr_y;

int main()
{
	regex_t		rex;
	regmatch_t	g[2];
	regex_t		trs;
	regmatch_t	trsg[3];
	int			err;
	char		str[1024*1024];	// 1MB 
	char	  * s = str;

	// read in file, up to 1MB (It won't be so large for an SVG, right?)
	FILE * fp = fopen("b.svg", "r");
	fread(str, 1024, 1024, fp);
	fclose(fp);

	err = regcomp(&rex, "\\<d\\>=\"([^\"]+)\"", REG_EXTENDED) +
		  regcomp(&trs, "\\<translate\\>\\(([^,]+),([^)]+)\\)", REG_EXTENDED);
	if (err) {
		printf("Regex compilation failed. %d\n", err);
		return 1;
	}

	if (!regexec(&trs, s, 3, trsg, 0)) {
		sscanf(&s[trsg[1].rm_so], "%g,%g", &tr_x, &tr_y);
		//printf("translate: %g, %g\n", tr_x, tr_y);
	}

	printf("function draw_svg()\n{\n");
	while (!regexec(&rex, s, 2, g, 0)) {
		s[g[1].rm_eo] = 0;
		s += g[1].rm_so;

		// process the macthed
		proc(s);

		// prepare for next match
		s += g[1].rm_eo - g[1].rm_so + 1;
	}
	printf("}\n");

	regfree(&rex);
	regfree(&trs);

	return 0;
}

static void proc(const char * s)
{
	printf("\n// %s\n", s);

	float pos[4][2];
	int i = 0;
	int state = 0;	// 0: start, 1: m, 2: c
	for (; *s; s++) {
		switch (*s) {
			case 'm':
				state = 1;
				break;
			case 'c':
				state = 2;
				break;
			case 'l':
				state = 1;
				break;
			case ' ':
				if (*(s+1) == 'c') {
					state = 2;
					s+=2;
					//printf("\n\n>>> %s\n\n", s);
				}
				else if (*(s+1) == 'l' || *(s+1) == 'm' ) {
					state = 1;
					s+=2;
					//printf("\n\n>>> %s\n\n", s);
				}
				sscanf(s, "%g,%g", &pos[i][0], &pos[i][1]);
				if (state == 1) {
					if (i) {
						pos[1][0] += pos[0][0];
						pos[1][1] += pos[0][1];
						printf("\tline(%g, %g, %g, %g);\n",
								pos[0][0], pos[0][1],
								pos[1][0], pos[1][1]);
						pos[0][0] = pos[1][0];
						pos[0][1] = pos[1][1];
					} else {
						pos[0][0] += tr_x;
						pos[0][1] += tr_y;
						i = 1;
					}
				}
				else if (state == 2) {
					if (!i) {
						pos[0][0] += tr_x;
						pos[0][1] += tr_y;
					}
					if (++i == 4) {
						i = 1;
						pos[1][0] += pos[0][0];
						pos[1][1] += pos[0][1];
						pos[2][0] += pos[0][0];
						pos[2][1] += pos[0][1];
						pos[3][0] += pos[0][0];
						pos[3][1] += pos[0][1];
						printf("\tbez(%g, %g, %g, %g, %g, %g, %g, %g);\n",
								pos[0][0], pos[0][1],
								pos[1][0], pos[1][1],
								pos[2][0], pos[2][1],
								pos[3][0], pos[3][1]);
						pos[0][0] = pos[3][0];
						pos[0][1] = pos[3][1];
					}
				}
				break;
		}
	}
}

