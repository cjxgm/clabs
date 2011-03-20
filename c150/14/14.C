main()
{
 int i,j,k,m,p;
 char xwei[5],side[10];
 char 
   ch[10]={'B','A','C','E','A','B','D','C','E','D'};/*给出逻辑信息*/
 xwei[0]='A';                                  /*围圈时固定A的位置*/ 
 for(i=1;i<5;i++)                              /*穷举建立新位置*/
   {
     xwei[i]='B';
     for(j=1;j<5;j++)
       if(j!=i)
	 {
	   xwei[j]='C';
	   for(k=1;k<5;k++)
	     if(k!=i&&k!=j)
	       {
		 xwei[k]='D';
		 xwei[10-i-j-k]='E';


		 for(p=0;p<5;p++)          /*对建立的位置建立逻辑关系*/
		  {
		    side[2*p]=xwei[(p+1)%5];      /*左边*/
		    side[2*p+1]=xwei[(p+4)%5];    /*右边*/
		  }
		for(p=0,m=0;p<10;p+=2)         /*对新位置进行逻辑判断*/
		  {
		    if((side[p]==ch[p])+(side[p+1]==ch[p+1])==1)
		    m++;
		  }
	       if(m==5)
		 {
		   printf("\nTHe order is:");  /*符合条件则输出*/
		   for(p=0;p<5;p++)
		     {
		       printf("%2d %c",(p+1),xwei[p]);

		     }
		  }
	     }
	}

    }
  return;
}