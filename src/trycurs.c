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
  delwin(sub);        /* delete (free) our subwindow */
  delwin(win);        /* delete (free) our window */
  endwin();           /* restore terminal settings */
  return 0;
}
