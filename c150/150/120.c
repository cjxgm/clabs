#define   NULL  0

#define   LEN     sizeof(struct grade)
struct  grade
   { char  no[7];				
      int  score;				
      struct  grade  *next;			
    };
struct grade *create( void )
    { struct grade *head=NULL, *new, *tail;
        int count=0;             		
        for(  ;   ;  )              		
     	  { new=(struct grade *)malloc(LEN);	

printf("Input the number of student No.%d(6 bytes): ", count+1);
scanf("%6s", new->no);
if(strcmp(new->no,"000000")==0)    
      { free(new);              	      	
         break;                    	      	       }
printf("Input the score of the student No.%d: ", count+1);
scanf("%d", &new->score);
count++;                     		
new->next=NULL;

if(count==1)  head=new;  
      else  tail->next=new;       
       tail=new;                         
     }
   return(head);
}						



