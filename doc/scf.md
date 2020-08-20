# The scf.h API

Parsing command lines in Unix “Standard Command Format” (SCF).
Indeed, there is no “standard command format”, only
traditions and conventions. However, POSIX provides some
“Utility Syntax Guidelines” (see below).

The routines here are essentially the same as POSIX
getopt(3) but avoiding global static state.

```C
#include "scf.h"

struct scfargs args;

char *scfname(char *argv);
void scfinit(struct scfargs *args, char **argv);
int scfopt(struct scfargs *args, const char *opts);
char *scfarg(struct scfargs *args);
```

**scfname:** return the substring after the last slash
in *argv[0]*, that is, the basename of the zeroth argument.
Return null if *argv* or *argv[0]* is null; this means that
the program was not invoked as it would have been by a typical
shell.

**scfinit:** initialize a `struct scfargs` from the given
argument vector for subsequent use by `scfopt` and `scfarg`.

**scfopt:** return the next option and update the *args*
structure; return `0` if there are no more options; return
`'?'` if there is an invalid option.
The option character itself is also available in `args->optopt`.
The option's argument, if any, is available in `args->optarg`.
Each alphanumeric character in *opts* is a valid option;
if it is followed by `:` then this option has a required
argument, and if it is followed by `::` then this option
has an optional argument.

**scfarg:** return the next argument and upate the *args*
structure; return null if there are no more arguments.
To be called only after option arguments, if any, have been
processed with `scfopt`.

## Usage example

```C
#include <stdio.h>
#include <stdlib.h>
#include "scf.h"

int main(int argc, char **argv)
{
  const char *me;
  struct scfargs args;

  int aflag = 0;
  char *barg = "foo";
  int carg = 0;

  int opt;
  char *arg;

  me = scfname(argv);
  scfinit(&args, argv);

  while ((opt = scfopt(&args, "ab:c::")) > 0) {
    switch (opt) {
    case 'a': aflag = 1; break;
    case 'b': barg = args.optarg; break;
    case 'c': carg = args.optarg ? atoi(args.optarg) : 0; break;
    case '?':
      fprintf(stderr, "%s: %s\n", me, args.errmsg);
      return 1;
    }
  }

  while ((arg = scfarg(&args)))
    printf("arg: %s\n", arg);

  return 0;
}
```

## Utility Syntax Guidelines

POSIX provides some “Utility Syntax Guidelines”
in [section 12 on Utility Conventions][posix-12].
Here is a short summary:

- tool names should be 2 to 9 lower case letters or digits
- each option name is a single alphanumeric character
  preceded by the `-` character, e.g. `-v`
- multiple options without arguments can be grouped behind
  one `-` character, e.g. `-av` for `-a -v`
- option arguments are in the following command argument
  (or immediately follow their option character)
- option arguments should not be optional (though this
  is supported by both getopt(3) and scf)
- all option arguments should precede all operand arguments
- the first `--` argument that is not an option argument
  indicates end of options; the following command arguments
  are operands, even if they begin with the `-` character
- the order of option arguments should not matter
  (the order of operand arguments often does matter,
  think of `cat` for file concatenation)
- the `-` operand may be used to refer to standard input
  or standard output

Not all Unix utilities follow these guidelines. Well-known
examples are **tar** (all options concatenated in the first
argument and without a dash) and **dd** (all arguments of
the form `key=value`).

[posix-12]: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html

Since 2004-03-20
(originally with global static state like getopt).
