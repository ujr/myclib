# Notes on C

## Standards

**K&R C** is the original C, as described in the first edition (1978)
of *The C Programming Language* by Kernighan and Ritchie.
C was standardized by ANSI in 1989, and the second edition (1988)
of *The C Programming Language* describes this revised version
of C, known as **ANSI C** or **C89**. As compilers added features
C was standardized again in 1999, creating the **C99** dialect.
The same happened again in 2011, creating the **C11** dialect.
Today it is probably save to assume that C99 is supported
by your compiler. Some of the new C99 features are:

- C++ style line comments: `// comment`
- the Boolean data type (`_Bool`, stdbool.h provides `bool`)
- mixing declarations and code (not only at start of block)
- variable-length arrays (length determined at runtime)
- [flexible array members][fam] (member array without a dimension)
- variadic macros, the [offsetof][offsetof] macro (from stddef.h)

[fam]: https://en.wikipedia.org/wiki/Flexible_array_member
[offsetof]: https://en.wikipedia.org/wiki/Offsetof

## Operator Precedence

Precedence and associativity together determine the order
in which operators are applied to their operands, that is,
the evaluation order. **Precedence** is how tight an operator
binds to its arguments. **Associativity** (left or right)
defines evaluation order amongst operators of the same precedence.

|  # | Operators | Name | Associativity |
|---:|-----------|------|:-------------:|
|  1 | `()   []   ->   .` | call, index, member | left |
|  2 | `!  ~  ++  --  +  -  *  &  (type)  sizeof` | unary | right |
|  3 | `*   /   %` | multiplicative | left |
|  4 | `+    -`    | additive       | left |
|  5 | `<<   >>`   | bitshift       | left |
|  6 | `<  <=  >  >=` | relational  | left |
|  7 | `==   !=`   | equality       | left |
|  8 | `&`         | bitwise and    | left |
|  9 | `^`         | bitwise xor    | left |
| 10 | `|`         | bitwise or     | left |
| 11 | `&&`        | logical and    | left |
| 12 | `||`        | logical or     | left |
| 13 | `?:`        | conditional    | right |
| 14 | `= += -= *= /= %= &= ^= |= <<= >>=` | assignment | right |
| 15 | `,`         | comma          | left |

Operators are listed in order from highest precedence (tightest
binding) to least precedence. Right associative are only the
unary ops, the ternary op, and all assignment ops.
All others bind left-to-right; for example, 4-2+1 is 3 (not 1).

> Side note: C has many levels of precedence, and other
> languages have many more. In contrast, METAFONT uses only
> 4 levels of precedence named: primary (literals, variables,
> unary, expr in parens), secondary (multiplicative),
> tertiary (additive), and expression (the rest).

## Standard Library

The standard library is an integral part of the C standard.
It provides some 200 functions for I/O, dynamic memory,
mathematics, assertions, formatting, etc. It is useful,
always available, but somewhat inconsistent.

The functionality of the ANSI C Standard Library is declared
in the following headers. Other headers, like `<stdint.h>`
are not part of the ANSI C standard.

|Header|Remarks|
|------|-------|
|`<assert.h>` | assertions: the `assert()` macro|
|`<ctype.h>`  | character classification: `isalpha()` etc.|
|`<errno.h>`  | the `errno` expression and the `EXXX` constants|
|`<float.h>`  | various float and double constants|
|`<limits.h>` | size of integral types: `XXX_MIN` and `XXX_MAX`|
|`<locale.h>` | localisation: `setlocale()` and `localeconv()`|
|`<math.h>`   | math functions: `sin()`, `atan2()`, `sqrt()`, etc.|
|`<setjmp.h>` | non-local jumps: `setjmp()` and `longjmp()`|
|`<signal.h>` | signal processing: `raise()` and `signal()`|
|`<stdarg.h>` | access to variable argument lists: `va_list` etc.|
|`<stddef.h>` | `NULL`, `size_t`, `ptrdiff_t`, `wchar_t`, `offsetof()`|
|`<stdio.h>`  | input and output streams: `fopen()`, `printf()`, etc.|
|`<stdlib.h>` | memory allocation, random numbers, and varia|
|`<string.h>` | string and memory functions: `strxxx()` and `memxxx()`|
|`<time.h>`   | getting and formatting time: `time()`, `strftime()`, etc.|

