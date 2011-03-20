#  define  N  5           
main( )
 {
    int   x  , num[N+1]  ;
    float   st[N+1] , ave , sum=0;      
    for (x=1 ; x<=N ; x++)
      scanf ("%d%f" , &num[x] , &st[x] )
    for (x=1 ; x<=N ; x++)
      sum=sum + st[x] ;
     ave =sum/N ;                   
   printf ( " average  is : %f\n" , ave );
   for (x=1 ; x<=N ; x++)
     if ( st[x] >= ave)
     printf ( "%d\t%f\n" , num[x] , st[x] ) 
}
