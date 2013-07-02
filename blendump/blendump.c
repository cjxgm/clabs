#if 0
gcc -Wall -Ofast -march=native -o blendump $0 || exit 1
./blendump factory.blend
rm -f blendump
exit 0
#endif

// compile and run: sh blendump.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#define CLR_NAME		"\e[0;32m"
#define CLR_VALUE		"\e[1;32m"
#define CLR_NAME_SUB	"\e[0;33m"
#define CLR_VALUE_SUB	"\e[1;33m"
#define CLR_NAME_ERR	"\e[0;31m"
#define CLR_VALUE_ERR	"\e[1;31m"
#define CLR_NAME_WARN	"\e[0;35m"
#define CLR_VALUE_WARN	"\e[1;35m"
#define CLR_NAME_PR		"\e[0;36m"
#define CLR_VALUE_PR	"\e[1;36m"
#define CLR_NAME_INFO	"\e[0;37m"
#define CLR_VALUE_INFO	"\e[1;37m"
#define CLR_RESET		"\e[0m"
#define EOL				CLR_RESET"\n"

#define CREATE(type, var)	type * var = malloc(sizeof(type))
#define READ(var, fp)		fread(&(var), sizeof(var), 1, fp);



// file block
typedef struct Block
{
	char   code[4];
	size_t size;
	void * oldptr;
	size_t sdna_id;
	size_t nstruct;
}
Block;

typedef struct FileBlock
{
	Block block;
	size_t pos;
	struct FileBlock * next;
}
FileBlock;

FileBlock * file_blocks;
size_t nblock;

FileBlock * FB_find(const char code[4], size_t nth);
FileBlock * FB_find_by_id(size_t id);
FileBlock * FB_find_and_goto(const char code[4], size_t nth, FILE * fp);




// sDNA
typedef struct Struct
{
	short type;
	short nfield;
	short * field_types;
	short * field_names;
}
Struct;
const char ** DNA_names;
const char ** DNA_types;
      short * DNA_tlens;
	 Struct * DNA_strcs;
size_t DNA_nname;
size_t DNA_ntype;
size_t DNA_nstrc;




static void error(const char * msg);
static void warn (const char * msg);
static void read_ztstr(char str[], size_t size, FILE * fp);




