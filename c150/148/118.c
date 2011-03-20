main()
 {
   enum day { sun,mon,tue,wed,thu,fri,sat};
   enum day today;
   int n;
   printf("input today's number(0--6)");
   scanf("%d",&n);
   switch(n)
    {
     case 0: today=sun;break;
     case 1: today=mon;break;
     case 2: today=tue;break;
     case 3: today=wed;break;
     case 4: today=thu;break;
     case 5: today=fri;break;
     case 6: today=sat;break;
     }
     printf("Today is ");
     switch(today)
      {
       case sun: printf("sunday\n");break;
       case mon: printf("monday\n");break;
       case tue: printf("tuesday\n"); break;
       case wed: printf("wendesday\n");break;
       case thu: printf("thursday\n");break;
       case fri: printf("friday\n"); break;
       case sat: printf("saturday\n");break;
       }
       }