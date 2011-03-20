    main( )
    {   int  a[11] , l , h , m , y , x , *p ;
        for ( y=1 ; y<=10 ; y++)
         scanf ("%d" , &a[y] ) ;
       scanf ( "%d" , &x) ;
         l=1 ; h=10 ; p=a ;
       while (l<=h)
         {  m= (l+h)/2 ;
	    if (x==*(p+m) )   break ;
            else if (x>*(p+m) )   l=m+1 ;
            else   h=m - 1 ;
         }
      if ( l<= h)
       printf ("%d is the position %d\n",x,m);
     else
       printf (" there is no %d\n" ,x );
  }
