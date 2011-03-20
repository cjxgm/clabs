 #include "stdio.h"
 main()
  {
    char c;
    printf("Input one character:");
    scanf("%c",&c);
    while (c!='#')
      {
      if((c>='a')&&(c<='z'))
      printf("%c",c-32);
      else if ((c>='A')&&(c<='Z'))
      printf("%c",c+32);
      scanf("%c",&c);
      }
    }