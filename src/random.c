/* Print random numbers from rand(3) */
/* Usage: random [-a] [N [seed]] */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pseudo random numbers:

  The standard library provides rand(3) and srand(3).

  The alternatives randseed() and randint() below are
  a linear congruential generator based on section 7.1
  in Numerical Recipes in C (www.nr.com) with constants
  m (modulus), a (multiplier), and c (increment) taken
  from the table on page 285. Result r is 0 <= r < limit.

  The 3rd edition of NR says linear congruential generators
  are a relict from the past, new better methods exist.
  May investigate some day...
*/

static unsigned long r = 1; /* 0 <= r < m */
void randseed(unsigned seed) { r = seed; }
int randint(int limit) {
  static unsigned a=9301, c=49297, m=233280;
  r = (r * a + c) % m;
  return (limit*r)/m; /* r < limit */
  // return lo + ((hi-lo+1)*r)/m; /* lo <= r <= hi */
}

typedef int randfun(void);
typedef void seedfun(unsigned);
static int randalt(void) { return randint(RAND_MAX); }

int main(int argc, char **argv)
{
  const unsigned default_seed = 1;
  const int default_count = 5;

  long s = -1;
  unsigned seed;
  int i, n = 5;
  int show = 0;
  const char *me = "random";

  randfun *rf = rand;
  seedfun *sf = srand;

  if (argv && argv[0] && *argv[0]) {
    me = argv[0];
  }

  if (argc > 1 && argv && !strcmp(argv[1], "-a")) {
    rf = randalt;
    sf = randseed;
    argc--;
    argv++;
  }

  switch (argc) {
    case 3:
      s = atol(argv[2]);
      if (s < 0 || s > UINT_MAX) {
        fprintf(stderr, "Error: seed out of range 0..%u, ignoring\n", UINT_MAX);
        return 1;
      }
      /* FALLTHRU */
    case 2:
      n = atoi(argv[1]);
      show = 1;
      /* FALLTHRU */
    case 1:
      break;
    default:
      fprintf(stderr, "Usage: %s [-a] [N [seed]]\n", me);
      fprintf(stderr, "Print N (default: %d) results from rand(3)\n", default_count);
      fprintf(stderr, "Option -a selects an alternative LCG (see source)\n");
      return 1;
  }

  seed = s < 0 ? default_seed : (unsigned) s;

  if (show) {
    fprintf(stderr, "N = %u, seed = %u, RAND_MAX = %d\n",
      n, seed, RAND_MAX);
  }

  sf(seed);

  for (i = 0; i < n; i++) {
    int r = rf();
    printf("%d\n", r);
  }

  return 0;
}
