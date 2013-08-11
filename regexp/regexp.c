#include <stdio.h>
#include <string.h>
#include <err.h>

enum { RE_ANY = 0x0100, RE_SPLIT, RE_JUMP,
		RE_GROUP_START, RE_GROUP_END, RE_MATCH };
typedef struct RE_State
{
	unsigned short c;	// 0x0000-0x00FF: match literal; RE_* for specials
	char g, ng;			// greedy/non-greedy jump
	char is_ng;			// is non-greedy?
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
	void append(unsigned short c, char g, char ng, char is_ng)
	{
		*s_tail++ = (RE_State){ c, g, ng, is_ng };
	}
	void expr(RE_State * s, int group)
	{
		RE_State * s_head = s;
		void prepend(unsigned short c, char g, char ng, char is_ng)
		{
			RE_State * s = s_tail++;
			for (; s != s_head-1; s--) *(s+1) = *s;
			*s_head = (RE_State){ c, g, ng, is_ng };
		}
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
				case '.': s_head = s_tail; append(RE_ANY, 0, 0, 0); break;
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
		for (; ; p+=(p-str < len), s++) {
			//printf(">> @%d(%x)\t[%d]%c(%2.2X)\n", s-re->state, s->c, p-str, *p, *p);
			switch (s->c) {
				case RE_ANY: if (!*p) return 0; continue;
				case RE_SPLIT:
					if (s->is_ng) {
						if (match(s+s->ng, p)) return 1;
						if (match(s+s-> g, p)) return 1;
					}
					else {
						if (match(s+s-> g, p)) return 1;
						if (match(s+s->ng, p)) return 1;
					}
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




void RE_dump_state(RE_State * s, int i)
{
#define NR	printf("%.3d  ", i)
	switch (s->c) {
		case RE_MATCH      : NR; printf(" M \n"); break;
		case RE_ANY        : NR; printf(" A \n"); break;
		case RE_SPLIT: NR; printf(" S%c%+4d[%.3d]%+4d[%.3d]\n", (s->is_ng ? '?' : ' '), s->g, i+s->g, s->ng, i+s->ng); break;
		case RE_JUMP       : NR; printf(" J %+4d[%.3d]\n", s->g, i+s->g); break;
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

int main()
{
	RE_Regexp re = RE_compile("(.*?)(<.*?>)(.*?)(</.*>)(.*)");
	RE_dump_states(re.state);
	void dump_match(RE_Match * m, const char * str)
	{
		int len = m->e - m->s;
		char buf[len + 1];
		strncpy(buf, &str[m->s], len);
		buf[len] = 0;
		printf("%s", buf);
	}
	{
		char str[] = "ad<a>so</a><html>that</html>yes";
		if (RE_match(&re, str)) {
			printf("M -> ");
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

