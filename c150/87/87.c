main ( )
 {
   int  x,y,p,t,j;
   int  a[3][4] = {{9 ,8 ,7 ,11}, {4, 6 ,2 , 1} ,{25 , 3 ,19 , 10}};

    for (x=0 ; x<=2; x++)
     {  p=0;
        for( y=1 ;y<=3 ; y++)
         if ( a[x][y] >a[x][p])  p=y;
          t=a[x][0];
          a[x][0]=a[x][p] ;
          a[x][p]=t ;
   }
   for (x=0 ;x<=2; x++)
      { for(j=0 ; j<=3; j++)
        printf("%d\t",a[x][y]);
        printf("\n");
    }
}
