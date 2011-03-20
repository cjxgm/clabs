# include <stdio.h>
# include <string.h>
# include <conio.h>
# include <ctype.h>

char xx[50][70];
int maxline=0;
int readdat(void);
void writedat(void);
void strol(void)
 {
    char word[21],yy[80],zz[80],*p;
   int i,j;
   for(i=0;i<maxline;i++){
     p=xx[i];
     j=0;
     memset(word,0,21);
     memset(yy,0,80);
     while(*p){
       if(isalpha(*p)){
       word[j++]=*p++;
       if(*p) continue;}
       strcpy(zz,yy);
       sprintf(yy,"%s%s",word,zz);
       j=0;
       memset(word,0,21);
       while(*p&&(!isalpha(*p)))
       p++;}
       strcpy(xx[i],yy);
     }
     }
       main()
      {
	clrscr();
	if (readdat()){
	  printf("the file cann't open!\007\n");
	  return;}
	  strol();
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