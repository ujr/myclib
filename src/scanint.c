/** Scan an int in decimal notation with optional sign */
int
scanint(const char *s, int *pv)
{
  const char *p;
  int sign;
  int v;

  if (!s) return 0;

  sign = 0;

  switch (*s) {
    case '-': sign=-1; ++s; break;
    case '+': sign=+1; ++s; break;
  }

  /* compute -v to get INT_MIN without overflow, but
     input beyond INT_MIN..INT_MAX silently overflows */
  for (p=s, v=0; '0'<=*p && *p<='9'; p++) {
    v = 10 * v - (*p - '0');
  }

  if (p == s) return 0; /* no digits */

  if (pv) *pv = sign<0 ? v : -v;
  return (p - s) + (sign ? 1 : 0);
}
