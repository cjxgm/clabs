 # include "stdio.h"
  main()
  {
    char str[3][60];
    char (*p)[60];
    int i,j,up,low,space,comma;
    up=0; low=0; space=0;comma=0;
    printf(" input three strings");
    for(i=0;i<3;i++)
     gets(str[i]);
     p=str;
     for(i=0;i<3;i++)
     for(j=0;j<strlen(str[i]);j++)
      {
	 if(*(*(p+i)+j)>='a'&&*(*(p+i)+j)<='z')
	   low++;
	 else if(*(*(p+i)+j)>='A'&&*(*(p+i)+j)<='Z')
	   up++;
	   else if (*(*(p+i)+j)==',')
	    comma++;
	    else if(*(*(p+i)+j)=='')
	    space++;
	    }
	   printf("low=%d up=%d space=%d  comma=%d", low,up,space,comma);
	   }