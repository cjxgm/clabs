
# include <stdio.h>
# include <string.h>
# include <conio.h>

char xx[50][70];
int maxline=0;
int readdat(void);
void writedat(void);
void strc(void)
 {
    char ch;
   int i,k;
   for(i=0;i<maxline;i++)
     for(k=0;k<strlen(xx[i]);k++){
     ch=xx[i][k];
     ch<<=4;
     if(ch<=32||ch>100) ch=xx[i][k];
     else  ch+=xx[i][k];
     xx[i][k]=ch;}
     }
       main()
      {
	clrscr();
	if (readdat()){
	  printf("the file cann't open!\007\n");
	  return;}
	  strc();
	  writedat();
	  }
	int readdat(void)
	 { FILE *fp;
	   int i=0;
	   char *p;
	   if((fp=fopen("hk1.dat","r"))==NULL)  return 1;
	   while(fgets (xx[i],70,fp)!=NULL){
	   p=strchr(xx[i],'\n');
	   if(p) *p=0;
	   i++ ;}
	   maxline=i;
	   fclose(fp);
	   return 0;
	   }
	   void writedat(void)
	    { FILE *fp;
	      int i;
	      clrscr();
	      fp=fopen("my4.dat","w");
	      for(i=0;i<maxline; i++){
		printf("%s\n",xx[i]);
		fprintf(fp,"%s\n",xx[i]);
		}
		  fclose(fp);
	      }
