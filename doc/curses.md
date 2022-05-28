# curses & ncurses

Curses was first developed for BSD and later for System V.
Linux usually comes with ncurses, a newer implementation
with additional functionality like color support.

The original author of *curses* is Ken Arnold, who wrote the package
while at Berkely. His 1984 paper *Screen Updating and Cursor Movement
Optimization: A Library Package* describes the package; an enhanced
version of 1992 by Elan Amil is attached for historical reference.
Mark Horton at Bell Labs, author of *terminfo*, wrote a second
version of *curses* that retained Arnold's function and added
others to take advantage of *terminfo* features.

## Windows, Screens, Images

A **window** is an independent rectangular area of characters on
the screen. Technically, it is a `WINDOW` structure (or a pointer
to it). The standard screen or `stdscr` is a `WINDOW` that fills
the entire screen of a video terminal. It is created automatically
when *curses* is initialized. The `WINDOW` structure is essentially
a character array that maintains the **screen image**, that is,
the characters to be shown on screen.

Initially, the screen image is filled with blanks. Functions to
add characters to a screen image include: `insch()`, `addch()`,
`addstr()`, and `printw()`.

### Logical Cursor

*Curses* maintains a **logical cursor** for each window. Use
`move(y,x)` to move the `stdscr` cursor, and `wmove(w,y,x)`
to move the cursor of window *w*. Note that the coordinate
order is *line,column* or *y,x*, and that these coordinates
are zero-based (0,0 is the top-left character in any window).

The term *physical cursor* refers to the cursor of the video
terminal. *Curses* moves it only upon `refresh()`.

### Screen Refresh

Each window maintains its own screen image (essentially a
character array in the `WINDOW` structure). The physical
screen is only refreshed upon a call to `refresh()` or
`wrefresh(w)`.

The `refresh()` algorithm does not output the whole screen
image; that would be inefficient at 9600 baud and painful at
1200 baud (fortunately, today terminals are much faster).
Instead, `refresh()` sends only the characters that have been
changed since the last refresh, after first moving the physical
cursor to those positions.

*Curses* does not know what the terminal displays, instead
it keeps an image of what it thinks the screen looks like
in a window called `curscr` (current screen). When `refresh()`
updates the terminal, it also updates `curscr` correspondingly.

Never change `curscr` directly for risk of confusing `refresh()`.
Also do not mix *curses* with regular output functions like
`printf()` or `putchar()` as these bypass `curscr`, and do not
always use the *curses* input functions, not `getch()` and the
like. If ever the screen gets garbled, say `refresh(curscr)`
to cause a full terminal refresh.

Depending on the input mode, certain input can cause problems:
some input characters may have control code effects on the
terminal, or a tab may be handled differently in the screen
image than on the screen. Any character that misaligns the
logical cursor from the physical cursor will cause problems.

## Multiple Windows

When `stdscr` is not enough, you can define additional windows
with `newwin()` or with `subwin()`. The former creates a new
window having its own character array and thus can overlap
any other windows. The latter creates a rectangular area
within another window, *sharing* the same character array!
Changes made in the subwindow are also visible in the parent
window (and vice versa). Each window *w* to be visible must
be refreshed separately with `wrefresh(w)` – `refresh()`
only refreshes the `stdscr`.

## The WINDOW Structure

It is helpful to understand a possible implementation of
the `WINDOW` structure. Because *curses* is defined by its
set of functions, each implementation is free to define
its `WINDOW` in any way it sees fit; still it is likely
to be a superset of the structure shown here:

```C
struct {
  short _cury, _curx;   /* window-relative cursor posn, zero-based */
  short _maxy, _maxx;   /* height and width (in characters) */
  short _begy, _begx;   /* screen-relative window position */
  short _flags;                  /* see text */
  bool _clear, _leave, _scroll;  /* see text */
  char ** _y;           /* the screen image (line arrays) */
  short *_firstch;
  short *_lastch;
}
```

Because `_cury` and `_curx` are zero-based (i.e., 0,0 is the
upper left character of the window), `_maxy` and `_maxx` are
one more than the maximum acceptable values for `_cury` and
`_curx`). Note that `_begy` and `_begx` are always relative
to the screen, even for subwindows!

