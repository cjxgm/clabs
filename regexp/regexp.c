// vim: noet ts=4 sw=4 sts=0
// compile: gcc -std=gnu11 -march=native -O3 -o regexp regexp.c
// 			strip -s regexp
// or, for a smaller executable, install rld(https://github.com/stfsux/rld)
// and run (you will get a 1~2KB sized file.):
// 			gcc -c -std=gnu11 -march=native -O3 -o regexp.o regexp.c
// 			rlds --compress -lc -o regexp regexp.o
// 			rm -f regexp.o
#include <stdio.h>
#include <string.h>
#include <err.h>

enum { RE_ANY = 0x0100, RE_SPLIT, RE_JUMP,
		RE_GROUP_START, RE_GROUP_END, RE_MATCH };
typedef struct RE_State
{
	unsigned short c;	// 0x0000-0x00FF: match literal; RE_* for specials
	char g, ng;			// greedy/non-greedy jump
} RE_State;
typedef struct RE_Match { unsigned short s, e; } RE_Match;
typedef struct RE_Regexp
{
	RE_State state[256];
	RE_Match match[256];
} RE_Regexp;

RE_Regexp RE_compile(const char * regexp)
{
	RE_Regexp re;
	RE_State * s_tail = re.state;
	int group_unused = 0;
	char is_ng()
	{
		if (regexp[0] && regexp[1] == '?') {
			regexp++;
			return 1;
		}
		return 0;
	}
#define STATE	(is_ng ? (RE_State){ c, ng, g } : (RE_State){ c, g, ng })
	void append(unsigned short c, char g, char ng, char is_ng)
	{
		*s_tail++ = STATE;
	}
	void expr(RE_State * s, int group)
	{
		RE_State * s_head = s;
		void prepend(unsigned short c, char g, char ng, char is_ng)
		{
			RE_State * s = s_tail++;
			for (; s != s_head-1; s--) *(s+1) = *s;
			*s_head = STATE;
		}
#undef STATE
		for (; *regexp; regexp++) {
			switch (*regexp) {
				case '(':
					regexp++;
					s_head = s_tail;
					append(RE_GROUP_START, group, 0, 0);
					expr(s_tail, ++group_unused);
					if (*regexp != ')') err(1, "missing ).");
					append(RE_GROUP_END, group, 0, 0);
					group++;
					break;
				case ')': return;
				case '?': prepend(RE_SPLIT, 1, s_tail-s_head+1, is_ng()); break;
				case '+':  append(RE_SPLIT, s_head-s_tail, 1, is_ng()); break;
				case '*': prepend(RE_SPLIT, 1, s_tail-s_head+2, is_ng());
						   append(RE_JUMP, s_head-s_tail, 0, 0);
						  break;
				case '|':
					s_head = s;
					prepend(RE_SPLIT, 1, s_tail-s_head+2, 0);
					s = s_tail;
					append(RE_JUMP, 0, 0, 0);
					regexp++;
					expr(s_tail, group);
					s->g = s_tail-s;
					regexp--;
					break;
				case '.': s_head = s_tail; append(RE_ANY , 0, 0, 0); break;
				default : s_head = s_tail; append(*regexp, 0, 0, 0);
			}
		}
	}
	expr(re.state, 0);
	append(RE_MATCH, 0, 0, 0);
	return re;
}

int RE_match(RE_Regexp * re, const char * str)
{
	RE_Match * m = re->match;
	int len = strlen(str);
	int match(RE_State * s, const char * p)
	{
		for (; ; p += (p-str < len), s++) {
			//printf(">> @%d(%x)\t[%d]%c(%2.2X)\n", s-re->state, s->c, p-str, *p, *p);
			switch (s->c) {
				case RE_ANY: if (!*p || *p == '\n') return 0; continue;
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
		return 0;
	}
	return match(re->state, str);
}




int main()
{
	void dump_state(RE_State * s, int i)
	{
#define NR	printf("\e[0;33m%.3d  ", i)
		switch (s->c) {
			case RE_MATCH      : NR; printf("\e[1;32m M \n"); break;
			case RE_ANY        : NR; printf("\e[0;34m A \n"); break;
			case RE_SPLIT: NR; printf("\e[1;35m S \e[0;35m%+4d[\e[0;33m%.3d\e[0;35m]%+4d[\e[0;33m%.3d\e[0;35m]\n", s->g, i+s->g, s->ng, i+s->ng); break;
			case RE_JUMP       : NR; printf("\e[1;35m J \e[0;35m%+4d[\e[0;33m%.3d\e[0;35m]\n", s->g, i+s->g); break;
			case RE_GROUP_START: NR; printf("\e[1;36m ( \e[0;36m%4d\n", s->g); break;
			case RE_GROUP_END  : NR; printf("\e[1;36m ) \e[0;36m%4d\n", s->g); break;
			default            : NR; printf("\e[0m'\e[1m%c\e[0m'\n"   , s->c); break;
		}
		printf("\e[0m");
#undef NR
	}

	void dump_states(RE_State * s)
	{
		for (int i=0; ; s++, i++) {
			dump_state(s, i);
			if (s->c == RE_MATCH) break;
		}
	}

	void dump_match(RE_Match * m, const char * str)
	{
		int len = m->e - m->s;
		char buf[len+1];
		// you can use strncpy
		for (int i=0; i<len; i++)
			buf[i] = str[i + m->s];
		buf[len] = 0;
		printf("%s", buf);
	}


	RE_Regexp re = RE_compile("(.*?)(<.*?>)(.*?)(</.*>)(.*)");
	dump_states(re.state);
	{
		char str[] = "ad<a>so</a><html>that</html>yes";
		printf("%s\n", str);
		if (RE_match(&re, str)) {
			printf("matched -> ");
			dump_match(&re.match[0], str);
			printf(" | ");
			dump_match(&re.match[1], str);
			printf(" | ");
			dump_match(&re.match[2], str);
			printf(" | ");
			dump_match(&re.match[3], str);
			printf(" | ");
			dump_match(&re.match[4], str);
			printf("\n");
		}
		else printf("unmatched\n");
	}
	return 0;
}

