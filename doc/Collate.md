# String Collation

Collation means locale-sensitive string comparison.
The Standard C Library provides two functions to determine
how two strings collate in the current locale:

```C
#include <string.h>

int strcoll(const char *s1, const char *s2);
size_t strxfrm(char *s1, const char *s2, size_t n);
```

The **strcoll** function compares the two strings *s1*
and *s2* returning an integer less than, equal to, or
greater than zero as *s1* collates prior to, equal to,
or after the string *s2* according to the category
`LC_COLLATE` in the current locale.

The **strxfrm** function transforms *s2* into the buffer
*s1* (of length *n*) in such a way that `strcmp` would
return the same result as `strcoll` would on the original
strings. *s1* and *s2* must not overlap; if *n* is zero,
*s1* may be null. Return value is length of transformed
string (not counting the terminating `\0`).

## Implementation

*The Standard C Library* by P.J. Plauger (Prentice Hall 1992,
ISBN 0-13-131509-9) includes an implementation of these functions.
What follows is mostly verbatim from this book:

Both **strcoll** and **strxfrm** determine collation sequence
by mapping strings to a form that collates properly when compared
using **strcmp**. The locale category `LC_COLLATE` determines
this mapping. It does so by specifying the state table used by
the internal function **_Strxfrm**. Thus, `strcoll` and `strxfrm`
call `_Strxfrm` to map strings appropriately.

All the collation functions include the internal header
`"xstrxfrm.h"`, which defines the type `_Cosave` and declares
the function `_Strxfrm`. A data object of type `_Cosave` stores
state information between calls to `_Strxfrm`.

```C
/* xstrxfrm.h internal header */
#include <string.h>
#include "xstate.h"

typedef struct {
    unsigned char _State;
    unsigned short _Wchar;
} _Cosave;

size_t _Strxfrm(char *, const unsigned char **, size_t, _Cosave *);
```

The state table definitions are in two other files, *xstate.h* and
*xstate.c*; the latter contains the single state table that is common
to all functions that use state tables. It is cleverly contrived
to produce useful, if simple, results for all these functions.

```C
/* xstate.h internal header */

#define ST_CH      0x00ff    /* character part */
#define ST_STATE   0x0f00    /* successor state part */
#define ST_STOFF   8
#define ST_FOLD    0x8000    /* fold input to accumulator */
#define ST_INPUT   0x4000    /* consume input */
#define ST_OUTPUT  0x2000    /* produce output */
#define ST_ROTATE  0x1000    /* reverse bytes in accumulator */
#define _NSTATE    16

typedef struct {
    const unsigned short *_Tab[_NSTATE];
} _Statab;

extern _Statab, _Costate, _Mbstate, _Wcstate;
```

