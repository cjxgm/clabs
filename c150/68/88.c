#include "stdio.h"
#include "dos.h"
main()
{
FILE *fp;
struct date d;
float sum,chm=0.0;
int len,i,j=0;
int c;
char ch[4]="",ch1[16]="",chtime[12]="",chshop[16],chmoney[8];
pp: clrscr();
sum=0.0;
gotoxy(1,1);printf("|---------------------------------------------------------------------------|");
gotoxy(1,2);printf("| money management system(C1.0) 2000.03 |");
gotoxy(1,3);printf("|---------------------------------------------------------------------------|");
gotoxy(1,4);printf("| -- money records -- | -- today cost list -- |");
gotoxy(1,5);printf("| ------------------------ |-------------------------------------|");
gotoxy(1,6);printf("| date: -------------- | |");
gotoxy(1,7);printf("| | | | |");
gotoxy(1,8);printf("| -------------- | |");
gotoxy(1,9);printf("| thgs: ------------------ | |");
gotoxy(1,10);printf("| | | | |");
gotoxy(1,11);printf("| ------------------ | |");
gotoxy(1,12);printf("| cost: ---------- | |");
gotoxy(1,13);printf("| | | | |");
gotoxy(1,14);printf("| ---------- | |");
gotoxy(1,15);printf("| | |");
gotoxy(1,16);printf("| | |");
gotoxy(1,17);printf("| | |");
gotoxy(1,18);printf("| | |");
gotoxy(1,19);printf("| | |");
gotoxy(1,20);printf("| | |");
gotoxy(1,21);printf("| | |");
gotoxy(1,22);printf("| | |");
gotoxy(1,23);printf("|---------------------------------------------------------------------------|");
i=0;
getdate(&d);
sprintf(chtime,"%4d.%02d.%02d",d.da_year,d.da_mon,d.da_day);
for(;;)
{
gotoxy(3,24);printf(" Tab __browse cost list Esc __quit");
gotoxy(13,10);printf(" ");
gotoxy(13,13);printf(" ");
gotoxy(13,7);printf("%s",chtime);
j=18;
ch[0]=getch();
if(ch[0]==27)
break;
strcpy(chshop,"");
strcpy(chmoney,"");
if(ch[0]==9)
{
mm:i=0;
fp=fopen("home.dat","r+");
gotoxy(3,24);printf(" ");
gotoxy(6,4);printf(" list records ");
gotoxy(1,5);printf("|-------------------------------------|");
gotoxy(41,4);printf(" ");
gotoxy(41,5);printf(" |");
while(fscanf(fp,"%10s%14s%f\n",chtime,chshop,&chm)!=EOF)
{ if(i==36)
{ getch();
i=0;}
if ((i%36)<17)
{ gotoxy(4,6+i);
printf(" ");
gotoxy(4,6+i);}
else
if((i%36)>16)
{ gotoxy(41,4+i-17);
printf(" ");
gotoxy(42,4+i-17);}
i++;
sum=sum+chm;
printf("%10s %-14s %6.1f\n",chtime,chshop,chm);}
gotoxy(1,23);printf("|---------------------------------------------------------------------------|");
gotoxy(1,24);printf("| |");
gotoxy(1,25);printf("|---------------------------------------------------------------------------|");
gotoxy(10,24);printf("total is %8.1f$",sum);
fclose(fp);
gotoxy(49,24);printf("press any key to.....");getch();goto pp;
}
else
{
while(ch[0]!='\r')
{ if(j<10)
{ strncat(chtime,ch,1);
j++;}
if(ch[0]==8)
{
len=strlen(chtime)-1;
if(j>15)
{ len=len+1; j=11;}
strcpy(ch1,"");
j=j-2;
strncat(ch1,chtime,len);
strcpy(chtime,"");
strncat(chtime,ch1,len-1);
gotoxy(13,7);printf(" ");}
gotoxy(13,7);printf("%s",chtime);ch[0]=getch();
if(ch[0]==9)
goto mm;
if(ch[0]==27)
exit(1);
}
gotoxy(3,24);printf(" ");
gotoxy(13,10);
j=0;
ch[0]=getch();
while(ch[0]!='\r')
{ if (j<14)
{ strncat(chshop,ch,1);
j++;}
if(ch[0]==8)
{ len=strlen(chshop)-1;
strcpy(ch1,"");
j=j-2;
strncat(ch1,chshop,len);
strcpy(chshop,"");
strncat(chshop,ch1,len-1);
gotoxy(13,10);printf(" ");}
gotoxy(13,10);printf("%s",chshop);ch[0]=getch();}
gotoxy(13,13);
j=0;
ch[0]=getch();
while(ch[0]!='\r')
{ if (j<6)
{ strncat(chmoney,ch,1);
j++;}
if(ch[0]==8)
{ len=strlen(chmoney)-1;
strcpy(ch1,"");
j=j-2;
strncat(ch1,chmoney,len);
strcpy(chmoney,"");
strncat(chmoney,ch1,len-1);
gotoxy(13,13);printf(" ");}
gotoxy(13,13);printf("%s",chmoney);ch[0]=getch();}
if((strlen(chshop)==0)||(strlen(chmoney)==0))
continue;
if((fp=fopen("home.dat","a+"))!=NULL);
fprintf(fp,"%10s%14s%6s",chtime,chshop,chmoney);
fputc('\n',fp);
fclose(fp);
i++;
gotoxy(41,5+i);
printf("%10s %-14s %-6s",chtime,chshop,chmoney);
}}}
