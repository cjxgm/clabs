/*10-1.c*/
#define PI 3.1415926
   #define step 10
   #define R 10
   #include<time.h>
   #include<stdlib.h>
   #include<graphics.h>
   #include<conio.h>
   #include<dos.h>
   main()
   {
	  int gdriver=DETECT,gmode;
	  static int startx=5;
	  static int starty=100;
	  int maxx,l=1,n=1;
	  double dalta=20,angle;
	  int size;
	  void *image;
	  initgraph(&gdriver,&gmode,"");
	  cleardevice();
   setbkcolor(BLUE);
	  size=imagesize(startx,starty,startx+60,starty+60);
	  image=(unsigned char *)malloc(size);
	  maxx=getmaxx();
	  while(!kbhit())
	  {
		if(l==1)
		{
		  n++;
		  angle=-1*(n*step)/PI*180/R;
		  if((int)(-1*angle)%360<dalta)
			 angle-=dalta;
		  if(n>(maxx-70)/step)
		  l=0;
		}
		if(l==0)
		{
		  --n;
		  angle=-1*(n*step)/R/PI*180;
		  if((int)(-1*angle)%360<dalta)
			angle-=dalta;
		  if(n==1)l=1;
		}
		rectangle(startx+n*step,starty,startx+n*step+60,starty+40);
		pieslice(startx+n*step+15,starty+50,angle,angle-dalta,10);
		pieslice(startx+n*step+45,starty+50,angle,angle-dalta,10);
		setcolor(GREEN);
		setfillstyle(SOLID_FILL,GREEN);
		circle(startx+n*step+15,starty+50,10);
		circle(startx+n*step+45,starty+50,10);
		circle(startx+n*step+15,starty+50,3);
		circle(startx+n*step+45,starty+50,3);
		getimage(startx+n*step,starty,startx+n*step+60,starty+60,image);
		delay(100);
		putimage(startx+n*step,starty,image,XOR_PUT);
		}
   free(image);
       closegraph();
}
