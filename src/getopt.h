/* An implementation of getopt(3) in C89 that is close to POSIX,
 * but not entirely to the standard: (1) an initial colon in the
 * optspec is ignored, (2) there is no error logging and opterr
 * is missing, and (3) a missing argument is reported by returning
 * ':' so that this can be distinguised form an invalid argument.
 *
 * Implemented as a header-only library, therefore optind, optopt,
 * optarg, and getopt() itself are declared static.
 */

#include <string.h>

static int optind = 1;
static int optopt = 0;
static const char *optarg = 0;

static int
getopt(int argc, char *argv[], const char *optspec)
{
  const char dash = '-';
  const char colon = ':';

  static int optpos = 1;
  const char *arg, *spec;

  (void) argc;  /* unused */

  if (optind == 0) {
    optind = 1;
    optpos = 1;
  }

  arg = argv[optind];
  optopt = 0;
  optarg = 0;

  if (!arg) {
    return -1;  /* no more args */
  }

  if (arg[0] != dash || arg[1] == 0) {
    return -1;  /* no more opts */
  }

  if (arg[0] == dash && arg[1] == dash && arg[2] == 0) {
    optind += 1;
    return -1;  /* stop options */
  }

  optopt = arg[optpos];
  spec = strchr(optspec, optopt);

  if (!spec) {
    optind += 1;
    optpos = 1;
    return '?';  /* invalid option */
  }

  if (spec[0] && spec[1] == colon) {
    if (arg[optpos+1]) {
      optarg = arg + optpos + 1;
      optind += 1;
      optpos = 1;
      return optopt;
    }
    if (argv[optind+1]) {
      optarg = argv[optind+1];
      optind += 2;
      optpos = 1;
      return optopt;
    }
    optind += 1;
    optpos = 1;
    return ':';  /* option requires an argument */
  }

  optpos += 1;
  if (!arg[optpos]) {
    optind += 1;
    optpos = 1;
  }
  return optopt;
}

