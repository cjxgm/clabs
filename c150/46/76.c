# include <graphics.h>
main()
 {
  int xcenter,ycenter,radius;
  int graphdriver=EGA,graphmode=1,x,y;

  initgraph(&graphdriver,&graphmode,"");
  cleardevice();
  xcenter=320;
  ycenter=180;
  radius=110;
  settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
  settextjustify(CENTER_TEXT,TOP_TEXT);
  outtextxy(320,6,"This is a Pie Chart");
  settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
  setfillstyle(SOLID_FILL,RED);
  pieslice(xcenter+10,ycenter-10,0,90,radius);
  settextjustify(LEFT_TEXT,BOTTOM_TEXT);
  outtextxy(410,100,"25%");
  setfillstyle(WIDE_DOT_FILL,GREEN);
  pieslice(xcenter,ycenter,90,135,radius);
  settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
  outtextxy(270,90,"12.5%");
  setfillstyle(INTERLEAVE_FILL,YELLOW);
  settextjustify(RIGHT_TEXT,CENTER_TEXT);
  pieslice(xcenter-10,ycenter,135,225,radius);
  settextjustify(RIGHT_TEXT,CENTER_TEXT);
  outtextxy(190,180,"%25");
  setfillstyle(HATCH_FILL,BLUE);
  pieslice(xcenter,ycenter,225,360,radius);
  settextjustify(LEFT_TEXT,TOP_TEXT);
  outtextxy(370,270,"37.5%");
  getch();
  closegraph();
  }