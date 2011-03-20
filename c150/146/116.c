#include "stdio.h"
main()
{ FILE *fp;
int i,j,n,ni;

char c[160],t,ch;
clrscr();
if((fp=fopen("A.dat","r"))==NULL)
{printf("file A cannot be opened\n");
exit(0);}
printf("\n A contents are :\n");
for(i=0;(ch=fgetc(fp))!=EOF;i++)
{c[i]=ch;
putchar(c[i]);
}

ni=i;
if((fp=fopen("B.dat","r"))==NULL)
{printf("file B cannot be opened\n");
exit(0);}
printf("\n B contents are :\n");
for(i=0;(ch=fgetc(fp))!=EOF;i++)
{c[i]=ch;
putchar(c[i]);
}

n=i;
for(i=0;i<n;i++)
for(j=i+1;j<n;j++)
if(c[i]>c[j])
{t=c[i];c[i]=c[j];c[j]=t;}
printf("\n C file is:\n");
fp=fopen("C.txt","w");
for(i=0;i<n;i++)
{ putc(c[i],fp);
putchar(c[i]);
}
fclose(fp);
}

