/** Scan an unsigned long in decimal notation */
int
scanulong(const char *s, unsigned long *pv)
{
  const char *p;
  unsigned long v;

  if (!s) return 0;

  for (p=s, v=0; '0'<=*p && *p<='9'; p++) {
    v = 10 * v + (*p - '0');
  }

  if (pv) *pv = v;
  return p - s;
}
