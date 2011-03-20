#include "stdio.h"
main()
{FILE *fp;
char str[100],filename[10];
int i=0;
if((fp=fopen("test","w"))==NULL)
{ printf("cannot open the file\n");
exit(0);}
printf("please input a string:\n");
gets(str);
while(str[i]!='!')
{ if(str[i]>='a'&&str[i]<='z')
str[i]=str[i]-32;
fputc(str[i],fp);
i++;}
fclose(fp);
fp=fopen("test","r");
fgets(str,strlen(str)+1,fp);
printf("%s\n",str);
fclose(fp);
}
