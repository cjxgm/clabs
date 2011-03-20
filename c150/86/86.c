main ( )
   {  long  a ,b ;                           
      int  x ;
      a=1 ;
      b=1 ;                                
      for ( x=1 ; x<=20 ; x++)                 
       { 
         printf ( "%16ld%16ld " ,a ,b ) ;        
	if (x%2==0)

	 printf ("\n") ;
         a=b +a ;
         b=a+b ;
       }
  }














  

            





