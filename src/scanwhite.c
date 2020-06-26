/** Scan leading white space */
int
scanwhite(const char *s)
{
  const char *p;
  if (!s) return 0;
  for (p=s; ; p++) switch (*p) {
    case ' ':  /* blank */
    case '\f': /* form feed */
    case '\n': /* newline */
    case '\r': /* carriage return */
    case '\t': /* horizontal tab */
    case '\v': /* vertical tab */
      break;
    default: goto done;
  }
done:
  return p - s;
}
