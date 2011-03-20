 # include "math.h"
 # include "string.h"
    main()
      { char ch[20];
	int i,j=1;
	double (*f)();
	printf("\n input sin or cos or tan:");
	scanf("%s",ch);
	if(!strcmp(ch,"sin")) 
         f=sin;
	else if(!strcmp(ch,"cos"))  
          f=cos;
	else if (!strcmp(ch,"tan")) 
          f=tan;
	else
	  j=0;
	  if (j==0) 
           printf("input error!");
	  else
	  for(i=0;i<5;i++)
	    printf("\n%3d%8.3f",(i+1)*10,(*f)(3.1415*(i+1)/18));
	    }