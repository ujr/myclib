#ifndef SCF_H
#define SCF_H

/* scf.h - standard command format parsing */

#define _SCF_ERRMSG_LEN 64

struct scfargs {
  char **argv;
  int optind;
  int subind;
  int optopt;
  char *optarg;
  char errmsg[_SCF_ERRMSG_LEN];
};

char *scfname(char **argv);
void scfinit(struct scfargs *args, char **argv);
int scfopt(struct scfargs *args, const char *opts);
char *scfarg(struct scfargs *args);

#endif
