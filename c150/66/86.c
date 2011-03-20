#include<stdio.h>
#include<conio.h>
#define X 3
#define Y 3

int a[X][Y];
int b[X][Y];
int c[X][Y];

void matrix(int b[][X],int c[][Y]);
main()
{
    int i,j,temp;
    clrscr();
    printf("Please input int matrix b[%d][%d]\n",X,Y);
    for(i=0;i<Y;i++)
        for(j=0;j<Y;j++){
        scanf("%d",&temp);
        b[i][j]=temp;
        }
    printf("Please input int matrix c[%d][%d]\n",X,Y);
    for(i=0;i<X;i++)
        for(j=0;j<Y;j++){
        scanf("%d",&temp);
        c[i][j]=temp;
        }
    matrix(b,c);
    printf("Now print resource matrix b[%d][%d]=",X,Y);
    for(i=0;i<X;i++){
        printf("\n");
        for(j=0;j<Y;j++)
        printf("%d ",b[i][j]);
        }
    printf("\n");
    printf("Now print resource matrix c[%d][%d]=",X,Y);
    for(i=0;i<X;i++){
        printf("\n");
        for(j=0;j<Y;j++)
        printf("%d ",c[i][j]);
        }
    printf("\n");
    printf("Now printm multiply results matrix a[%d][%d]=B*C:",X,Y);
    for(i=0;i<X;i++){
        printf("\n");
        for(j=0;j<Y;j++)
        printf("%d ",a[i][j]);
    }
    getch();
    return 0;
}
/********************************************************************/
void matrix(int b[][X],int c[][Y])
{
    int i,j,k,temp;
    for(i=0;i<X;i++)
        for(j=0;j<Y;j++){
        for(k=0;k<Y;k++)
        a[i][j]+=b[i][k]*c[k][j];
    }
}

