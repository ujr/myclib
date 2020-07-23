# The myunix.h API

Collection of miscellaneous Unix utilities.

```C
#include "myunix.h"

int daemonize(void);

int open_read(const char *fn);
int open_write(const char *fn);
int open_append(const char *fn);
int open_trunc(const char *fn);
int open_excl(const char *fn);

int fdblocking(int fd);
int fdnonblock(int fd);

int readable(int fd);
int writeable(int fd);
```

**daemonize** makes the caller a daemon: fork, let the parent quit
and the child continue as a new session leader.
A daemon is the name given for long-running processes that do not
have a controlling terminal and therefore run in the background.
Changing the current directory to the root directory ensures that
the daemon does not prevent a mounted filesystem from being unmounted.
For a detailed discussion, see: Stevens 1993, section 13.3.

The **open_xxx** functions are simple wrappers around `open(2)`.
**open_read** opens *fn* for reading only; the file must already exist.
**open_write** opens *fn* for writing only; the file must already exist.
**open_append** opens *fn* for appending only; the file is created
if it does not yet exist.
**open_trunc** creates *fn* if it does not yet exist and truncates
it to length zero if it does already exist; the file descriptor
returned is for writing only.
**open_excl** creates *fn* if it does not yet exist and fails
if it already exists; the file descriptor returned is for writing only.
All functions return the open file descriptor or `-1` on error.
Files are created with mode `0644` (append, trunc) or `0600` (excl);
use `umask(2)` prior to opening for more restricted access permissions.

**fdblocking** sets the file referred to by *fd* to blocking I/O.
Subsequent calls to read(2) and write(2) will block until the
requested data can be read of written (or the operation fails
for another reason). Returns `0` if successful. On error,
returns `-1` and sets *errno* appropriately.

**fdnonblock** sets the file referred to by *fd* to non-blocking I/O.
Subsequent calls to read(2) and write(2) return immediately.
If no data was available or could written, they return an error.

**readable** returns true (non-zero) if the given file descriptor
*fd* is open for reading; oterhwise it returns false (zero).

**writeable** returns true (non-zero) if the given file descriptor
*fd* is open for writing; otherwise it returns false (zero).

## Opening a file

The Unix **open** system call takes two or three arguments.
The first is the *name* of the file to open.
The second are some *flags* telling how to open the file.
The third, if present, is the *mode* for new files and
therefore only useful if the flags tell `open` to
create the file.

The *flags* consist of one of: `O_RDONLY`, `O_WRONLY`, `O_RDWR`;
and any or none of:
`O_APPEND` (write(2) always appends at the end of the file),
`O_CREAT` (the file is created if it does not exist),
`O_EXCL` (with `O_CREAT`: fail if the file already exists),
`O_NONBLOCK` (open in non-blocking mode),
`O_TRUNC` (truncate the file to zero bytes, if it exists
and is regular).
Some other flags might exist on a particular system.

The *mode* is an octal constant that encodes the access
permission bits in the usual way; the *mode* bits are
changed by the calling process's *umask*.

The Unix **creat** system call is the same as `open` with
flags `O_CREAT|O_WRONLY|O_TRUNC`. It is a relic from ancient
days when `open` did not create files and, yes, it is really
spelled `creat`.

The **umask** is an attribute of every process. It is a bit mask
that *clears* corresponding bits in the *mode* argument to `open`
and `creat`. The umask can be set with the `umask` system call.
This call never fails and returns the previous value of the
process's umask.

## Testing for readable/writable

To check if a given file descriptor *fd* is open for reading or writing:
Get fd's flags with `fcntl`; if this fails, the file is not open at all.
Then inspect the flags for `O_RDONLY` or `O_WRONLY` or `O_RDWR`.

## Non-blocking I/O

Some system calls can block the caller forever:
opening a named pipe, reading from sockets, and writing to terminals
are typical examples. Non-blocking I/O means to change the behaviour
of the `read` and `write` system calls so that they always
return immediately, but may with an error stating that they
*would have blocked* if they were called in blocking mode.

File can be put to non-blocking mode by specifying the `O_NONBLOCK`
flag when opening the file or later on using the `fcntl` system call.

If a file is in non-blocking mode and a read or write operation
*would block*, then they return with a negative value and set *errno*
to `EAGAIN` (some systems may use `EWOULDBLOCK` instead—be prepared
to handle both cases). We may then invoke the failed system call
later again or report an error like “service temporarily unavailable”.

Note: `O_NDELAY` is obsolete. It had read(2) return 0 if there was
no data available, but read(2) also returns 0 on end-of-file.
Linux still has `O_NDELAY` for compatibility reasons, but it's
the same as `O_NONBLOCK`.
