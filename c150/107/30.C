 # include "stdio.h"
 # define P  3.14
 main()
  {
   char c;
   float r,area, length;
   printf("Input one charace and one number:");
   scanf("%c%f",&c,&r);
   switch(c)
     {
      case 'a':
      case 'A': area=P*r*r;
		printf("area=%f",area);
		break;
      case 'b':
      case 'B': length=2*P*r;
		printf("length=%f",length);
		break;
      default: area=P*r*r;
		length=2*P*r;
		printf("area=%d,length=%f",area,length);
      }
     }
