/** demostr.h
 * Demolino: String
 **/
#ifndef __DEMOSTR_H__
#define __DEMOSTR_H__

void draw_char(char ch);
void draw_string3(const char *str, float spacing, float line_height);
#define draw_string2(str, spacing) draw_string3(str, spacing, 2.5)
#define draw_string(str) draw_string2(str, 1.2)

#endif

