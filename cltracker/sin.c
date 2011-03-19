
#include <stdio.h>
#include <math.h>

#define MAX 256

int filter(int value);

int main(void)
{
	FILE *fp = fopen("/dev/dsp", "wb");
	unsigned char data[MAX];
	int i;
	float a = 0;
	int times = 0;
	float fac = 1;
	for (;;){
		if (times == 10){
			fac+=1;
			times = 0;
		}
		for (i=0; i<MAX; a+=0.1, i++)
			data[i] = filter((sin(a/fac)+1)/2 * 128);
		fwrite(data, 1, MAX, fp);
		times++;
	}
	fclose(fp);
	return 0;
}

int filter(int value)
{
	/* Envlope
	\
	 \
	  \            /\
	   \*___/\____/  \___/\__*
	*/
	static float data[] = {1, 0.75, 0.5, 0.25, 0, 0.25, 0,
						0.25, 0.5, 0.25, 0.25, -1};
	static int i = 0;
	value *= data[i++];
	// printf("%d\t", value);
	if (data[i] == -1) i = 4;
	return value;
}

