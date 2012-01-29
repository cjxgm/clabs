#include<linux/joystick.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
	int fd;
	int getnum;
	int success;
	struct js_event e;

	fd=open(argv[1],O_RDONLY);
	if (fd==-1){printf("joyport open fail!!\n");exit(0);}
	for(;;)
	{
		read(fd,&e,sizeof(struct js_event));
		printf("time:%d value:%d type:%d number:%d\n",e.time,e.value,e.type,e.number);
	}
	return 0;
}
