 #include "stdio.h"
 #include "stdlib.h"                      /*应用随机函数*/   
 main()
  {
    int i,j;
    char ch1,ch2,
         s[104],                          /*存放一副牌*/
         a[]="hdsc";                      /*定义花色*/  
    char b[13]={'A','K','Q','J','10','9','8','7','6','5','4','3','2'};
                                          /*定义牌值*/
    for(i=0;i<4;i++)                      /*生成一副新牌*/ 
      for(j=0;j<13;j++)
	{
	  s[2*j+26*i]=a[i];
	  s[2*j+26*i+1]=b[j];
	}
    randomize();                         
    for(i=0;i<52;i++)                     /*随机打乱*/
      {
	j=random(52-i);                    
	ch1=s[2*j];                       /*与最后一张牌交换花色*/
        s[2*j]=s[102-2*i];
        s[102-2*i]=ch1;
	ch2=s[2*j+1];                     /*与最后一张牌交换牌值*/
        s[2*j+1]=s[103-2*i];
        s[103-2*i]=ch2;
      }
    printf("\n\n");
    for(i=0,j=0;i<104;i+=2,j++)           /*显示打乱后新牌*/
      {
	if(!(j%13))
	   printf("\n\n\n");              /*13张牌为一组*/
	printf("  %c%c",s[i],s[i+1]);
      }
    return;
  }