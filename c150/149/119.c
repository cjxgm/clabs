#include "stdio.h"
main(int argc, char *argv[])
    { FILE *fp;
char string[81];	
       if(argc>2)            
          {  printf("Too many parameters...\n\n");
	         printf("Usage: hk1  filename\n");
              exit(0);
          }

if(argc= =1)                      	
      { printf("Input the filename: ");
        gets(string);		
        argv[1]=(char *)malloc(strlen(string)+1);
        strcpy(argv[1],string);      }
  if ((fp=fopen(argv[1],"w"))==NULL)   	
      {  printf("can not open this file\n");
	   exit(0);
      }
    printf("Input a string: "); gets(string);	
  fputs(string, fp);				
  fclose(fp);

if ((fp=fopen(argv[1],"r"))==NULL)   	
         {  printf("can not open this file\n");
            exit(0);
         }
    fgets(string, strlen(string)+1, fp);		
printf("Output the string: "); puts(string);	
    fclose(fp);
  }

