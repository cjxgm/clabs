/****
 * compile: gcc -o rex rex.c
 * usage:   ./rex > coo.js
 */

#include <stdio.h>
#include <regex.h>

static void proc(const char * s);
static int  read_coo(const char * s, float * x, float * y);

regex_t		coo;
regmatch_t	coog[3];
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
		  regcomp(&trs, "\\<translate\\>\\(([^,]+),([^)]+)\\)",
		  												REG_EXTENDED) +
		  regcomp(&coo, "([^ ,]+),([^ ]+)", REG_EXTENDED);
	if (err) {
		printf("Regex compilation failed. %d\n", err);
		return 1;
	}

	if (!regexec(&trs, s, 3, trsg, 0)) {
		sscanf(&s[trsg[1].rm_so], "%g,%g", &tr_x, &tr_y);
		//printf("translate: %g, %g\n", tr_x, tr_y);
	}

	printf("coo = [\n");
	while (!regexec(&rex, s, 2, g, 0)) {
		s[g[1].rm_eo] = 0;
		s += g[1].rm_so;

		// process the macthed
		proc(s);

		// prepare for next match
		s += g[1].rm_eo - g[1].rm_so + 1;
	}
	printf("[]];\n");

	regfree(&rex);
	regfree(&trs);
	regfree(&coo);

	return 0;
}

static void proc(const char * s)
{
	float pos[4][2];
	int   offset;

	printf("// %s\n", s);

	offset = read_coo(s, &pos[0][0], &pos[0][1]);
	pos[0][0] += tr_x;
	pos[0][1] += tr_y;
	if (offset == -1) {
		printf("FAILED!\n");
		return;
	}

	while (1) {
		offset = read_coo(s += offset, &pos[1][0], &pos[1][1]);
		if (offset == -1) break;
		pos[1][0] += pos[0][0];
		pos[1][1] += pos[0][1];

		offset = read_coo(s += offset, &pos[2][0], &pos[2][1]);
		if (offset == -1) break;
		pos[2][0] += pos[1][0];
		pos[2][1] += pos[1][1];

		offset = read_coo(s += offset, &pos[3][0], &pos[3][1]);
		if (offset == -1) break;
		pos[3][0] += pos[2][0];
		pos[3][1] += pos[2][1];

		printf("[ [%g, %g, 0], [%g, %g, 0], [%g, %g, 0], [%g, %g, 0] ],\n",
				pos[0][0], pos[0][1], pos[1][0], pos[1][1],
				pos[2][0], pos[2][1], pos[3][0], pos[3][1]);

		pos[0][0] = pos[3][0];
		pos[0][1] = pos[3][1];
	}
}

static int read_coo(const char * s, float * x, float * y)
{
	if (!regexec(&coo, s, 3, coog, 0)) {
		sscanf(&s[coog[1].rm_so], "%g,%g", x, y);
		return coog[2].rm_eo;
	}
	else return -1;
}

