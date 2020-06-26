/** Scan leading blanks (space and tab) */
int
scanblank(const char *s)
{
  static const char blank = ' ';
  static const char tab = '\t';

  const char *p;
  if (!s) return 0;
  for (p=s; *p == blank || *p == tab; p++) {}
  return p - s;
}
