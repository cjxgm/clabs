#include <stdio.h>

#define RE_ANY				0x0100
#define RE_SPLIT			0x0101
#define RE_JUMP				0x0102
#define RE_GROUP_START		0x0200
#define RE_GROUP_END		0x0201
#define RE_MATCH			0xFFFF
typedef struct RE_State
{
	unsigned short c;	// 0x0000-0x00FF: match literal; RE_* for specials
	char g;				// greedy jump
	char ng;			// non-greedy jump
}
RE_State;

typedef struct RE_Match
{
	unsigned short s;	// start position
	unsigned short e;	// end position
}
RE_Match;

typedef struct RE_Regexp
{
	RE_State state[256];
	RE_Match match[256];
}
RE_Regexp;

void RE_dump_state(RE_State * s, int i)
{
#define NR	printf("%3d  ", i)
	switch (s->c) {
		case RE_MATCH      : NR; printf(" M \n"); break;
		case RE_ANY        : NR; printf(" A \n"); break;
		case RE_SPLIT: NR; printf(" S %+4d%+4d\n", s->g, s->ng); break;
		case RE_JUMP       : NR; printf(" J %+4d\n", s->g); break;
		case RE_GROUP_START: NR; printf(" ( %4d\n", s->g); break;
		case RE_GROUP_END  : NR; printf(" ) %4d\n", s->g); break;
		default            : NR; printf("'%c'\n"   , s->c); break;
	}
#undef NR
}

void RE_dump_states(RE_State * s)
{
	for (int i=0; ; s++, i++) {
		RE_dump_state(s, i);
		if (s->c == RE_MATCH) break;
	}
}

RE_Regexp RE_compile(const char * regexp)
{
	RE_Regexp re;
	RE_State * s_tail = re.state;
	void append(unsigned short c, char g, char ng)
	{
		*s_tail++ = (RE_State){ c, g, ng };
	}
	void expr(RE_State * s, int group)
	{
		RE_State * s_head = s;
		void prepend(unsigned short c, char g, char ng)
		{
			RE_State * s = s_tail++;
			for (; s != s_head-1; s--)
				*(s+1) = *s;
			*s_head = (RE_State){ c, g, ng };
		}
		for (; *regexp; regexp++) {
			switch (*regexp) {
				case '(':
					regexp++;
					s_head = s_tail;
					append(RE_GROUP_START, group, 0);
					expr(s_tail, group+1);
					if (*regexp != ')') err(1, "missing ).");
					append(RE_GROUP_END, group, 0);
					break;
				case ')': return;
				case '?': prepend(RE_SPLIT, 1, s_tail-s_head+1); break;
				case '+':  append(RE_SPLIT, s_head-s_tail, 1); break;
				case '*': prepend(RE_SPLIT, 1, s_tail-s_head+2);
						   append(RE_SPLIT, s_head-s_tail+1, 1);
						  break;
				case '|':
					s_head = s;
					prepend(RE_SPLIT, 1, s_tail-s_head+2);
					s = s_tail;
					append(RE_JUMP, 0, 0);
					regexp++;
					expr(s_tail, group);
					s->g = s_tail-s;
					regexp--;
					break;
				case '.':
					s_head = s_tail;
					append(RE_ANY, 0, 0);
					break;
				default:
					s_head = s_tail;
					append(*regexp, 0, 0);
			}
		}
	}
	expr(re.state, 0);
	append(RE_MATCH, 0, 0);
	return re;
}

int RE_match(RE_Regexp * re, const char * str)
{
	RE_Match * m = re->match;
	int match(RE_State * s, const char * p)
	{
		for (; ; p+=(*p!=0), s++) {
			//printf(">> @%d(%x)\t[%d]%c(%2.2X)\n", s-re->state, s->c, p-str, *p, *p);
			switch (s->c) {
				case RE_ANY: if (!*p) return 0; continue;
				case RE_SPLIT:
					if (match(s+s-> g, p)) return 1;
					if (match(s+s->ng, p)) return 1;
					return 0;
				case RE_JUMP: s += s->g-1; p--; break;
				case RE_MATCH: return !*p;
				case RE_GROUP_START: m[s->g].s = p-str; p--; break;
				case RE_GROUP_END  : m[s->g].e = p-str; p--; break;
				default: if (*p != s->c) return 0;
			}
		}
#if 0
		for (;; s++) {
			//printf(">> @%d(%x)\t[%d]%c(%2.2X)\n", s-re->state, s->c, p-str, *p, *p);
			switch (s->c) {
				case RE_SPLIT:
					if (match(s+s-> g, p)) return 1;
					if (match(s+s->ng, p)) return 1;
					return 0;
				case RE_JUMP: s += s->g-1; break;
				case RE_MATCH: return 1;
				case RE_GROUP_START: m[s->g].s = p-str; break;
				case RE_GROUP_END  : m[s->g].e = p-str; break;
				default: return 0;
			}
		}
#endif
		return 0;
	}
	return match(re->state, str);
}

int main()
{
	//RE_Regexp re = RE_compile("ab(c|def).*");
	RE_Regexp re = RE_compile("a(((b|c)?d)+).*");
	RE_dump_states(re.state);
	{
		char str[] = "ad";
		if (RE_match(&re, str)) {
			str[re.match[0].e] = 0;
			printf("M%4d%4d -> %s\n", re.match[0].s, re.match[0].e, &str[re.match[0].s]);
		}
		else printf("!\n");
	}
	{
		char str[] = "abd";
		if (RE_match(&re, str)) {
			str[re.match[0].e] = 0;
			printf("M%4d%4d -> %s\n", re.match[0].s, re.match[0].e, &str[re.match[0].s]);
		}
		else printf("!\n");
	}
	{
		char str[] = "acd";
		if (RE_match(&re, str)) {
			str[re.match[0].e] = 0;
			printf("M%4d%4d -> %s\n", re.match[0].s, re.match[0].e, &str[re.match[0].s]);
		}
		else printf("!\n");
	}
	{
		char str[] = "abddcddd";
		if (RE_match(&re, str)) {
			str[re.match[0].e] = 0;
			printf("M%4d%4d -> %s\n", re.match[0].s, re.match[0].e, &str[re.match[0].s]);
		}
		else printf("!\n");
	}
	{
		char str[] = "adddbdbcd";
		if (RE_match(&re, str)) {
			str[re.match[0].e] = 0;
			printf("M%4d%4d -> %s\n", re.match[0].s, re.match[0].e, &str[re.match[0].s]);
		}
		else printf("!\n");
	}
	return 0;
}

