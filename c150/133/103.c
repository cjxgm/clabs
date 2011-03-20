#include "stdio.h"
struct student
{ char num[6];
char name[8];
int score[3];
float avr;
} stu[5];
main()
{int i,j,sum;
FILE *fp;
/*input*/
for(i=0;i<5;i++)
{ printf("\n please input No. %d score:\n",i);
printf("stuNo:");
scanf("%s",stu[i].num);
printf("name:");
scanf("%s",stu[i].name);
sum=0;
for(j=0;j<3;j++)
{ printf("score %d.",j+1);
scanf("%d",&stu[i].score[j]);
sum+=stu[i].score[j];
}
stu[i].avr=sum/3.0;
}
fp=fopen("stud","w");
for(i=0;i<5;i++)
if(fwrite(&stu[i],sizeof(struct student),1,fp)!=1)
printf("file write error\n");
fclose(fp);
}

