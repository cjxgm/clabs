#include "stdlib.h"
#include "stdio.h"
struct list
{ int data;
struct list *next;
};
typedef struct list node;
typedef node *link;
void main()
{ link ptr,head;
int num,i;
ptr=(link)malloc(sizeof(node));
ptr=head;
printf("please input 5 numbers==>\n");
for(i=0;i<=4;i++)
{
scanf("%d",&num);
ptr->data=num;
ptr->next=(link)malloc(sizeof(node));
if(i==4) ptr->next=NULL;
else ptr=ptr->next;
}
ptr=head;
while(ptr!=NULL)
{ printf("The value is ==>%d\n",ptr->data);
ptr=ptr->next;
}
}


