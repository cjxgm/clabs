#include<stdio.h>/*for function of printf()*/
#include<stdlib.h>/*for function of exit()*/
#include<string.h>/*for function of strcmp()*/


void loadmous(void); 

main()
{
    loadmous();
    return 0;
}

void loadmous()
{
    char *p;
    if((p=getenv("MOUSE"))!=NULL){
        if(!strcmp(p,"YES"))
        printf("Mouse is OK\n");
    }
    else{
        printf("\n No mouse");
        exit(1);
    }
}

