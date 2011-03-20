#include <stdio.h> 
#include <conio.h> 
int main() 
{ 
int i; 
char *c[]={"BLACK",   "BLUE",   "GREEN",   "CYAN",   "RED","MAGENTA", "BROWN", "LIGHTGRAY"}; 
textbackground(0);            
clrscr();                     
for(i=1; i<8; i++) { 
window(10+i*5, 5+i, 30+i*5, 15+i); 
textbackground(i);        
clrscr();                 
} getch(); return 0; 
}

