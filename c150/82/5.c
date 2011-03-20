# include "stdio.h"
    # include "string.h"             
   main( )
    {
       char  a[3][18] , s[18] ;    
       gets (a[0]) ;
       gets (a[1]) ;
       gets ( a[2]) ;
       if (strcmp (a[0] ,a[1] )>0)
          {   strcpy (s , a[0] );
              strcpy (a[0] ,a[1]) ;
              strcpy (a[1] , s) ;   }     
       if (strcmp (a[0] ,a[2] )>0)
          {   strcpy (s , a[0] );
              strcpy (a[0] ,a[2]) ;
              strcpy (a[2] , s) ;   }     
       if (strcmp (a[1] ,a[2] )>0)
          {   strcpy (s , a[1] );
              strcpy (a[1] ,a[2]) ;
              strcpy (a[2] , s) ;   }     
      printf("\n%s\n%s\n%s\n", a[0] ,a[1], a[2] ) ;
  }
