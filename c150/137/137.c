# include <stdio.h>
# include <string.h>
# include <conio.h>

char xx[50][70];
int maxline=0;
int readdat(void);
void writedat(void);
void stror(void)
 {
    char *p,yy[70];
   int i;
   for(i=0;i<maxline;i++){
     p=strchr(xx[i],'o');
     while(p!=NULL){
     memset(yy,0,70);
     memcpy(yy,xx[i],p-xx[i]);
     strcpy(xx[i],xx[i]+(p-xx[i]+1));
     strcat(xx[i],yy);
     p=strchr(xx[i],'o');}
     }
     }
       main()
      {
	clrscr();
	if (readdat()){
	  printf("the file cann't open!\007\n");
	  return;}
	  stror();
	  writedat();
	  }
	int readdat(void)
	 { FILE *fp;
	   int i=0;
	   char *p;
	   if((fp=fopen("hk1.dat","r"))==NULL)  return 1;
	   while(fgets (xx[i],80,fp)!=NULL){
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