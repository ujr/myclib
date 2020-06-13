/* Duff's Device */

/* Emit count shorts to given port address */
void emitdata(short *to, short *from, int count)
{
  do { /* assume count > 0 */
    *to = *from++;
  } while (--count > 0);
}

/* Same as above, with loop unrolled */
void unrolled(short *to, short *from, int count)
{
  register int n = (count+7)/8;  /* round up */
  switch (count % 8) {  /* jump to remainder */
  case 0: do { *to = *from++; /*FALLTHRU*/
  case 7:      *to = *from++; /*FALLTHRU*/
  case 6:      *to = *from++; /*FALLTHRU*/
  case 5:      *to = *from++; /*FALLTHRU*/
  case 4:      *to = *from++; /*FALLTHRU*/
  case 3:      *to = *from++; /*FALLTHRU*/
  case 2:      *to = *from++; /*FALLTHRU*/
  case 1:      *to = *from++; /*FALLTHRU*/
          } while (--n > 0);
  }
}

int main()
{
  short target, source[18] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18
  };

  emitdata(&target, source, 18);
  unrolled(&target, source, 18);
  return 0;
}

