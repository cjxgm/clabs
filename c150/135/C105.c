hanoi(int n,char x, char y, char z)
 { if(n==1)
   printf("%c->%c\n",x,z);
   else
    { hanoi(n-1,x,z,y);
    printf("%c->%c\n",x,z);
    hanoi(n-1,y,x,z);}
    }
   main()
    {
      int m;
      printf("input the number of diskes:");
      scanf("%d",&m);
      printf("The step to moving %3d diskes:",m);
      hanoi(m,'a','b','c');
      }