 # include <graphics.h>
 main()
 { int driver=EGA,mode=1;
 int b[]={1,3,5,2,4,3,2,1,4,5};
 int i,j,xstep=48,ystep=40;
 char buffer[10];
 initgraph (&driver,&mode,"");
 gotoxy(30,3);
 printf("%s","this is sample");
 line(40,80,40,310);
 line(40,310,540,310);
 j=310;
 settextjustify(CENTER_TEXT,CENTER_TEXT);
  for(i=0;i<=5;i++)
   {line(20,j,40,j);
   itoa(i,buffer,10);
   outtextxy(10,j,buffer);
   j-=ystep;
   }
   gotoxy(6,4);
   printf("%s","value");
   j=40;
   settextjustify(CENTER_TEXT,TOP_TEXT);
   for(i=0;i<=10;i++)
    {
      setfillstyle(i+1,i+4);
      line(j,320,j,310);
      itoa(i,buffer,10);
      outtextxy(j,330,buffer);
      if(i!=10)
       {
	 bar(j,(310-(b[i]*ystep)),j+xstep,310-1);
	 rectangle(j,(310-b[i]*ystep),j+xstep,310);
	 }
       j+=xstep;
       }
      gotoxy(j/8,22);
      printf("%s","month");
      getch();
      closegraph();
      }