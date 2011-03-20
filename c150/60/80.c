# include <graphics.h>
main(){
int orgData[100][100],resData[100][100];
int nCount,nRows,nCols,i,j,times;
int GraphDriver=DETECT,GraphMode;
for (i=0;i<100;i++)
for (j=0;j<100;j++) orgData[i][j]=1;
initgraph(&GraphDriver,&GraphMode,"");
setcolor(WHITE);
rectangle(270,190,370,290); 
for (times=1;times<200;times++){
for (nRows=1;nRows<99;nRows++) {
for (nCols=1;nCols<99;nCols++){
nCount=orgData[nRows-1][nCols-1]+orgData[nRows-1][nCols]
+orgData[nRows+1][nCols-1]+orgData[nRows][nCols-1]
+orgData[nRows][nCols-1]+orgData[nRows+1][nCols-1]
+orgData[nRows-1][nCols]+orgData[nRows+1][nCols-1];
switch(nCount){
case 3: putpixel(nCols+210,120+nRows,BLACK);
resData[nRows][nCols]=1;break;
case 2: resData[nRows][nCols]=orgData[nRows][nCols];break;
default:resData[nRows][nCols]=0;
putpixel(nCols+210,120+nRows,WHITE);
}}}for (i=1;i<99;i++)
for (j=1;j<99;j++) orgData[i][j]=resData[i][j];
getch();}}

