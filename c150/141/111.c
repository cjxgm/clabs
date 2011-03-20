# include <stdio.h>
# include <conio.h>
# define MAX 50
float xx[MAX];
int n=0;
double sum=0.0;
double sumint =0.0;
double sumdec=0.0;
int readdat(void);
void writedat(void);
void calvalue(void)
 {
   int i;
   long val;
   for(i=0;i<n;i++)
   sum+=xx[i];

   for(i=0;i<n;i++)
    {
     val=(long)xx[i];
     if (val%2==0) {
     sumint+=val;
     sumdec+=(xx[i]-val);
     }
     }
     }
     main()
      {
	clrscr();
	if (readdat()){
	  printf("the file cann't open!\007\n");
	  return;}
	  calvalue();
	  printf("the file have %d numbers\n",n);
	  printf("sum  is %.2lf\n",sum);
	  printf("the sumint is %.2lf\n",sumint);
	  printf("the sumdec is %.2lf\n",sumdec);
	  writedat();
	  }
	int readdat(void)
	 { FILE *fp;
	   int j;
	   if((fp=fopen("my.dat","r"))==NULL)  return 1;
	   while(!feof(fp)){
	   fscanf(fp,"%f",&xx[n]);
	   if(xx[n]>0.001)
	   n++ ;}
	   fclose(fp);
	   return 0;
	   }
	   void writedat(void)
	    { FILE *fp;
	      fp=fopen("my2.dat","w");
	      fprintf(fp,"%d\n%.2lf\n%.2lf\n%.2lf\n",n,sum,sumint,sumdec);
	      fclose(fp);
	      }
