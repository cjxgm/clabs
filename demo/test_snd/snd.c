#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PI
#define PI 3.1415927
#endif

int main(void)
{
	FILE *fp = fopen("/dev/dsp", "wb");
	if (fp == NULL){
		fprintf(stderr, "Fatal opening /dev/dsp.\n");
		exit(1);
	}

	int i;
	float k = 1.0;
	i = 40;
	while(i--)
		k *= 1.059463;
	/*                  12 ___
		1.059463 ~= 1 +  ./ 2
	 */
	printf("%f\n", k);

	unsigned char data[8000];
	for (i=0; i<2048; i++)
		data[i] = sin(k * 2.0 * PI * i / 2048.0) * 127 + 127;
	fwrite(data, 1, 2048, fp);

	fclose(fp);
	return 0;
}
