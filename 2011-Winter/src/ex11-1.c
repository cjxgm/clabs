/** ex11-1.c
 * 
 **
 * eXerigumo Clanjor
 **
 * Under GPLv3 and KING Principle
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int total = 0;

// prototype
void init_resource();
void print_sentence(FILE *fp);

int main(int argc, char *argv[])
{
	init_resource();

	char key[21];
	printf("Please input a word to find: ");
	scanf("%20s", key);
	char *cp = key;
	while (*cp != 0){
		*cp = tolower(*cp);
		cp++;
	}

	FILE *fp = fopen("./data/ex11-1.in", "r");

	char word[21], pos;
	while (!feof(fp)){
		for (pos=0; pos<21; pos++){
			int ch = getc(fp);
			if (ch == EOF) break;
			if (ch == ' ' || ch == '\n' || ch == '.')
				break;
			word[pos] = tolower(ch);
		}
		if (pos == 0) continue; // Skip blank
		word[pos] = 0;

		if (strcmp(word, key) == 0)
			print_sentence(fp);
	}
	printf("Total: %d\n", total);

	return 0;
}

void init_resource()
{
	FILE *fp = fopen("./data/ex11-1.in", "w");
	fprintf(fp, "Student must have\n\na book. They should be at school. \n I have \nno pens now.\n\nBye.");
	fclose(fp);
}

void print_sentence(FILE *fp)
{
	total++;
	FILE *fpout = stdout;

	while (ftell(fp) != 0){
		fseek(fp, -1, SEEK_CUR);
		if (getc(fp) == '.') break;
		fseek(fp, -1, SEEK_CUR);
	}

	int ch;
	while ((ch = getc(fp)) == ' ' || ch == '\n');
	for(;;){
		if (ch == ' ' || ch == '\n'){
			fprintf(fpout, " ");
			while(ch == ' ' || ch == '\n')
				ch = getc(fp);
		}
		else {
			fprintf(fpout, "%c", ch);
			ch = getc(fp);
		}
		if (ch == '.')
			break;
	}
	fprintf(fpout, ".\n");

	// fclose(fpout);
}