The `_flags` are mostly used by `refresh()` for optimization;
here are just a few commonly used flags:

- `00001` = `_SUBWIN` is set if the window is a sub-window.
- `00002` = `_ENDLINE` is set if the right end of each line
  is the edge of the screen
- `00004` = `_FULLWIN` is set if the window fills the whole screen
- `00010` = `_SCROLLWIN` is set if last character of the window
  is the lower right edge of the screen; the terminal should
  scroll if a character is put here

The `_clear` flag is set by `clearok()` and `clear()` if the
screen should be cleared directly by using the terminal's clear
control code; `refresh()` will only use this if the window uses
the full screen (i.e., if `_FULLWIN` is set).

The `_leave` flag tells `refresh()` to leave the hardware cursor
wherever the update happens to leave it (spares a cursor motion);
normally, the hardware cursor is left at the window being refreshed.
This flag is set by `leaveok()`.

The `_scroll` flag is set by `scrollok()` and controls window
scrolling (that is, logical scrolling, not physical terminal
scrolling). Logical scrolling will shift lines up in the
`WINDOW` structure's `**_y` character array.

The `_y` points to a list of character arrays that contain
the screen image; `_y[i]` is the *i*th line and `_y[i][j]`
is the *j*th character on that line. This implementation
allows for efficient scrolling (but makes moving a window
difficult).

The `_firstch` and `_lastch` arrays are the positions of the
first and last characters on each line that have changed since
the last `refresh()` and help with optimization.

## Terminal Independence

The windowing features of *curses* easily overshadow terminal
independence, which abstracts from the many different terminals
manufacturers and models in use at the time of *curses*.
Sending regular characters like an `A` cause no trouble,
but control codes like `^A` differ between terminals.

Unix uses a database that describes the control codes used by
many different terminals. The first such facility developed is
called *termcap* and lives in the text file */etc/termcap*.
A more recent facility is called *terminfo* and lives in a
number of binary files in */etc/terminfo*.

*Curses* is built on top of *termcap* or *terminfo*.

## Using Curses

To use curses, `#include <curses.h>` in your source file(s)
and link against the curses library, either `-lcurses` or
`-lncurses` (on my 2022 Ubuntu system, *curses.h* and *libcurses*
are symbolic links to *ncurses.h* and *libncurses*, respectively).

## The Curses Functions

Many functions are just macros that operate on the `stdscr`.
For example, `refresh()` is equivalent to `wrefresh(stdscr)`.
The functions that operate on a specific window have names
beginning with `w` and the window is the first argument.

The so called *Move and Act Functions* first move the cursor
and then do some output or input; their names begin with `mv`
and their arguments begin with *y* (row) and *x* (column).

Window-specific move and act functions have names beginning
with `mvw` but their arguments begin with *y*, *x*, *win*
(in this order).

Some functions exist in all four variations, for example:
`addch(ch)`, `waddch(win,ch)`, `mvaddch(y,x,ch)`, `mvwaddch(win,y,x,ch)`.

## Initialize and Terminate

- `initscr()` – initialize curses
- `endwin()` – terminate curses

Must call `initscr()` before using other curses functions.
For character-at-a-time input without echoing, also call
`cbreak()` and `noecho()` after `initscr()`.

When done with curses, call `endwin()` to restore terminal
settings. You also want to trap at least `SIGINT` to ensure
the terminal is restored when the program is interrupted.

## Moving the Cursor

- `move(y,x)` – move logical cursor to line y and column x
- `getyx(win,y,x)` – put logical cursor location into y and x
- `inch()` – get character at logical cursor in *stdscr*
- `leaveok(win,flag)` – set or clear the `_leave` flag

## Output to Screen Image

- `addch(ch)` – add character to *stdscr*
- `addstr(str)` – add string by calling `addch()`
- `printw(fmt, ...)` – format and output to *stdscr*
- `insch(ch)` – insert character into *stdscr*
- `insertln()` – insert a line above current line in *stdscr*
- `box(win,v,h)` – draw a box at the edge of *win*
- `refresh()` – update screen to look like *stdscr*

