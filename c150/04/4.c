#include "stdio.h"
#include "stdlib.h"                 /*应用random函数和randomize函数/  
#include "ctype.h"                  /*应用toupper函数*/
main()
  {
    int count,                      /*猜数次数*/
        number,                     /*随机被猜数*/
        guess;                      /*所猜数字*/
    char yes='Y';
    printf("\nNow let us play the game.\n Guess the number:");
    while(toupper(yes)=='Y')
      {
         count=0;
         randomize();
         number=random(100)+1;                             /*产生随机数*/
         do{
              do{
                  printf("\nInput an integer number(1-100):");
                  scanf("%d",&guess);
                 }while(!(guess>=1&&guess<=100));
               if(guess<number)
                   printf("\nYour answer is LOW,try again.");
               if(guess>number)
                   printf("\nYour answer is HIGH,try again."); /*交互信息*/
               count++;
               if(count==15)
                  {
                     printf("\nThia is the %d times!Think it hard next!");
                     exit(0);                               /*15次退出*/
                    }
	     }while(!(guess==number));                      /*等值判断*/
           if(count<=7)
             {
               printf("\nYou have got it in %d times.\n",count);
               printf("\nCongretulations!");
             }
                else
             {
               printf("\nYou got it in %d times.\n",count);
               printf("\nI bet you can do it better");
              }
            printf("\n NEXT?(Y/N):");                         /*重新游戏*/
            scanf(" %c",&yes);
        }
}
