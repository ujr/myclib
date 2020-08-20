/* scf.c - Standard Command Format parsing */

#include "scf.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

char *scfname(char **argv)
{
  static const char sep = '/';
  if (!argv || !argv[0]) return 0; /* no arg0 */
  char *s = argv[0];
  register char *p;
  for (p = s; *p; ) {
    if (*p++ == sep) {
      if (*p) s = p;
    }
  }
  return s;
}

void scfinit(struct scfargs *args, char **argv)
{
  assert(args != NULL);
  /* no need for argc if we trust that argv[argc] is NULL */
  args->argv = argv;
  args->subind = 0;
  args->optind = 1;
  args->optopt = 0;
  args->optarg = 0;
  args->errmsg[0] = 0;
}

static int isdashdash(const char *arg)
{
  return arg && arg[0] == '-' && arg[1] == '-' && arg[2] == 0;
}

static int isshortopt(const char *arg)
{
  return arg && arg[0] == '-' && arg[1] != '-' && arg[1] != 0;
}

static int getopttype(char opt, const char *opts)
{
  const char *p;
  if (opt == ':') return -1; /* invalid opt */
  p = strchr(opts, opt);
  if (!p || !p[0]) return -1; /* invalid opt */
  if (p[1] != ':') return 0; /* no arg */
  if (p[2] != ':') return 1; /* required arg */
  return 2; /* optional arg */
}

static int error(struct scfargs *args, const char *msg, char opt)
{
  char *p = args->errmsg;
  size_t n = _SCF_ERRMSG_LEN - 6; /* leave room for ": 'x'\0" */
  while (*msg && n--) *p++ = *msg++;
  *p++ = ':'; *p++ = ' ';
  *p++ = '\''; *p++ = opt; *p++ = '\'';
  *p = 0;
  return '?';
}

int scfopt(struct scfargs *args, const char *opts)
{
  char *opt;
  char *nextarg;

  assert(args != NULL);
  args->optopt = 0;
  args->optarg = 0;
  args->errmsg[0] = 0;

  opt = args->argv[args->optind];

  if (!opt)
    return 0; /* no more opts */

  if (isdashdash(opt)) {
    args->optind += 1;
    return 0; /* no more opts */
  }

  if (!isshortopt(opt))
    return 0; /* no more opts */

  opt += 1 + args->subind; /* skip dash and prev opts */
  args->optopt = *opt;
  switch (getopttype(*opt, opts)) {
  case 0: /* no option arg */
    if (opt[1]) args->subind += 1;
    else { args->subind = 0; args->optind += 1; }
    return *opt;
  case 1: /* required arg */
    args->optind += 1;
    args->subind = 0;
    if (opt[1]) {
      args->optarg = opt+1; /* joined arg */
      return *opt;
    }
    nextarg = args->argv[args->optind];
    if (nextarg) {
      args->optarg = nextarg;
      args->optind += 1;
      return *opt;
    }
    return error(args, "option requires an argument", *opt);
  case 2: /* optional arg */
    args->optind += 1;
    args->optarg = 0;
    if (opt[1]) args->optarg = opt+1; /* joined arg */
    else args->optarg = 0; /* optional arg missing */
    return *opt;
  default: /* invalid option */
    args->optind += 1;
    return error(args, "invalid option", *opt);
  }
}

char *scfarg(struct scfargs *args)
{
  char *arg;
  assert(args != NULL);
  args->optopt = 0;
  args->optarg = 0;
  args->errmsg[0] = 0;
  arg = args->argv[args->optind];
  if (arg) args->optind += 1;
  return arg;
}

#ifdef DEMO

#include <stdio.h>
#define OPTS "abc:d::"
#define UNUSED(x) ((void) x)

int main(int argc, char **argv)
{
  const char *me;
  struct scfargs args;

  int opt;
  char *arg;

  me = scfname(argv);
  scfinit(&args, argv);
  UNUSED(argc);

  printf("The opts arg to scfopt() is: \"%s\"\n", OPTS);

  while ((opt = scfopt(&args, OPTS)) > 0) {
    printf("opt: %c", opt);
    if (opt == '?') printf(" (%s: %s)", me, args.errmsg);
    else if (args.optarg) printf(" = %s", args.optarg);
    printf("\n");
  }

  while ((arg = scfarg(&args)))
    printf("arg: %s\n", arg);

  return 0;
}

#endif