Some characters are special for `addch()`: `\t` emulates a tab,
`\r` (carriage return) moves the logical cursor to the left end
of the line, `\b` (backspace) moves the logical cursor one position
to the left and does not change the screen image, `\n` (newline)
clears the rest of the line and moves the cursor one line down
and scrolls the window, if necessary.

`insch()` is like `addch()` but shifts the remainder of the line
one position to the right (losing the last character: there is
no wrapping to the next line); also it does not move the cursor.

## Standout Mode

- `standout()` – begin standout mode
- `standend()` – end standout mode

Standout mode is whatever highlighting the terminal can do,
like reverse video, a brighter color, etc. The screen image
stores not only the characters, but also the mode (old
versions of *curses* use 7 bits for the character and the
eighth bit is for standout mode).

Modern versions of *curses* support not only standout mode,
but many more *attributes* like color, background color,
bold, underline, reverse. The visual effect on the terminal
depends on its capabilities, of course.

## Input from Terminal

- `getch()` – get a character from the terminal
- `scanw(fmt,...)` – like `scanf`

There is also `getstr(str)` but it has no way to prevent
buffer overflow, so do not use it.

## Input Modes

- `crmode()` and `nocrmode()` – set control mode, where
  ^S, ^Q, ^C, ^Y go to kernel.
- `raw()` and `noraw()` – set raw input mode, where no processing
  is done by the kernel.
- `echo()` and `noecho()` – set echo mode, where input characters
  are echoed on screen and in window.
- `nl()` and `nonl()` – whether terminal sends RETURN after
  LINEFEED or not.

There are three input modes: **default** (full line buffering and
line editing by kernel), **cbreak** (terminal control characters
^C, ^S, ^Q, !Y are handled by kernel, all others are sent straight
to curses), and **raw** (no processing and no buffering by kernel).

With the typical *curses* application you want cbreak mode.
Default mode means, that *curses* sees input only when the
user hits enter, you hardly want that. And beware that in
raw ^C will *not* interrupt, nor will any other control
characters work (unless handled by your app).

Echo mode is orthogonal to the input modes. For entering
passwords you certainly want `noecho()` mode, otherwise
it depends on your application's requirements.

Newline mode (`nl()`) maps a carriage return to a full newline
on input and output; this mapping occurs at the terminal level,
not inside of curses. With *curses* applications you usually
want `nonl()` as *curses* can then use linefeed to move the
cursor one line down (but not the the left margin).

## Erasing and Clearing

- `delch()` – delete single character, shift left rest of line.
- `deleteln()` – delete a line, shifting lines up from below.
- `clrtoeol()` – erase line from cursor to edge of window.
- `clrtobot()` – erase line(s) from cursor to bottom of window.

These functions delete the character or line(s) under the
logical cursor, shift line tail or lines around, and add
blanks in various spaces.

