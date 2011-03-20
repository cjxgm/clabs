#include <stdio.h>
void main()
{
char letter;
printf("please input the first letter of someday\n");
while ((letter=getch())!='Y')
{ switch (letter)
{case 'S':printf("please input second letter\n");
if((letter=getch())=='a')
printf("saturday\n");
else if ((letter=getch())=='u')
printf("sunday\n");
else printf("data error\n");
break;
case 'F':printf("friday\n");break;
case 'M':printf("monday\n");break;
case 'T':printf("please input second letter\n");
if((letter=getch())=='u')
printf("tuesday\n");
else if ((letter=getch())=='h')
printf("thursday\n");
else printf("data error\n");
break;
case 'W':printf("wednesday\n");break;
default: printf("data error\n");
}
}
}

