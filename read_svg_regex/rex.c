
#include <stdio.h>
#include <regex.h>

static void proc(const char * s);

int main()
{
	regex_t		rex;
	regmatch_t	g[2];
	regex_t		trs;
	regmatch_t	trsg[3];
	float		tr_x, tr_y;
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
		printf("translate: %g, %g\n", tr_x, tr_y);
	}

	while (!regexec(&rex, s, 2, g, 0)) {
		s[g[1].rm_eo] = 0;
		s += g[1].rm_so;

		// process the macthed
		proc(s);

		// prepare for next match
		s += g[1].rm_eo - g[1].rm_so + 1;
	}

	regfree(&rex);

	return 0;
}

static void proc(const char * s)
{
	printf("%s\n", s);
}