```C
/* xstate.c - default state tables */
#include <limits.h>
#include "xlocale.h"
#if UCHAR_MAX != 255
#error WRONG STATE TABLE
#endif

#define X  (ST_FOLD | ST_OUTPUT | ST_INPUT)

static const unsigned short tab0[257] = {0,  /* alloc flag */
X|0x00, X|0x01, X|0x02, X|0x03, X|0x04, X|0x05, X|0x06, X|0x07,
X|0x08, X|0x09, X|0x0A, X|0x0B, X|0x0C, X|0x0D, X|0x0E, X|0x0F,
X|0x10, X|0x11, X|0x12, X|0x13, X|0x14, X|0x15, X|0x16, X|0x17,
X|0x18, X|0x19, X|0x1A, X|0x1B, X|0x1C, X|0x1D, X|0x1E, X|0x1F,
X|0x20, X|0x21, X|0x22, X|0x23, X|0x24, X|0x25, X|0x26, X|0x27,
X|0x28, X|0x29, X|0x2A, X|0x2B, X|0x2C, X|0x2D, X|0x2E, X|0x2F,
X|0x30, X|0x31, X|0x32, X|0x33, X|0x34, X|0x35, X|0x36, X|0x37,
X|0x38, X|0x39, X|0x3A, X|0x3B, X|0x3C, X|0x3D, X|0x3E, X|0x3F,
X|0x40, X|0x41, X|0x42, X|0x43, X|0x44, X|0x45, X|0x46, X|0x47,
X|0x48, X|0x49, X|0x4A, X|0x4B, X|0x4C, X|0x4D, X|0x4E, X|0x4F,
X|0x50, X|0x51, X|0x52, X|0x53, X|0x54, X|0x55, X|0x56, X|0x57,
X|0x58, X|0x59, X|0x5A, X|0x5B, X|0x5C, X|0x5D, X|0x5E, X|0x5F,
X|0x60, X|0x61, X|0x62, X|0x63, X|0x64, X|0x65, X|0x66, X|0x67,
X|0x68, X|0x69, X|0x6A, X|0x6B, X|0x6C, X|0x6D, X|0x6E, X|0x6F,
X|0x70, X|0x71, X|0x72, X|0x73, X|0x74, X|0x75, X|0x76, X|0x77,
X|0x78, X|0x79, X|0x7A, X|0x7B, X|0x7C, X|0x7D, X|0x7E, X|0x7F,

X|0x80, X|0x81, X|0x82, X|0x83, X|0x84, X|0x85, X|0x86, X|0x87,
X|0x88, X|0x89, X|0x8A, X|0x8B, X|0x8C, X|0x8D, X|0x8E, X|0x8F,
X|0x90, X|0x91, X|0x92, X|0x93, X|0x94, X|0x95, X|0x96, X|0x97,
X|0x98, X|0x99, X|0x9A, X|0x9B, X|0x9C, X|0x9D, X|0x9E, X|0x9F,
X|0xA0, X|0xA1, X|0xA2, X|0xA3, X|0xA4, X|0xA5, X|0xA6, X|0xA7,
X|0xA8, X|0xA9, X|0xAA, X|0xAB, X|0xAC, X|0xAD, X|0xAE, X|0xAF,
X|0xB0, X|0xB1, X|0xB2, X|0xB3, X|0xB4, X|0xB5, X|0xB6, X|0xB7,
X|0xB8, X|0xB9, X|0xBA, X|0xBB, X|0xBC, X|0xBD, X|0xBE, X|0xBF,
X|0xC0, X|0xC1, X|0xC2, X|0xC3, X|0xC4, X|0xC5, X|0xC6, X|0xC7,
X|0xC8, X|0xC9, X|0xCA, X|0xCB, X|0xCC, X|0xCD, X|0xCE, X|0xCF,
X|0xD0, X|0xD1, X|0xD2, X|0xD3, X|0xD4, X|0xD5, X|0xD6, X|0xD7,
X|0xD8, X|0xD9, X|0xDA, X|0xDB, X|0xDC, X|0xDD, X|0xDE, X|0xDF,
X|0xE0, X|0xE1, X|0xE2, X|0xE3, X|0xE4, X|0xE5, X|0xE6, X|0xE7,
X|0xE8, X|0xE9, X|0xEA, X|0xEB, X|0xEC, X|0xED, X|0xEE, X|0xEF,
X|0xF0, X|0xF1, X|0xF2, X|0xF3, X|0xF4, X|0xF5, X|0xF6, X|0xF7,
X|0xF8, X|0xF9, X|0xFA, X|0xFB, X|0xFC, X|0xFD, X|0xFE, X|0xFF,
};

char _Mbcurmax = 1;

_Statab _Costate = {&tab0[1]};
_Statab _Mbstate = {&tab0[1]};
_Statab _Wcstate = {&tab0[1]};
```

The function **strxfrm** best illustrates how the collation functions
work together. It stores the mapped string in the buffer pointed to by
*s1*, of length *n*. Once the buffer is full, the function translates
the remainder of the source string *s2* to determine the full length
of the mapped string. `Strxfrm` stores any such excess characters in
its own dynamic temporary buffer `buf`.

```C
/* strxfrm: transform s2[] to s1[] by locale-dependent rule */
size_t (strxfrm)(char *s1, const char *s2, size_t n)
{
  size_t nx = 0;
  const unsigned char *s = (const unsigned char *) s2;
  _Cosave state = {0};

  while (nx < n) {  /* translate and deliver */
      size_t i = _Strxfrm(s1, &s, nx - n, &state);

      s1 += i, nx += i;
      if (0 < i && s1[-1] == '\0')
          return nx - 1;
      else if (*s == '\0')  /* rescan */
          s = (const unsigned char *) s2;
  }
  for (;;) {  /* translate and count */
      char buf[32];
      size_t i = _Strxfrm(buf, &s, sizeof buf, &state);

      nx += i;
      if (0 < i && buf[i - 1] == '\0')
          return nx - 1;
      else if (*s == '\0')  /* rescan */
          s = (const unsigned char *) s2;
  }
}
```

The function **_Strxfrm** performs the actual mapping. It does so
as a finite-state machine executing the state table stored at
`_Costate`. `_Strxfrm` must be particularly cautios because this
table can be flawed. It can change with locale category `LC_COLLATE`
in ways that the Standard C library cannot control.

Note the various ways the function can elect to take an error return:

- if a transfer occurs to an undefined state
- if no state table exists for a given state
- if the function makes so many state transitions since generating
 an output character that it must be looping
- if the state table entry specifically signals an error

The rest of `_Strxfrm` is simle by comparison. The function retains the
wide-character accumulator (`ps->_Wchar`) as part of the state memory.
That simplifies generating a sequence of mapped characters with a
common component while in a given shift state. `_Strxfrm` returns
after it fills the output bufer (with `size` characters) or whenever
it encounters the terminating null character in the source string.

