
#include "iniconf.h"
#include "strbuf.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

typedef struct {
  iniconf_reader reader;
  void *readerstate;
  size_t lineno;
  size_t offset;
  int pushback;
  int cc;
} parser_t;

static int iniconf(parser_t *pp, iniconf_handler handler, void *userstate);

int iniconf_fn(const char *fn, iniconf_handler handler, void *userdata)
{
  int r, saverr;
  FILE *fp = fopen(fn, "r");
  if (!fp) return -1;
  r = iniconf_fp(fp, handler, userdata);
  saverr = errno;
  fclose(fp);
  errno = saverr;
  return r;
}

static int readstream(void *state)
{
  return getc((FILE *) state);
}

int iniconf_fp(FILE *fp, iniconf_handler handler, void *userdata)
{
  return iniconf_rf(readstream, fp, handler, userdata);
}

struct stringstate { const char *sz; size_t i; };

static int readstring(void *state)
{
  struct stringstate *ss = (struct stringstate *) state;
  char c = ss->sz[ss->i];
  if (!c) return EOF;
  ss->i += 1;
  return (int) (unsigned char) c;
}

int iniconf_sz(const char *sz, iniconf_handler handler, void *userdata)
{
  struct stringstate state = { sz, 0 };
  return iniconf_rf(readstring, &state, handler, userdata);
}

int iniconf_rf(iniconf_reader rf, void *rfstate, iniconf_handler handler, void *userdata)
{
  parser_t parser;

  if (!rf) return 0;

  parser.reader = rf;
  parser.readerstate = rfstate;
  parser.lineno = 1;
  parser.offset = 0;
  parser.pushback = -1;

  return iniconf(&parser, handler, userdata);
}

static void trimright(strbuf *sp)
{ /* trim white space from the right, but keep prefix of at least keeplen */
  size_t i;
  const char *s;
  if (!sp) return;
  i = strbuf_len(sp);
  if (i < 1) return;
  i -= 1; /* index of last char */
  s = strbuf_ptr(sp);
  while (i > 0 && isspace((unsigned char) s[i])) --i;
  strbuf_trunc(sp, i+1);
}

static int skipbom(parser_t *pp)
{
  int c;

  c = pp->reader(pp->readerstate);
  if (c != EOF) pp->offset++;
  if (c != 0xEF) goto nobom;

  c = pp->reader(pp->readerstate);
  if (c != EOF) pp->offset++;
  if (c != 0xBB) goto nobom;

  c = pp->reader(pp->readerstate);
  if (c != EOF) pp->offset++;
  if (c != 0xBF) goto nobom;

  return 1; /* skipped a UTF-8 BOM */

nobom:
  pp->pushback = c;
  return 0; /* no BOM */
}

static int advance(parser_t *pp)
{ /* read next char, convert eol to \n, update lineno */
  int c;

  if (pp->pushback < 0) {
    c = pp->reader(pp->readerstate);
    if (c != EOF) pp->offset++;
  }
  else {
    c = pp->pushback;
    pp->pushback = -1;
  }

  if (c == '\r') {
    c = pp->reader(pp->readerstate);
    if (c != EOF) pp->offset++;
    if (c != '\n') {
      pp->pushback = c;
      c = '\n'; /* change CR to LF */
    }
  }
  if (c == '\n')
    pp->lineno++;

  return pp->cc = c;
}

static void skip_blank(parser_t *pp)
{ /* skip blanks and tabs */
  while (pp->cc == ' ' || pp->cc == '\t')
    advance(pp);
}

static void skip_space(parser_t *pp)
{ /* skip white space */
  while (isspace(pp->cc))
    advance(pp);
}

static void skip_line(parser_t *pp)
{ /* skip up to (and including) the next newline */
  while (pp->cc != EOF && pp->cc != '\n')
    advance(pp);
  if (pp->cc == '\n')
    advance(pp);
}

static int is_comment(parser_t *pp)
{
  return pp->cc == '#' || pp->cc == ';';
}

static void scan_text(parser_t *pp, char delim, strbuf *sp)
{ /* scan a piece of "text" up to (but excluding) the delim */
  const int escape = '\\';
  const int newline = '\n';

  skip_blank(pp);

  while (pp->cc != EOF && pp->cc != delim && pp->cc != newline) {
    if (pp->cc == escape) {
      /* discard <backslash newline> and <backslash eof> */
      if (advance(pp) == newline)
        advance(pp);
      else if (pp->cc != EOF)
        sbaddc(sp, escape);
      continue;
    }

    sbaddc(sp, pp->cc);
    advance(pp);
  }

  trimright(sp);
}

/*
  Config  =  { Section | Entry | Comment }
  Section =  '[' Name ']' NewLine
  Entry   =  Name [ '=' Value ] NewLine
  Comment =  ('#' | ';') Any NewLine
  Name    =  Text
  Value   =  Text
  Text    =  any characters up to the next delimiter or unescaped newline, trimmed
  NewLine =  LF | CRLF | CR

  Blanks (space or tab) may appear between any tokens and will be ignored.
  Empty or blank lines are allowed and will be skipped.
  Text tokens can be continued to the next line by ending them with a backslash.
  Comment lines cannot be continued with a backslash (use another comment line).
  Entries with an empty name are not emitted.
*/

static int
iniconf(parser_t *pp, iniconf_handler handler, void *userdata)
{
  strbuf buffer = {0};
  strbuf *sp = &buffer; /* accumulates "section\0name\0value\0" */
  size_t nameofs, valueofs;
  int r = 0;

  assert(pp != NULL);

  skipbom(pp);
  advance(pp);

  /* empty default section */
  sbaddc(sp, '\0');
  nameofs = 1;

  while (pp->cc != EOF) {
    skip_space(pp);
    if (is_comment(pp)) {
      skip_line(pp); /* skip comment line */
    }
    else if (pp->cc == '[') { /* section */
      advance(pp);
      sbtrunc(sp, 0);
      scan_text(pp, ']', sp);
      sbaddc(sp, '\0');
      nameofs = sblen(sp);
      if (pp->cc == ']') advance(pp);
      else skip_line(pp); /* skip newline */
    }
    else { /* name = value */
      size_t lineno = pp->lineno;

      sbtrunc(sp, nameofs);
      scan_text(pp, '=', sp);
      sbaddc(sp, '\0');
      valueofs = sblen(sp);

      if (pp->cc == '=') {
        advance(pp);
        scan_text(pp, 0, sp);
      }
      else sbaddc(sp, '\0'); /* an empty value */

      advance(pp); /* skip the newline */

      if (sbfailed(sp)) {
        r = -1;
        errno = ENOMEM;
        break;
      }

      if (handler && valueofs > nameofs+1) {
        const char *s = sbptr(sp);
        const char *n = &s[nameofs];
        const char *v = &s[valueofs];
        r = handler(s, n, v, lineno, userdata);
        if (r != 0) break; /* stop parsing */
      }
      /* else: do not call handler on empty name */
    }
  }

  sbfree(sp);

  return r;
}
