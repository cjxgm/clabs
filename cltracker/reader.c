
#include <stdio.h>

#define MAX 10

int filter(int value);

int main(void)
{
	FILE *fp = fopen("/dev/dsp", "rb");
	FILE *fpout = fopen("/dev/dsp", "wb");
	unsigned char data[MAX];
	for (;;){
		fread(data, 1, MAX, fp);
		int i;
		for (i=0; i<MAX; i++){
			printf("%d\t", data[i]);
			data[i] = filter(data[i]);
		}
		fwrite(data, 1, MAX, fp);
	}
	fclose(fpout);
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

