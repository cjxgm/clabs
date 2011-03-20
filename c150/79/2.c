# include  <stdio.h>
       main( )
        {  enum fruit {apple, orange , banana , pineapple , pear };  
              enum fruit x, y, z, pri ;       
              int   n, loop ;
             n=0 ;
             for (x=apple ;x<=pear ; x++)
                for(y=apple ;y<=pear ; y++)     
                 if (x !=y)
                    {  for (z=apple ;z<=pear ; z++)
                       if ((z !=x)&&(z !=y))
                        { n=n+1;
                          printf("\n%-4d",n) ;
                   for (loop =1 ; loop<=3 ; loop++)    
                            { switch (loop)             
                              { case 1: pri=x ; break ;
                                case 2: pri=y ; break ;    
                                case 3: pri=z ; break ;             
                                 default :  break ;  }
                                switch ( pri )        
                        { case apple : printf("%-8s",  "apple") ;  break ;    
                          case orange: printf("%-8s", "orange") ;  break ;    
                          case banana : printf("%-8s", "banana") ;  break ;    
                          case pineapple : printf("%-8s","pineapple") ;  break ;    
                          case pear : printf("%-8s",  "pear") ;  break ;    
                          default:  break ;  } 
                              }
                        }
                 }
       printf("\n total: %d",n) ;
  }     