- `erase()` – erase the window (but don't clear screen).
- `clear()` – reset window to blanks and clear screen.
- `clearok(scr,flag)` – clear screen at next refresh (but don't reset window)

These functions clear an entire window, either by filling it
with blanks, or by requesting a hardware clear on the next
`refresh()`. Use `erase()` to erase (fill with blanks) windows
that cover part of the screen; the hardware clear will always
clear the entire screen. `Clearok()` sets the `_clear` flag
so that the next `refresh()` will clear the screen (if the
window covers the whole screen). `Clear()` fills the screen
image with blanks and sets the `_clear` flag. If you know
a window is *not* full screen, use `erase()`; otherwise
just use `clear()`.

## Using Multiple Windows

- `newwin(rows,cols,y,x)` – create a new window
- `subwin(win,rows,cols,y,x)` – creat a subwindow
- `delwin(win)` – delete a window or subwindow

A new window is completely independen, whereas a subwindow
shares the screen image (character arrays) with its parent
window.

The screen image of a new window is filled with blanks.
Both `newwin()` and `subwin()` return `NULL` if they
cannot allocate memory or if any part of the window is
beyond the screen. Always check the return value!

If *rows* or *cols* is zero, the window extends to the
bottom or margin of the screen or the parent window.
Note that *y,x* are relative to the screen even for
subwindows!

Subwindows can be nested to any depth; however, note
that subwindows have no pointer to their parent window.

Use `delwin()` to free the memory allocated for a window.
For subwindows, this will *not* free the screen image.
However, it *will* free the screen image for real windows,
even if they have subwindows! Be sure to also `delwin()`
all subwindows!

- `overlay(w1,w2)` – overlay *w1* on *w2*, excluding blanks.
- `overwrite(w1,w2)` – overwrite *w1* on *w2*, including blanks.
- `touchwin(win)` – make next refresh rewrite whole window.
- `mvwin(win,y,x)` – move top left corner of *win* to *y,x*.

Both `overlay()` and `overwrite()` copy the screen image
of *w1* to the screen image of *w2*. The top left corners
of the screen images are aligned. Only the overlapping
area is copied. `Overlay()` copies only non-blank characters,
whereas `overwrite()` copies all characters.

The `touchwin()` function is useful to display the window
below another window, or to redraw all subwindows of a
given window with a single `wrefresh()`. Given the `WINDOW`
structure sketched above, `touchwin()` sets `_firstch=0`
and `_lastch=_maxx-1` for all lines of the window, so that
the next refresh will compare the entire screen image
against *curscr*.

## Miscellaneous Functions

- `mvcur(oldy,oldx,newy,newx)` – move the physical cursor.
- `unctrl(ch)` – return a printable string for a control char.
- `scrollok(win,flag)` – permit or prohibit logical scrolling.
- `scroll(win)` – scroll *win* up by one line.

The old position in `mvcur()` helps *curses* optimize the
move. If the position is unknown, use one far from the new
one. Use with great care, as it separates the physical
cursor form the logical cursor! Normally, use `move()`
to move the logical cursor and have `refresh()` move the
physical cursor accordingly.

To use `unctrl()` you must `#include <unctrl.h>`; the
function returns a pointer to a string like `"^X"`.

## Sample Program

```C
#include <ncurses.h>

int
main(void)
{
  WINDOW *win, *sub;

  initscr();  /* initialize curses and terminal */
  cbreak();   /* set terminal to cbreak mode */

  move(2, 3);  /* position cursor at line 2, column 3 */
  addstr("Hello ncurses!");  /* print string at cursor */

  /* Create a new window, draw a frame in its outermost
     character positions, and write some text into it: */
  win = newwin(10, 16, 4, 6);  /* size: 10x16, posn: 6,4 */
  box(win, ACS_VLINE, ACS_HLINE);  /* draw frame */
  sub = subwin(win, 8,14,4+1,6+1);
  mvwaddstr(sub, 1, 1,
    "A very long text that will be wrapped around inside the window.");

  /* Write something to the main window (stdscr), behind
     our subwindow (we will have to update it): */
  mvaddstr(12, 1, "This text will be partially covered.");

  /* Now print some text underlined and in reverse video: */
  attron(A_REVERSE); attron(A_UNDERLINE);
  mvaddstr(15, 3, "Inverted and underlined");
  attroff(A_REVERSE); attroff(A_UNDERLINE);

  /* Finally, try some colors, if available: */
  if (has_colors() && (start_color() == OK)) {
    init_pair(1, COLOR_GREEN, COLOR_RED);  /* create color pair */
    attron(COLOR_PAIR(1));
    mvaddstr(17, 5, "Green text on red background");
    attroff(COLOR_PAIR(1));
  }

  refresh();          /* send stdscr to terminal */
  wrefresh(win);      /* same for our window (and its subwin) */
  (void) getch();     /* wait until key pressed */
  delwin(win);        /* delete our window */
  endwin();           /* restore terminal settings */
  return 0;
}
```

## References

- John Strang, *Programming with curses*, O'Reilly, 1986.
- ncurses at Wikipedia, <https://en.wikipedia.org/wiki/Ncurses>
- Matthias Kalle Dalheimer, *Linux-Wegweiser zur Programmierung
  und Entwicklung*, O'Reilly, 1997.
- Ken Arnold, *Screen Updating and Cursor Movement Optimization:
  A Library Package*, 1984, [local copy](./papers/arnold92curses.pdf)