## Assertions

Assertions document assumptions inherent in the code
and enforce them at run-time by aborting the program
if an assertion fails. The standard `<assert.h>` header
defines the **assert** macro. If `NDEBUG` is defined
when `<assert.h>` is included, it always evaluates to
`((void) 0)`, that is, it does nothing. Otherwise, it
tests the argument expression; if true, it does nothing,
if false, it prints a diagnostic message to standard
error and aborts the program. Beware that the expression
being asserted must not have side effects.
It is recommended to leave `NDEBUG` undefined.
The header *could* be implemented as follows:

```C
#undef assert
#ifdef NDEBUG
#define assert(test) ((void) 0)
#else
void _assert(char *msg);
#define _STR(x) _VAL(x)
#define _VAL(x) #x
#define assert(test) ((test) ? (void) 0 : \
  _assert(__FILE__ ":" _STR(__LINE__) " " #test))
#endif
```

The initial `#undef` allows assertions to be turned
on and off in the same source file. The active form
of the macro evaluates to a conditional expression
resulting in `((void) 0)` if the assertion holds and
an invocation of some `_assert` function if it fails.
The `_STR` and `_VAL` trickery converts `__LINE__`
(an integer) into a literal string: when on line 13,
`_STR(__LINE__)` evaluates to `_VAL(13)`, which then
evaluates to the string `"13"`, such that the `_assert`
function is called with a literal string argument like
`"foo.c:13 bar > 0"`.

## Character Types

The `<ctype.h>` header is for character class testing.

```C
isalnum(c)   // isalpha(c) or isdigit(c)
isalpha(c)   // isupper(c) or islower(c)
iscntrl(c)   // control character
isdigit(c)   // decimal digit
isgraph(c)   // printing character except space
islower(c)   // lower-case letter
isprint(c)   // printing character including space
ispunct(c)   // punctuation
isspace(c)   // space, \n, \r, \t, formfeed, vertical tab
isupper(c)   // upper-case letter
isxdigit(c)  // hexadecimal digit: 0-9, a-f, A-F
```

In all these functions (typically implemented as macros),
the argument is of type `int` but must hold a value
representable as an `unsigned char` or the special value
`EOF` (usually `-1`). The return value is of type `int`;
it is non-zero if the condition is satisified, and zero
if not.

In the ASCII character set, the printing characters are
0x20 (space) through 0x7E (`~`); the control characters
are 0 to 0x1F, and 0x7F (DEL).

The function `tolower(c)` returns the corresponding
lower-case letter if `isupper(c)` and `c` if not.
Similarly, `toupper(c)` returns the corresponding
upper-case letter if `islower(c)` and `c` if not.

## Undefined Behavior

The C specification leaves some behavior unspecified.
This includes:

- dereferencing a null pointer

```C
void foo(int *p) {
  int dead = *p;   // dead deref
  if (!p) return;  // null check
  *p = 13;         // assignment
}
```

Dereferencing a null pointer is always undefined,
even if it happens in dead code. Therefore, in the
code above a compiler may conclude that p is not
null and optimize away anything but the assignment,
with catastrophic effects.

## References

Kernighan and Ritchie:
*The C Programming Language*, 2nd edition,
Prentice Hall 1988.
[Amazon](https://www.amazon.com/dp/0131103628).

P.J. Plauger: *The Standard C Library*.
Prentice Hall, 1992.
[Amazon](https://www.amazon.com/dp/0131315099)

Rob Pike:
*Notes on Programming in C*, 1989,
[archived at Lysator](https://www.lysator.liu.se/c/pikestyle.html),
and [local copy](PikeStyle.md)
