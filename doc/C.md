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

## References

Kernighan and Ritchie,
*The C Programming Language*, 2nd edition,
Prentice Hall 1988,
[ISBN 0131103628](https://www.amazon.com/dp/0131103628).

Rob Pike,
*Notes on Programming in C*, 1989,
[archived at Lysator](https://www.lysator.liu.se/c/pikestyle.html),
and [local copy](PikeStyle.md)
