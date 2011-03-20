# include <stdio.h>
# include <string.h>
# include <conio.h>

char xx[50][70];
int maxline=0;
int readdat(void);
void writedat(void);
void CHA(void)
 {
    char ch;
   int i,j;
   for(i=0;i<maxline;i++){
     ch=xx[i][0];
     for(j=0;j<strlen(xx[i]);j++)
     if (j<strlen(xx[i]-1))
       xx[i][j]+=xx[i][j+1];
       xx[i][strlen(xx[i])-1]+=ch;
       strrev(xx[i]);
     }
     }
       main()
      {
	clrscr();
	if (readdat()){
	  printf("the file cann't open!\007\n");
	  return;}
	  CHA();
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