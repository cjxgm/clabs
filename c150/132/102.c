
# include <stdio.h>
# include <conio.h>
# define MAX 100
float xx[MAX];
int n=0;
double aver=0.0;
double averint =0.0;
double averdec=0.0;
int readdat(void);
void writedat(void);
void calvalue(void)
 {
   int i;
   long val;
   for(i=0;i<n;i++)
    {
     aver+=xx[i];
     val=(long)xx[i];
     averint+=(double)val;
     averdec+=xx[i]-val;
     }
     aver/=(double)n;
     averint/=(double)n;
     averdec/=(double)n;
     }
     main()
      {
	clrscr();
	if (readdat()){
	  printf("the file cann't open!\007\n");
	  return;}
	  calvalue();
	  printf("the file have %d numbers\n",n);
	  printf("average is %.2lf\n",aver);
	  printf("the  averint is %.2lf\n",averint);
	  printf("the  averdec is %.2lf\n",averdec);
	  writedat();
	  }
	int readdat(void)
	 { FILE *fp;
	   int j;
	   if((fp=fopen("in.dat","r"))==NULL)  return 1;
	   while(!feof(fp)){
	   fscanf(fp,"%f",&xx[n]);
	   if(xx[n]>0.001)
	   n++ ;}
	   fclose(fp);
	   return 0;
	   }
	   void writedat(void)
	    { FILE *fp;
	      fp=fopen("out10.dat","w");
	      fprintf(fp,"%d\n%.2lf\n%.2lf\n%.2lf\n",n,aver,averint,averdec);
	      fclose(fp);
	      }
