/*7-2.c*/
#include"stdio.h"
#include"dos.h"
#include<conio.h>
#include<math.h>
#include<math.h>
#include<graphics.h>
unsigned long  pre_cale_y2[480];
typedef unsigned char byte;
union REGS reg;
struct SREGS inreg;
typedef struct
{
byte red;
byte grn;
byte blu;
}
rgb;

typedef rgb palette_Register[256];

void set_palette(palette_Register hue)
{
reg.x.ax=0x1012;
segread(&inreg);
inreg.es=inreg.ds;
reg.x.bx=0;
reg.x.cx=256;
reg.x.dx=(int)&hue[0];
int86x(0x10,&reg,&reg,&inreg);
}
void init_palette_2(palette_Register color)
{
int i;
for(i=0;i<36;i++)
{
color[i].red=0;
color[i].grn=0;
color[i].blu=(int)(1.8*i+0.5);
}
for(i=36;i<72;i++)
{
color[i].red=0;
color[i].grn=(int)(1.8*(i-36)+0.5);
color[i].blu=0;
}
for(i=72;i<108;i++)
{
color[i].red=0;
color[i].grn=(int)(1.8*(i-72)+0.5);
color[i].blu=(int)(1.8*(i-72)+0.5);
}
for(i=108;i<144;i++)
{
color[i].red=(int)(1.8*(i-108)+0.5);
color[i].grn=0;
color[i].blu=0;
}
for(i=144;i<180;i++)
{
color[i].red=(int)(1.8*(i-144)+0.5);
color[i].grn=0;
color[i].blu=(int)(1.8*(i-144)+0.5);
}
for(i=180;i<216;i++)
{
color[i].red=(int)(1.8*(i-180)+0.5);
color[i].grn=(int)(1.8*(i-180)+0.5);
color[i].blu=0;
}
for(i=216;i<252;i++)
{
color[i].red=(int)(1.8*(i-216)+0.5);
color[i].grn=(int)(1.8*(i-216)+0.5);
color[i].blu=(int)(1.8*(i-216)+0.5);
}
}
void precale()
{
unsigned int j;
for(j=0;j<480;j++)
{
pre_cale_y2[j]=640*j;
}
}
void plot(int x,int y,char color)
{
long L_offset;
int offset,page;
char far *address;
precale();
if((x<640)&&(y<480))
{
L_offset=pre_cale_y2[y]+x;
page=(L_offset>>16);
offset=L_offset&65535;
outportb(0x3c4,0xe);
outportb(0x3c5,(page&0xf)^0x2);
address=(char far *)(0xa0000000L+offset);
*address=color;
 }
}
void put_pix(int x,int y,char color,char inten)
{
char col;
col=((35+1)*(color-1)+inten)&255;
plot(x,y,col);
}
main()
{
int i,j;
palette_Register color;
precale();
reg.h.ah=0;
reg.h.al=0x13;
int86(0x10,&reg,&reg);
init_palette_2(color);
set_palette(color);
for(i=0;i<300;i++)
for(j=0;j<35;j++)
put_pix(i,j,3,j);
getchar();
closegraph();
}
