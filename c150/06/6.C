 #include "stdio.h"
 #include "math.h"
 float fun(float k)                       /*定义一元函数*/
   {
     return (k*(k*(k*(k*(k+1)+2)-2)+1)-3);
   }
 main()
  {
    float xmin=0,                         /*起始最小值*/
	  xmid=0,                         /*起始最大值*/
	  xmax=0,
	  fmin=0,
	  fmid=0,
	  fmax=0;
    do{
	printf("\n Input xmin & xmax:");
	scanf("%f%f",&xmin,&xmax);        /*得到初始范围*/      
	fmin=fun(xmin);                   /*调用一元函数*/ 
	fmax=fun(xmax);
      }while(fmin*fmax>0);                /*满足条件则循环*/    
    for(;fabs(fmax-fmin)>=1e-5;)          /*设定终止条件*/
      {
	xmid=(xmin+xmax)/2;
	fmid=fun(xmid);
	if(fmid*fmin<0)                   /*根据判断结果进行迭代*/
	  {
	    xmax=xmid;
	    fmax=fmid;
	  }
	else
	  {
	    xmin=xmid;
	    fmin=fmid;
	  }
      }
    printf("\n x=%5.3f \n",xmid);
    return;
 }

