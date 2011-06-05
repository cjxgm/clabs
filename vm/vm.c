/** vm.c
 * 8086 Virtual Machine
 ** ABSOLUTELY NO WARRANTY !!!
 * Under GPLv3
 **
 * by eXerigumo Clanjor
 */

#include <stdio.h>

#define GET_BIT(D, N)	(((D) >> (N)) & 1)
#define STOP			while (1) {}

typedef unsigned short	u16;
typedef unsigned char	u8;

char * regs_name[] = {  "ax", "cx", "dx", "bx",
						"es", "cs", "ss", "ds"  };

u16	regs[8];
u8	mem[1024*1024];		// 1 megabyte
u16	ip = 0;

void dump_regs(void);

int main(void)
{
	// Read in
	FILE * fp = fopen("test", "rb");
	int ch;
	while ((ch = getc(fp)) != EOF)
		mem[ip++] = ch;
	
	ip = 0;				// Start from 0.
	regs[0] = 0x1234;		// ax = 123
	regs[4] = 23;		// es = 23

	char src, dst;
	char is_seg, is_16bit, dir;
	u16  tmp;

	while (1) {
		switch (mem[ip]>>3) {
		case 0x11:	// movrr
			is_seg		= GET_BIT(mem[ip], 2);
			dir			= GET_BIT(mem[ip], 1);
			is_16bit	= GET_BIT(mem[ip], 0);
			ip++;

			src	= (mem[ip] >> 3) & 0x7;
			dst	=  mem[ip]       & 0x7;

			src |= is_seg<<2;

			// TODO: Must it be segment regs?
			if (is_seg && dir) {
				// Swap!
				src ^= dst;
				dst ^= src;
				src ^= dst;
			}
			
			if (is_16bit)
				regs[dst] = regs[src];
			else {
				tmp = regs[src & 0x3];

				if (GET_BIT(src, 2))	tmp >>= 8;
				else					tmp &= 0xFF;

				if (GET_BIT(dst, 2))
					regs[dst & 0x3] = (regs[dst & 0x3]
							^ ((regs[dst & 3] >> 8) << 8))
						| (tmp << 8);
				else
					regs[dst & 0x3] = (regs[dst & 0x3]
							^ (regs[dst & 3] & 0xFF))
						| tmp;
			}

			ip++;
			break;

		case 0x16:	// movri (8bit)
			dst = mem[ip++] & 0x7;
			tmp = mem[ip++];

			if (GET_BIT(dst, 2))
				regs[dst & 0x3] = (regs[dst & 0x3]
						^ ((regs[dst & 3] >> 8) << 8))
					| (tmp << 8);
			else
				regs[dst & 0x3] = (regs[dst & 0x3]
						^ (regs[dst & 3] & 0xFF))
					| tmp;

			break;

		case 0x17:	// movri (16bit)
			dst = mem[ip++] & 0x7;
			tmp = (mem[ip + 1] << 8) | mem[ip];
			ip += 2;
			regs[dst] = tmp;
			break;

		case 0x12:	// nop
			dump_regs();
			fflush(stdout);

			STOP;

			ip++;
			break;
		default:
			fprintf(stderr, "Unknown opcode: %Xh\n", mem[ip]);
			ip++;
			break;
		}
	}

	return 0;
}

void dump_regs(void)
{
	int i;
	for (i=0; i<8; i++)
		printf("%s = %4.4Xh\n", regs_name[i], regs[i]);
}

