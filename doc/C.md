# Notes on C

## Standards

**K&R C** is the original C, as described in the first edition (1978)
of *The C Programming Language* by Kernighan and Ritchie.
C was standardized by ANSI in 1989, and the second edition (1988)
of *The C Programming Language* describes this revised version
of C, known as **ANSI C** or **C89**. As compilers added features,
C was standardized again in 1999, creating the **C99** dialect.
The same happened again in 2011, creating the **C11** dialect.
Today it is probably save to assume that C99 is supported
by your compiler. Some of the new C99 features are:

- C++ style line comments: `// comment`
- the Boolean data type (`_Bool`, stdbool.h provides `bool`)
- mixing code and declarations (not only at start of block)
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
mathematics, assertions, formatting, etc.
It is somewhat inconsistent, but useful and always available.
See [CLib.md](./CLib.md)

## Idioms

C has idioms a few idioms that might not be immediately obvious,
but useful if you know them.

- `!!x` converts the integer `x` to a Boolean value: the
  first negation turns zero into 1 and non-zero into 0,
  the second negation inverts 0 and 1.

- `do { ... } while (0)` executes `...` exactly once, so
  it is redundant, but useful in macro definitions because
  it turns potentially many statements `...` into one
  compound statement.

- `while (n--) *p++ = *s++;` or in a more extreme variation
  `while (*t++ = *s++);` is probably the most classic idiom:
  *s* and *t* are typically character pointers; the idiom then
  copies characters up to and including the zero terminator;
  of course, *t* must point to large enough a buffer.

- `if ((a = b))...` assigns the value of *b* to *a*, then
  tests against `0` to decide how to branch; the additional
  parentheses are an optional indication that this is not just
  a mistyped `if (a == b)...` and may silence a compiler warning.

- `for (p = &a[0]; p < &a[N]; p++) *p = 0;` may be faster than
  the equivalent `for (i = 0; i < N; i++) a[i] = 0;` on old
  compilers. Note that `a[N]` is one beyond the array, which
  is ok for taking the address (but no further beyond).

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

Dereferencing a null pointer is always undefined, even
if it happens in dead code. Therefore, an implementation
may assume that any pointer that *is* dereferenced is
not NULL. In the code above a compiler may conclude that
`p` is not NULL and optimize away anything but the assignment,
with catastrophic effects.

The `offsetof` macro from `<stddef.h>` could be implemented as

```C
#define offsetof(t, m) ((size_t)&(((t *)0)->m))
```

but here a pointer to the zero address, the typical
implementation of the NULL pointer, is dereferenced,
so this implementation is most likely invalid.

- signed integer overflow

For example, `INT_MAX+1` is not guaranteed to be `INT_MIN`.
However, unsigned integer overflow is guaranteed to wrap around.

- array index out of bounds
- uninitialized variables

Other do bounds-checking and initialize all local variables to zero.
C does not for performance reasons, at the price of undefined behaviour.

- modifying string literals

They may be stored in read-only memory. Therefore,
the 2nd statement in `char *s = "hi"; s[0] = 'H';`
may cause an error.

- pointer comparison

Pointer comparison is only defined for pointers pointing
into the same object (array or struct). In practice, however,
any two pointers can be compared almost always (but remember
the segmented memory of DOS).

There is more undefined behaviour; see, e.g.,
[Wikipedia](https://en.wikipedia.org/wiki/Undefined_behavior).

## Bibliography

Kernighan and Ritchie:
*The C Programming Language*, 2nd edition,
Prentice Hall 1988.
[Amazon](https://www.amazon.com/dp/0131103628)

P.J. Plauger: *The Standard C Library*,
Prentice Hall 1992.
[Amazon](https://www.amazon.com/dp/0131315099)

Rob Pike:
*Notes on Programming in C*, 1989,
[archived at Lysator](https://www.lysator.liu.se/c/pikestyle.html),
and [local copy](PikeStyle.md)

Steve Summit:
*C Programming FAQs*, Addison-Wesley 1995.
[Amazon](https://www.amazon.com/dp/0201845199)
and online at [c-faq.com](http://c-faq.com/)

Eric Laroche:
*C Programming Language Overview*, 2004,
<http://www.lrdev.com/lr/c/c-programming-language-overview.html>

Jim Larson:
*Standards and Style for Coding in ANSI C*, 1996,
once at <http://www.jetcafe.org/jim/c-style.html>
