# include "math.h"
 main()
  { float  a,b,c,p,x1,x2,t1,t2;
    scanf("%f%f%f",&a,&b,&c);
    if (a==0&&b==0)
      printf("unsolvable.\n");
    else if (a==0&&b!=0)
     printf("the single root is %f\n",-c/b);
     else if (a!=0)
      { p=b*b-4*a*c ;
       t1=-b/(2*a);
       t2=sqrt(fabs(p))/(2*a);
       if(p<0)
       { printf("complex  root:\n");
	 printf("%8.4f+%8.4fi\n",t1,t2);
	 printf("%8.4f-%8.4fi\n",t1,t2);
	 }
	 else
	 {
	   x1=t1+t2;
	   x2=t1-t2;
	   printf("realroot:\n");
	   printf("%8.4f  and  %8.4f\n",x1,x2);
	   }
	   }
	   }