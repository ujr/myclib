#include "endian.h"

/* Assumes sizeof(long) > sizeof(char) */

int getendian(void)
{
  long value = 1;
  if (sizeof(long) <= sizeof(char)) return 0;
  return *((char *) &value) == 1 ? ENDIAN_LITTLE : ENDIAN_BIG;
}

#ifdef DEMO
#include <stdio.h>
int main(void)
{
  switch (getendian()) {
  	case ENDIAN_LITTLE: puts("endian: little"); break;
  	case ENDIAN_BIG:    puts("endian: big");    break;
  	default:            puts("endian: error");  break;
  }
  return 0;
}
#endif
