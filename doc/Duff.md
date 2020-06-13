# Duff's Device

Duff's Device is a scheme to implement loop unrolling
manually in the C programming language. It solves the
problem using very little code even if the loop count
is not a multiple of the n-way unrolling.

The surprising aspect of Duff's Device is its use of
two control structures, `while` and `switch`, not
nested nor in sequence, but intertwined: the switch
cases jump right into the midst of the while loop.

Discovery of this scheme is attributed to Tom Duff in
1983, while working at Lucasfilm.

## Code

The code below copies 16-bit words from an array to a
memory-mapped output register (for memory-to-memory copy,
the `to` pointer would have to be incremented as well).

```C
void senddata(short *to, short *from, int count)
{
  do { /* assume count > 0 */
    *to = *from++;
  } while (--count > 0);
}
```

Compared to the small loop body, a lot of time is spent
on the loop condition. Loop unrolling is the standard
solution to this problem. However, special treatment
is necessary if the loop count is not a multiple of the
unrolling factor. In assembly programming, this is solved
by jumping into the unrolled loop body. Duff realized
that this is also possible with the C language.

```C
void unrolled(short *to, short *from, int count)
{
  register int n = (count+7)/8;  /* round up */
  switch (count % 8) {  /* jump to remainder */
  case 0: do { *to = *from++;
  case 7:      *to = *from++;
  case 6:      *to = *from++;
  case 5:      *to = *from++;
  case 4:      *to = *from++;
  case 3:      *to = *from++;
  case 2:      *to = *from++;
  case 1:      *to = *from++;
          } while (--n > 0);
  }
}
```

In the example above, the unrolling factor is 8.
The loop condition is executed 8 times less often,
which accounts for the performance benefit.
The loop count is divided by 8 (and rounded up),
and the `switch` jumps into the loop body such as
to account for the remainder of count/8.
In the C language, the switch cases are really
just labels and *fall through* (the lack of `break`
statements) is valid (though frowned upon and the
compiler will probably give warnings).

The following driver program calls the original
and the unrolled copy routines for illustration.

```C
void main()
{
  short target, source[18] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18
  };

  senddata(&target, source, 18);
  unrolled(&target, source, 18);
}
```

## Remarks

Loop unrolling is an optimization technique. Modern
compilers perform similar optimizations automatically.

To my knowledge, C is the only language, where Duff's
device is possible (excluding, of course, assembly language).

## References

- [Wikipedia: Duff's device][wiki]
- [The Jargon File: Duff's device][jargon]

[wiki]: https://en.wikipedia.org/wiki/Duff%27s_device
[jargon]: http://www.catb.org/~esr/jargon/html/D/Duffs-device.html

(ujr, July 2016)