int main(int argc, const char * argv[])
{
	if (argc != 2) error("parameter error.");

	FILE * fp = fopen(argv[1], "r");
	if (!fp) error("unable to open file.");

	//---------------- dump the header
	char header[12];
	READ(header, fp);
	printf(CLR_NAME"HEADER: "CLR_VALUE"%.12s"EOL, header);

	if (strncmp(header, "BLENDER", 7)) error("not a blender file.");
	printf("\t"CLR_NAME_SUB"identifier:   "CLR_VALUE_SUB"%.7s"EOL, header);
	printf("\t"CLR_NAME_SUB"pointer size: "CLR_VALUE_SUB"%d bits"EOL,
			(header[7] == '_' ? 32 : 64));
	printf("\t"CLR_NAME_SUB"endianness:   "CLR_VALUE_SUB"%s endian"EOL,
			(header[8] == 'v' ? "little" : "big"));
	printf("\t"CLR_NAME_SUB"version:      "CLR_VALUE_SUB"%c.%.2s"EOL,
			header[9], &header[10]);
	if (header[7] != '_') error("only support 32 bits.");
	if (header[8] != 'v') error("only support little endian.");
	warn("DATA blocks are represented as dot(.).");

	printf(CLR_VALUE_INFO"press RETURN to continue...");
	getchar();

	//---------------- read and dump file block's headers
	while (1) {
		CREATE(FileBlock, fblock);
		READ(fblock->block, fp);
		fblock->pos  = ftell(fp);
		fblock->next = file_blocks;
		file_blocks  = fblock;

		Block * b = &fblock->block;
		if (strncmp(b->code, "DATA", 4)) {
			printf("\n"CLR_NAME"BLOCK HEADER: "CLR_VALUE"[%d] at %Xh"EOL,
					nblock, fblock->pos);
			printf("\t"CLR_NAME_SUB"code:    "CLR_VALUE_SUB"%.4s"EOL, b->code);
			printf("\t"CLR_NAME_SUB"size:    "CLR_VALUE_SUB"%Xh"EOL, b->size);
			printf("\t"CLR_NAME_SUB"old ptr: "CLR_VALUE_SUB"%p"EOL, b->oldptr);
			printf("\t"CLR_NAME_SUB"sdna id: "CLR_VALUE_SUB"%u"EOL, b->sdna_id);
			printf("\t"CLR_NAME_SUB"nstruct: "CLR_VALUE_SUB"%u"EOL, b->nstruct);
		}
		else {
			printf(".");
		}

		nblock++;

		if (!strncmp(b->code, "ENDB", 4)) break;
		fseek(fp, b->size, SEEK_CUR);
	}

	// reverse the block list so that the index is correct.
	{
		FileBlock * fblock = file_blocks;
		file_blocks = NULL;

		while (fblock) {
			FileBlock * next = fblock->next;
			fblock->next = file_blocks;
			file_blocks = fblock;
			fblock = next;
		}
	}

	printf(CLR_VALUE_INFO"press RETURN to continue...");
	getchar();

	//---------------- read and dump the sDNA
	FB_find_and_goto("DNA1", 0, fp);
	char identifier[4];
	size_t i, j;

	// header
	READ(identifier, fp);
	printf(CLR_NAME"sDNA identifier: "CLR_VALUE"%.4s"EOL, identifier);

	// names
	READ(identifier, fp);
	READ(DNA_nname, fp);
	DNA_names = calloc(sizeof(*DNA_names), DNA_nname);
	for (i=0; i<DNA_nname; i++) {
		char buf[64];
		read_ztstr(buf, 64, fp);
		DNA_names[i] = strdup(buf);
	}

	// types
	fseek(fp, 4 - (ftell(fp) & 0b11), SEEK_CUR);	// 4-byte align
	READ(identifier, fp);
	READ(DNA_ntype, fp);
	DNA_types = calloc(sizeof(*DNA_types), DNA_ntype);
	for (i=0; i<DNA_ntype; i++) {
		char buf[64];
		read_ztstr(buf, 64, fp);
		DNA_types[i] = strdup(buf);
	}

	// type lengths
	fseek(fp, 4 - (ftell(fp) & 0b11), SEEK_CUR);	// 4-byte align
	READ(identifier, fp);
	DNA_tlens = calloc(sizeof(*DNA_tlens), DNA_ntype);
	for (i=0; i<DNA_ntype; i++)
		READ(DNA_tlens[i], fp);

	// dump the above info
	printf("\t"CLR_NAME_SUB"nname: "CLR_VALUE_SUB"%u"EOL, DNA_nname);
	printf("\t"CLR_NAME_SUB"ntype: "CLR_VALUE_SUB"%u"EOL, DNA_ntype);

	printf(CLR_VALUE_INFO"press RETURN to continue...");
	getchar();

	// structures
	fseek(fp, 4 - (ftell(fp) & 0b11), SEEK_CUR);	// 4-byte align
	READ(identifier, fp);
	READ(DNA_nstrc, fp);
	DNA_strcs = calloc(sizeof(*DNA_strcs), DNA_nstrc);
	printf(CLR_NAME"sDNA structures "CLR_VALUE"(%u)"EOL, DNA_nstrc);
	for (i=0; i<DNA_nstrc; i++) {
		Struct * s = &DNA_strcs[i];
		READ(s->type, fp);
		READ(s->nfield, fp);
		s->field_types = calloc(sizeof(*s->field_types), s->nfield);
		s->field_names = calloc(sizeof(*s->field_names), s->nfield);

		printf("\t"CLR_NAME_SUB"[%u] "CLR_VALUE_SUB"%s"CLR_NAME_SUB":%hd"EOL, i,
				DNA_types[s->type],
				DNA_tlens[s->type]);
		for (j=0; j<s->nfield; j++) {
			READ(s->field_types[j], fp);
			READ(s->field_names[j], fp);
			printf("\t\t"CLR_NAME_PR"%s "CLR_VALUE_PR"%s"CLR_NAME_PR":%hd"EOL,
					DNA_types[s->field_types[j]],
					DNA_names[s->field_names[j]],
					DNA_tlens[s->field_types[j]]);
		}
		if (i % 5 == 4) {
			printf(CLR_VALUE_INFO"press RETURN to continue...");
			getchar();
		}
	}




	warn("TODO: print data structure's values.");




	fclose(fp);
	return 0;
}


static void error(const char * msg)
{
	fprintf(stderr, CLR_NAME_ERR"error: "CLR_VALUE_ERR"%s"EOL, msg);
	exit(1);
}


static void warn(const char * msg)
{
	fprintf(stderr, CLR_NAME_WARN"warning: "CLR_VALUE_WARN"%s"EOL, msg);
}


// read zero-terminated string
static void read_ztstr(char str[], size_t size, FILE * fp)
{
	if (!size) error("fuck you!");

	size_t i = 0;
	int ch;
	while ((ch = fgetc(fp)) != EOF && ch) {
		str[i++] = ch;
		if (i == size) error("buffer overflow.");
	}
	str[i] = 0;
}




FileBlock * FB_find(const char code[4], size_t nth)
{
	FileBlock * fblock = file_blocks;
	size_t n = 0;
	while (fblock) {
		Block * b = &fblock->block;
		if (!strncmp(b->code, code, 4)) {
			if (n == nth) return fblock;
			n++;
		}
		fblock = fblock->next;
	}
	return NULL;
}

FileBlock * FB_find_by_id(size_t id)
{
	FileBlock * fblock = file_blocks;
	while (id-- && fblock) fblock = fblock->next;
	return fblock;
}


FileBlock * FB_find_and_goto(const char code[4], size_t nth, FILE * fp)
{
	FileBlock * fblock = FB_find(code, nth);
	if (fblock) fseek(fp, fblock->pos, SEEK_SET);
	return fblock;
}


// vim: noet ts=4 sw=4 sts=0

