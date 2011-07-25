/** demostr.c
 * Demolino: String
 **/
#include <GL/glut.h>
#include "demostr.h"
#include "demoutil.h"

void draw_string3(const char * str, float spacing, float line_height)
{
	PUSH;
	PUSH;
	for (; *str; str++){
		switch (*str){
			case '\t': /* TODO: Do the real job as tab does */
				glTranslatef(4 * spacing, 0, 0);
				break;
			case '\n':
				POP;
				glTranslatef(0, -line_height, 0);
				PUSH;
				break;
			default:
				draw_char(*str);
				/* NO BREAK HERE!!! */
			case ' ':
				glTranslatef(spacing, 0, 0);
		}
	}
	POP;
	POP;
}

void draw_char(char ch)
{
	/*
	if (ch == ' ' || ch == '\t' || ch == '\n')
		return;
	*/
	switch (ch){
		// ALPHA
		case 'A':
		case 'a':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, -1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			break;
		case 'B':
		case 'b':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			/* NO BREAK HERE!!! */
		case 'D':
		case 'd':
			glBegin(GL_LINE_LOOP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			break;
		case 'E':
		case 'e':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			/* NO BREAK HERE!!! */
		case 'C':
		case 'c':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case 'F':
		case 'f':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			break;
		case 'G':
		case 'g':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.0, -1.0);
			glVertex2f(+0.5, -0.5);
			glVertex2f(+0.5, +0.0);
			glVertex2f(+0.0, +0.0);
			glEnd();
			break;
		case 'H':
		case 'h':
			glBegin(GL_LINES);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			break;
		case 'I':
		case 'i':
			glBegin(GL_LINES);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(+0.0, +1.0);
			glVertex2f(+0.0, -1.0);
			glEnd();
			break;
		case 'J':
		case 'j':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			break;
		case 'K':
		case 'k':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case 'L':
		case 'l':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case 'M':
		case 'm':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, -1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.0, +1.0);
			glVertex2f(+0.0, -1.0);
			glEnd();
			break;
		case 'N':
		case 'n':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, -1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.0, +1.0);
			glVertex2f(+0.0, -1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(+0.5, +1.0);
			glEnd();
			break;
		case 'O':
		case 'o':
			draw_square(-0.5, +1.0,
						+0.5, -1.0, false);
			break;
		case 'R':
		case 'r':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			/* NO BREAK HERE!!! */
		case 'P':
		case 'p':
			draw_square(-0.5, +1.0,
						+0.5, +0.0, false);
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +0.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			break;
		case 'Q':
		case 'q':
			glBegin(GL_LINE_LOOP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.0, -1.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.0, +0.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case 'S':
		case 's':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +0.5);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(-0.5, -0.5);
			glEnd();
			break;
		case 'T':
		case 't':
			glBegin(GL_LINES);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.0, +1.0);
			glVertex2f(+0.0, -1.0);
			glEnd();
			break;
		case 'U':
		case 'u':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(+0.5, +1.0);
			glEnd();
			break;
		case 'V':
		case 'v':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.0, -1.0);
			glVertex2f(+0.5, +1.0);
			glEnd();
			break;
		case 'W':
		case 'w':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(+0.5, +1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.0, +1.0);
			glVertex2f(+0.0, -1.0);
			glEnd();
			break;
		case 'X':
		case 'x':
			glBegin(GL_LINES);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			break;
		case 'Y':
		case 'y':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glVertex2f(+0.5, +1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.0, +0.0);
			glVertex2f(+0.0, -1.0);
			glEnd();
			break;
		case 'Z':
		case 'z':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;

		// DIGIT
		case '0':
			draw_square(-0.5, +1.0,
						+0.5, -1.0, false);
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, -0.2);
			glVertex2f(+0.5, +0.2);
			glEnd();
			break;
		case '1':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.2, +0.5);
			glVertex2f(+0.3, +1.0);
			glVertex2f(+0.3, -1.0);
			glEnd();
			break;
		case '2':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, +0.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case '3':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			break;
		case '4':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case '5':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			break;
		case '6':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.0, +1.0);
			glVertex2f(-0.5, +0.5);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glVertex2f(+0.5, +0.0);
			glVertex2f(-0.5, +0.0);
			glEnd();
			break;
		case '7':
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case '8':
			glBegin(GL_LINE_LOOP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			break;
		case '9':
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.0, -1.0);
			glVertex2f(+0.5, -0.5);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, +1.0);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			break;

		// Symbol
		case '.':
			glBegin(GL_QUADS);
			glVertex2f(-0.2, -1.0);
			glVertex2f(+0.2, -1.0);
			glVertex2f(+0.2, -0.8);
			glVertex2f(-0.2, -0.8);
			glEnd();
			break;
		case '+':
			glBegin(GL_LINES);
			glVertex2f(+0.0, +0.5);
			glVertex2f(+0.0, -0.5);
			glEnd();
			/* NO BREAK HERE! */
		case '-':
			glBegin(GL_LINES);
			glVertex2f(-0.5, +0.0);
			glVertex2f(+0.5, +0.0);
			glEnd();
			break;

		// Unknown char
		default:
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.5, +1.0);
			glVertex2f(+0.5, -1.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex2f(+0.5, +1.0);
			glVertex2f(-0.5, -1.0);
			glEnd();
			draw_square(-0.5, +1.0,
						+0.5, -1.0, false);
	}
}