That can happen more than once. Note the careful way that `strxfrm`
distinguishes the three reasons why `_Strxfrm` returns:

- If the last character delivered is a null character, the translation
  is complete. `_Strxfrm` delivers a null character if an error occurs.
  It also jiggers the stored state information to fail immediately should
  it be inadvertently called again for the same string.
- Otherwise, if the next source character is a null character,
  `_Strxfrm` wants to rescan the source string. `_Strxfrm` will not
  point past a null character in the source string.
- Otherwise, `_Strxfrm` wants to continue where it left off.

```C
#include <limits.h>
#include "xstrxfrm.h"

/* _Strxfrm: translate string to collatable form */
size_t _Strxfrm(char *sout, const unsigned char **psin,
                size_t size, _Cosave *ps)
{
  char state = ps->_State;
  int leave = 0;
  int limit = 0;
  int nout = 0;
  const unsigned char *sin = *psin;
  unsigned short wc = ps->_Wchar;

  for (;;) {  /* perform a state transformation */
      unsigned short code;
      const unsigned short *stab;

      if (_NSTATE <= state
          || (stab = _Costate._Tab[state]) == NULL
          || (_NSTATE*UCHAR_MAX) <= ++limit
          || (code = stab[*sin]) == 0)
          break;
      state = (code & ST_STATE) >> ST_STOFF;
      if (code & ST_FOLD)
          wc = wc & ~UCHAR_MAX | code & ST_CH;
      if (code & ST_ROTATE)
          wc = wc >> CHAR_BIT & UCHAR_MAX | wc << CHAR_BIT;
      if (code & ST_OUTPUT
          && ((sout[nout++] = code & ST_CH ? code : wc) == '\0'
              || size <= nout))
          leave = 1;
      if (code & ST_INPUT)
          if (*sin != '\0') ++sin, limit = 0;
          else leave = 1;

      if (leave) {  /* return for now */
          *psin = sin;
          ps->_State = state;
          ps->_Wchar = wc;
          return nout;
      }
  }
  /* error return */
  sout[nout++] = '\0';
  *psin = sin;
  ps->_State = _NSTATE;
  return nout;
}
```

The function **strcoll** is somewhat more complex than `strxfrm`.
It must translate *two* source strings a piece at a time so that
it can compare their mapped forms. The type `Sctl` describes
a data object that holds the information needed to process each
source string. The internal function `getxfrm` calls `_Strxfrm`
to update an `Sctl` data object.

The comparison loop within `strcoll` thus calls `getxfrm` for
each source string that has no mapped characters in its `Sctl`
buffer. That ensures that each source string is represented by
at least one mapped character, if any such characters remain
to be generated. `Strcoll` compares all the mapped characters
that it can. It returns zero only if both mapped strings compare
equal character by character and have the same length.

```C
/* strcoll.c */
#include "xstrxfrm.h"

typedef struct {
    char buf[32];
    const unsigned char *s1, *s2, *sout;
    _Cosave state;
} Sctl;

/* getxfrm: get transformed chars */
static size_t getxfrm(Sctl *p)
{
  size_t i;

  do {  /* loop until chars delivered */
      p->sout = (const unsigned char *) p->buf;
      i = _Strxfrm(p->buf, &p->s1, sizeof (p->buf), &p->state);
      if (0 < i && p->buf[i - 1] == '\0')
          return i - 1;
      else if (*p->s1 == '\0')  /* rescan */
          p->s1 = p->s2;
  } while (i == 0);
  return i;
}

/* strcoll: compare s1[], s2[] using locale-dependent rule */
int (strcoll)(const char *s1, const char *s2)
{
  size_t n1, n2;
  Sctl st1, st2;
  static const _Cosave initial = {0};

  st1.s1 = (const unsigned char *) s1;
  st1.s2 = (const unsigned char *) s1;
  st1.state = initial;

  st2.s1 = (const unsigned char *) s2;
  st2.s2 = (const unsigned char *) s2;
  st2.state = initial;

  for (n1 = n2 = 0; ; ) {  /* compare transformed chars */
      int ans;
      size_t n;

      if (n1 == 0) n1 = getxfrm(&st1);
      if (n2 == 0) n2 = getxfrm(&st2);
      n = (n1 < n2) ? n1 : n2;

      if (n == 0)
          return (n1 == n2 ? 0 : 0 < n2 ? -1 : +1);
      else if ((ans = memcmp(st1.sout, st2.sout, n)) != 0)
          return ans;

      st1.sout += n, n1 -= n;
      st2.sout += n, n2 -= n;
  }
}
```

**Exercise 14.1**.
The following locale file defines a simple "dictionary" collation
sequence that ignores punctuation and distinctions between uppercase
and lowercase letters:

