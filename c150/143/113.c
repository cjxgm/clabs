# include <stdio.h>
# include <string.h>
# include <conio.h>

char xx[50][70];
int maxline=0;
int readdat(void);
void writedat(void);
void charD(void)
 {
    char ch;
   int i,j,k;
   for(i=0;i<maxline;i++){
     for(j=0;j<strlen(xx[i]);j++){
	for(k=j;k<strlen(xx[i]);k++) {
	if (xx[i][j]<xx[i][k])
	{ ch=xx[i][k];
	  xx[i][k]=xx[i][j];
	  xx[i][j]=ch;}}}}}

  void   main()
      {
	clrscr();
	if (readdat()){
	  printf("the file cann't open!\007\n");
	  return;}
	  charD();
	  writedat();
	  }
	int readdat(void)
	 { FILE *fp;
	   int i=0;
	   char *p;
	   if((fp=fopen("hk.dat","r"))==NULL)  return 1;
	   while(fgets[xx[i],80,fp)!=NULL){
	   p=strchr(xx[i],'\n');
	   if(p) *p=0
	   i++ ;}
	   maxline=i;
	   fclose(fp);
	   return 0;
	   }
	   void writedat(void)
	    { FILE *fp;
	      int i;
	      clrscr();
	      fp=fopen("my3.dat","w");
	      for(i=0;i<maxline; i++){
		printf("%s\n",xx[i]);
		fprintf(fp,"%s\n",xx[i]);
		}
		  fclose(fp);
	      }
