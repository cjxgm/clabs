    main ( )
     {
       int   x , y , z ;
       int  a[10] ;                               
    printf("input 10 numbers:\n");
      for(x=0 ;  x<10 ;  x ++)
        scanf("%d",&a[x]);                      
        printf("\n");     
for ( x=1 ; x<9 ; x++ )
         for ( y=x+1 ; y<10 ; y++)                    
         if ( a[y]< a[x] )
            {
               z= a[y] ;
               a[y] =a[x] ;
               a[x] = z ;   }                 
           printf("the sorted numbers:\n");
          for ( x=0 ;  x<10 ;  x ++) 
           printf ("%d\t" , a[x]) ;              
}
