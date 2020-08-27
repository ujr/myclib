# The iniconf.h API

Parsing INI style configuration files.

```C
#include "iniconf.h"
#include <stdio.h>

int handler(
  const char *sect, const char *name, const char *value,
  size_t lineno, void *userdata)
{
  printf("%s:%s=%s (line %zd)\n", sect, name, value, lineno);
  return 0;  /* continue parsing, non-zero to stop */
}

int r;           /* return status */
const char *fn;  /* file name */
FILE *fp;        /* FILE pointer */
const char *sz;  /* zero-terminated string */
void *userdata;  /* e.g. pointer to config struct */

r = iniconf_fn(fn, handler, userdata);
r = iniconf_fp(fp, handler, userdata);
r = iniconf_sz(sz, handler, userdata);

int readerfun(void *state);

r = iniconf_rf(rf, handler, userdata);
```

All four **iniconf_xxx** functions parse an INI style file or
string and invoke the given *handler* for each key/value pair.
The *handler* is passed the current *section*, *name*, and *value*
strings, as well as the current line number and the given *userdata*.
The strings passed to *handler* are owned by the parser, thus the
*handler* must copy any strings it wants to keep.

All functions return `0` or the first non-zero return from *handler*,
or `-1` if there was an unrecoverable error.

The parser is very lenient. Any syntax checking should be done
by the *handler*, which may log meaningful error messages
using the *lineno* passed.

**iniconf_fn:** read from file with the given filename.  
**iniconf_fp:** read from the given `FILE` pointer.  
**iniconf_sz:** read from the given zero-terminated string.  
**iniconf_rf:** invoke the given reader function until it
 returns `EOF` instead of the next input character.

## Format

The format accepted by `iniconf` is described
by this EBNF grammar:

```text
  Config  =  { Section | Entry | Comment }
  Section =  '[' Name ']' NewLine
  Entry   =  Name [ '=' Value ] NewLine
  Comment =  ('#' | ';') Any NewLine
  Name    =  Text
  Value   =  Text
  Text    =  any characters up to the next delimiter
             or unescaped newline, trimmed
  NewLine =  LF | CRLF | CR
```

- Blanks (space or tab) may appear between any tokens and will be ignored.
- Empty or blank lines are allowed and will be skipped.
- Text tokens can be continued to the next line
  by ending them with a backslash.
- Comment lines cannot be continued with a backslash
  (use another comment line).
- The *handler* is not called for entries with an empty name.
- The byte sequence 0xEF, 0xBB, 0xBF at the start of the input
  is skipped, as it is the UTF-8 encoded BOM (Byte Order Mark).

## Example

An INI file could look like this:

```ini
# comment line
global = true

[user]
name = John Smith
mail = john@smith.com
description = Lines can be continued\
 by escaping the newline with a backslash.
; The bachslash and the newline are discarded, the
; two physical lines joined into one logical line.
oops = 42 # this is not a comment!
```

When called on the above INI file, the *handler*
would be called 5 times as follows:

1. `""`, `"global"`, `"true"`, 2
2. `"user"`, `"name"`, `"John Smith"`, 5
3. `"user"`, `"mail"`, `"john@smith.com"`, 6
4. `"user"`, `"description"`, `"Lines can be continued by ..."`, 7
5. `"user"`, `"oops"`, `"42 # this is not a comment!"`, 11
