#ifndef STR_H_
#define STR_H_

#include <stddef.h>

typedef struct {
    char *str;
    unsigned int len;
} str;

struct _strBuf;
typedef struct _strBuf* strBuf;

#define S(s) (str){ (char *)(s), sizeof(s) - 1 }

str  cstr(char *s);
str  asStr(strBuf s);

int  strLen(str s);
int  strStr(str haystack, str needle);
int  strChr(str s, char c);
int  strRchr(str s, char c);
int  strCmp(str s1, str s2);
str  strSub(str s, int start, int end);
str  strCut(str *s, str split);

strBuf fromCStr(char *s);
strBuf fromBlk(void *ptr, int len);
strBuf strNew(int cap);
void   strFree(strBuf buf);

strBuf strDup(str s);
strBuf strCpy(strBuf to, str from);
strBuf strCat(strBuf to, str from);
strBuf strAppend(strBuf to, char c);
strBuf strShrink(strBuf to, unsigned int n);

/* ================= implementation ================= */

#ifdef STR_IMPLEMENTATION

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct _strBuf {
    unsigned int cap;
    unsigned int len;
    char data[];
};

static int
str__grow_cap(int old_len) {
    if (old_len == 0) return 16;
    if (old_len < 1024) return old_len * 2;
    return (int)(old_len * 1.25);
}

static char *
str__ptr_by_idx(char *p, int len, int idx) {
    if (idx >= 0) {
        return (idx <= len) ? p + idx : NULL;
    }
    return (idx + len >= 0) ? p + len + idx : NULL;
}

static strBuf
str__reserve(strBuf s, unsigned int cap) {
    if (s->cap >= cap) return s;
    strBuf tmp = strNew(cap);
    tmp = strCpy(tmp, toStr(s));
    strFree(s);
    return tmp;
}

str
cstr(char *s) {
    return (str){ s, (unsigned int)strlen(s) };
}

str
toStr(strBuf s) {
    return (str){ s->data, s->len };
}

int
strLen(str s) {
    return (int)s.len;
}

int
strStr(str haystack, str needle) {
    unsigned int pos = 0;
    while (pos + needle.len <= haystack.len) {
        unsigned int i;
        for (i = 0; i < needle.len; i++) {
            if (haystack.str[pos + i] != needle.str[i]) {
                pos++;
                break;
            }
        }
        if (i == needle.len) return (int)pos;
    }
    return -1;
}

int
strChr(str s, char c) {
    for (unsigned int i = 0; i < s.len; i++) {
        if (s.str[i] == c) return (int)i;
    }
    return -1;
}

int
strRchr(str s, char c) {
    for (int i = (int)s.len - 1; i >= 0; i--) {
        if (s.str[i] == c) return i;
    }
    return -1;
}

int
strCmp(str s1, str s2) {
    unsigned int i = 0;
    while (i < s1.len && i < s2.len) {
        if (s1.str[i] != s2.str[i])
            return (unsigned char)s1.str[i] - (unsigned char)s2.str[i];
        i++;
    }
    return (int)(s1.len - s2.len);
}

str
strSub(str s, int start, int end) {
    char *p1 = str__ptr_by_idx(s.str, s.len, start);
    char *p2 = str__ptr_by_idx(s.str, s.len, end);
    if (!p1 || !p2 || p1 >= p2) return (str){0};
    return (str){ p1, (unsigned int)(p2 - p1) };
}

char *
toCStr(strBuf s) {
    return s->data;
}

strBuf
fromBlk(void *ptr, int len) {
    strBuf ret = (strBuf)malloc(sizeof(struct _strBuf) + len + 1);
    if (ptr) memcpy(ret->data, ptr, len);
    ret->data[len] = '\0';
    ret->len = len;
    ret->cap = len + 1;
    return ret;
}

strBuf
fromCStr(char *s) {
    return fromBlk(s, (int)strlen(s));
}

strBuf
strNew(int cap) {
    assert(cap > 0);
    strBuf ret = (strBuf)malloc(sizeof(struct _strBuf) + cap);
    ret->cap = cap;
    ret->len = 0;
    ret->data[0] = '\0';
    return ret;
}

void
strFree(strBuf buf) {
    free(buf);
}

strBuf
strDup(str s) {
    return fromBlk(s.str, s.len);
}

strBuf
strCpy(strBuf to, str from) {
    if (to->cap < from.len + 1) {
        strFree(to);
        to = strNew(from.len + 1);
    }
    memcpy(to->data, from.str, from.len);
    to->len = from.len;
    to->data[to->len] = '\0';
    return to;
}

strBuf
strCat(strBuf to, str from) {
    to = str__reserve(to, to->len + from.len + 1);
    memcpy(to->data + to->len, from.str, from.len);
    to->len += from.len;
    to->data[to->len] = '\0';
    return to;
}

strBuf
strAppend(strBuf to, char c) {
    if (to->len + 2 > to->cap) {
        to = str__reserve(to, str__grow_cap(to->len + 1));
    }
    to->data[to->len++] = c;
    to->data[to->len] = '\0';
    return to;
}

strBuf
strShrink(strBuf to, unsigned int n) {
    if (n >= to->len) {
        to->len = 0;
    } else {
        to->len -= n;
    }
    to->data[to->len] = '\0';
    return to;
}

str
strCut(str *s, str split) {
    if (s == NULL || s->len == 0 || split.len == 0) {
        return (str){0};
    }

    int pos = strStr(*s, split);
    if (pos < 0) {
        str ret = *s;
        s->len = 0;
        return ret;
    }

    str ret = {
        .str = s->str,
        .len = (unsigned int)pos
    };

    unsigned int new_pos = pos + split.len;
    s->str += new_pos;
    s->len -= new_pos;

    return ret;
}

#endif /* STR_IMPLEMENTATION */
#endif /* STR_H_ */
