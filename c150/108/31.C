struct student
  {  char num[6];
     char name[8];
     int  score[4];
     float  avr;} stu[10];
  main()
   {
     int i,j,max,maxi,sum;
     float  average;
     clrscr();
     for(i=0;i<10;i++)
      { printf("\n Input %d student's score:\n",i);
	scanf("%s",stu[i].num);
	scanf("%s",stu[i].name);
	for(j=0;j<3;j++)
	scanf("%d",&stu[i].score[j]);
	}
      average=0;
      max=0;
      maxi=0;
      for(i=0;i<10;i++)
       {
	sum=0;
	for(j=0;j<3;j++)
	 sum+=stu[i].score[j];
	 stu[i].avr=sum/3.0;
	 average+=stu[i].avr;
	 if(sum>max)
	  { max=sum;
	    maxi=i;}
	    }
	    average/=3;
	    for(i=0;i<10;i++)
	     { printf("%8s,%10s",stu[i].num,stu[i].name);
	       for(j=0;j<3;j++)
	       printf("%7d",stu[i].score[j]);
	       printf("%6.2f\n",stu[i].avr);
	       }
	       printf("average is %5.2f\n",average);
	       printf("best score is %s, sum is %d",stu[maxi].name,max);
	       }