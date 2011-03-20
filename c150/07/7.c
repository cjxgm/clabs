#include "stdio.h"
#include "ctype.h"               /*应用函数toupper*/
void move(int n,char a,char b)   /*n为移动的序数,a为源柱号,b目的柱号*/
  {
    int i=0;
    printf("Disc%d move from %c to %c;",(n+1),a,b);
    if(i++%2==0)
      printf("\n");
  }

void hanoi(int n,char a,char b,char c)
  {                   /*n移动的序数,a为源柱号,b为目的柱号,c为中间柱号*/
    if(n>0)
      {
	hanoi(n-1,a,c,b);        /*递归调用*/
	move(n-1,a,b);
	hanoi(n-1,c,b,a);
      }
   }
main()
  {
   int n;
   char ch='Y';
   printf("Now let us play the game of hanoitower\n");
   while(ch=='Y')                                     /*通用性设计*/
     {
       printf("\n Input the number of disc[2-15]:");
       scanf("%d",&n);getchar();
       printf("\n The orders of moving %d discs from A to B are:\n",n);
       hanoi(n,'A','B','C');                           /*调用函数*/
       printf("\n Continue(Y/N):");
       ch=getchar();
       getchar();
       ch=toupper(ch);
     }
  return;
}
