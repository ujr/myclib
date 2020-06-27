/* Showing and testing the limits.h macros
 *
 * Problems raise a compile-time error.
 * At run-time, values are just shown.
 */

#include <limits.h>
#include <stdio.h>

int main()
{
  printf("Values from <limits.h>   (acceptable ANSI C minimum magnitudes)\n\n");
  printf(" CHAR_BIT = %11i  (at least 8)\n", CHAR_BIT);
  printf(" CHAR_MIN = %11i  (0 or SCHAR_MIN)\n", CHAR_MIN);
  printf(" CHAR_MAX = %11i  (UCHAR_MAX or SCHAR_MAX)\n", CHAR_MAX);
  printf("SCHAR_MIN = %11i  (-127)\n", SCHAR_MIN);
  printf("SCHAR_MAX = %11i  (+127)\n", SCHAR_MAX);
  printf("UCHAR_MAX = %11u  ( 255)\n\n", UCHAR_MAX);

  printf(" SHRT_MIN = %11i  (-32767)\n", SHRT_MIN);
  printf(" SHRT_MAX = %11i  (+32767)\n", SHRT_MAX);
  printf("USHRT_MAX = %11u  ( 65535)\n\n", USHRT_MAX);

  printf("  INT_MIN = %11i  (-32767)\n", INT_MIN);
  printf("  INT_MAX = %11i  (+32767)\n", INT_MAX);
  printf(" UINT_MAX = %11u  ( 65535)\n\n", UINT_MAX);

  printf(" LONG_MIN = %20li  (-2147483647)\n", LONG_MIN);
  printf(" LONG_MAX = %20li  (+2147483647)\n", LONG_MAX);
  printf("ULONG_MAX = %20lu  ( 4294967295)\n\n", ULONG_MAX);

  printf("sizeof char/short/int/long: %d/%d/%d/%d bytes\n\n",
    (int) sizeof(char), (int) sizeof(short), (int) sizeof(int), (int) sizeof(long));

#if CHAR_BIT < 8 || CHAR_MAX < 127 || 0 < CHAR_MIN \
    || CHAR_MAX != SCHAR_MAX && CHAR_MAX != UCHAR_MAX
# error bad char properties
#endif

#if INT_MAX < 32767 || -32767 < INT_MIN || INT_MAX < SHRT_MAX
# error bad int properties
#endif

#if LONG_MAX < 2147483647 || -2147483647 < LONG_MIN || LONG_MAX < INT_MAX
# error bad long properties
#endif

#if SCHAR_MAX < 127 || -127 < SCHAR_MIN
# error bad signed char properties
#endif

#if SHRT_MAX < 32767 || -32767 < SHRT_MIN || SHRT_MAX < SCHAR_MAX
# error bad short properties
#endif

#if UCHAR_MAX < 255 || UCHAR_MAX / 2 < SCHAR_MAX
# error bad unsigned char properties
#endif

#if UINT_MAX < 65535 || UINT_MAX / 2 < INT_MAX || UINT_MAX < USHRT_MAX
# error bad unsigned int properties
#endif

#if ULONG_MAX < 4294967295 || ULONG_MAX / 2 < LONG_MAX || ULONG_MAX < UINT_MAX
# error bad unsigned long properties
#endif

#if USHRT_MAX < 65535 || USHRT_MAX / 2 < SHRT_MAX || USHRT_MAX < UCHAR_MAX
# error bad unsigned short properties
#endif

  return 0;
}
