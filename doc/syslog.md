# The Syslog Facility

Unix systems provide this interface for applications,
mostly “daemons”, to write to the system log.

```C
#include <syslog.h>

void openlog(char *tag, int option, int facility);
void syslog(int priority, char *format, ...);
void closelog(void);
```

The use of **openlog** and **closelog** is optional, but
`openlog(3)` may be handy to specify how syslog should log.
The *tag* will be prepended to each log message (usually the
program's name); *option* is a combination of
`LOG_CONS` (if there's an error logging, write to the console),
`LOG_PERROR` (also write to stderr),
`LOG_PID` (include PID in log messages),
and a few others (see the man page).
Finally, *facility* is a default facility in case none is
provided in subsequent calls to `syslog(3)`.

Calls to **syslog** create log messages that are sent to
the syslog daemon, syslogd(8). The parameter *priority*
is a combination of a *facility* and a *level* (see below),
and *format* is as in printf(3) with the addition that `%m`
will be replaced by the result from `strerror(errno)`.

## Facilities and Levels

In syslog speak, a *facility* describes the origin of a log
message in terms of a class of programs or daemons. Possible
values are:

- `LOG_KERNEL` (for kernel-generated messages),
- `LOG_AUTHPRIV` (authorisation and security),
- `LOG_CRON` (cron and at daemons),
- `LOG_FTP` (the ftp daemon),
- `LOG_LPR` (the printing system),
- `LOG_MAIL` (the mail system),
- `LOG_NEWS` (the news system),
- `LOG_UUCP` (the UUCP system),
- `LOG_SYSLOG` (syslog-internal messages),
- `LOG_DAEMON` (any other daemon),
- `LOG_USER` (generic messages – **this is the default**),
- `LOG_LOCAL0` through `LOG_LOCAL7` (for local use)

The **level** specifies the severity of a message.
Possible values are (from most severe to least):

- `LOG_EMERG`
- `LOG_ALERT`
- `LOG_CRIT`
- `LOG_ERR`
- `LOG_WARNING`
- `LOG_NOTICE`
- `LOG_INFO`
- `LOG_DEBUG`

Note that the syslog daemon can be configured to dispatch incoming
log messages based on both *facility* and *level*.
The config information is usually in */etc/syslog.conf*.

## Example

The following lines are taken from the venerable *lprps*
printer filter. During initialisation it says:

```C
openlog("lprps", LOG_PID, LOG_LPR);
```

This causes all log messages to be prepended with "lprps" and
the calling process's PID to be included in the message; the
facility is sensibly set to LPR, the printing system.

Later on, the program generates log messages like that:

```C
syslog(LOG_ERR, "open error for %s: %m", filename);
```

Note that no facility is specified (this was done in openlog),
just a severity (or log level) and the actual message as would
be done in a call to printf.

## Using syslog from the shell

Linux systems include a utility called **logger** that
logs its arguments to syslog. Dan J. Bernstein (the qmail
author) provides a similar tool called **splogger** in
his daemontools package. Both tools make, of course, use of
the functions described above.

**Note:** Bernstein and other authors warn that the
syslog mechanism is unreliable: there is no guarantee that
messages will be logged (and syslog is slow).