```text
LOCALE DICT
NOTE dictionary collation sequence
collate[0, 0      ] '.'        $O $I $0
collate[0, 1:$#   ]               $I $0
collate[0, 'a':'z'] $@         $O $I $0
collate[0, 'A':'Z'] $@+'a'-'A' $O $I $0
collate[1, 0:$#   ] $@         $O $I $1
LOCALE end
```

Describe the mapping that it performs. Why does it rescan?
Draw a state-transition diagram for this mapping.

Note: The general form of lines defining state tables is
`table[state, lo:hi] expr`
where *expr* consists of terms that are added together (unless
one is preceded with a minus; plus signs are optional); the meaning
of the $*x* terms is: `$#` `UCHAR_MAX`, `$@` the table index,
`$O` cause output, `$I` consume input, `$0` `$1` ... `$7` successor state.

**Exercise 14.2**.
Modify the locale file in the previous exercise to order names that
begin with `Mac` interchangeably with names that begin with `Mc`.
Order `Mac` before `Mc` only if the names otherwise are equal.

---

Citing again from Plauger's 1992 *Standard C Library*:

Several pairs of functions in this implementation use state tables
to define their behavior. You can specify up to 16 state tables for
each of the three entity names:

- `collate` – determines `strcoll` and `strxfrm`
- `mbtowc` – determines `mbtowc` and `mbstowcs`
- `wctomb` – determines `wctomb` and `wcstombs`

Here is how you can write the specification for the simple state
table in the file *xstate.c*. It makes the functions `mbtowc` and
`mbstowcs`, declared in `<stdlib.h>`, perform a one-to-one mapping
between multibyte and wide characters:

```text
mb_cur_max         1
mbtowc[0, 0:$#]    $@ $F $I $O $0
```

The first line gives the macro `MB_CUR_MAX`, defined in `stdlib.h`,
the value 1. No multibyte sequence requires more than one character.
The second line defines all elements of state table zero for `mbtowc`
and `mbstowcs`. It tells the functions to:

- *fold* the translation value into the accumulated value (`$F`)
- with the input code *mapped* to itself (`$@`)
- consume the *input* (`$I`)
- write the accumulated value as the *output* (`$O`)

The successor state is state zero (`$0`). Translation ends, in
this case, when a zero input code produces a zero wide character.

Here's another example [taken from exercise 14.1]:

```text
LOCALE DICT
NOTE dictionary collation sequence
collate[0, 0      ] '.'        $O $I $0
collate[0, 1:$#   ]               $I $0
collate[0, 'a':'z'] $@         $O $I $0
collate[0, 'A':'Z'] $@+'a'-'A' $O $I $0
collate[1, 0:$#   ] $@         $O $I $1
LOCALE end
```

This state table folds uppercase letters to lowercase letters,
maps `\0` to a dot `.` and sets all other entries to zero.
One state is sufficient (NO IDEA, WHAT STATE 1 IS GOOD FOR?).

The general form is: `table[state, lo:hi] expression`

An expression is simply a sequence of terms that get added together.
Terms can be added simply by writing them one after the other, plus
signs are optional. Use a minus sign before a term to subtract.

Decimal, octal, and hexadecimal numbers follow the usual rules of
C constants. The sequences 10, 012, 0xA all represent the decimal
value ten.

Single quotes around a character yield the value of the character,
just as for a character constant in a C source file. (No escape
sequences, such as '\012', are permitted, however.)

An uppercase letter has the value last assigned by a SET line.
All such variables are set to zero at program startup.

In addition to these terms, there are special terms signalled
by a leading dollar sign:

- `$$` – the current contents of a table element.
- `$@` – the index of a table element.
  `$$` and `$@`, if present, must precede any other terms in an expression.
- `$^` – the value of the macro `CHAR_MAX`.
- `$#` – the value of the macro `UCHAR_MAX`.
- `$a`, `$b`, `$f`, `$n`, `$r`, `$t`, `$v` – the value of the character
  escape sequences '\a', '\b', '\f', '\n', '\r', '\t', '\v'.
- `$A`, `$C`, `$D`, `$H`, `$L`, `$M`, `$P`, `$S`, `$U`, `$W` – the character
  classification bits used in the table ctype.
- `$0`, `$1`, `$2`, `$3`, `$4`, `$5`, `$6`, `$7` – the successor states
  0 through 7 in a state-table element. (No symbols are provided for
 successor states 8 through 15. Write `$7+$1` for state 8, and so forth.)
- `$F`, `$I`, `$O`, `$R` – the command bits used in a state-table element.
  These specify, in order: *fold* translated value into the accumulated
  value, consume *input*, produce *output*, and *reverse* bytes in the
  accumulated value.

With these special terms, you can write expressions in locale
files that don't depend on implementation-specific code values.
