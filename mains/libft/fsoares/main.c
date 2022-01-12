#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "libft.h"

#define create_test_ctype(fn) int test_##fn(void) \
{ \
  int c; \
  int res = 1; \
  for(c = 0; c <= 0xff; c++) { \
    if (!fn(c) != !ft_##fn(c)) { \
      printf(#fn "(%i) std: %i, yours: %i\n", c, \
        fn(c), ft_##fn(c)); \
      res = 0; \
    } \
  } \
  if (!fn(EOF) != !ft_##fn(EOF)) { \
    printf(#fn "(EOF) std: %i, yours: %i\n", \
      fn(EOF), ft_##fn(EOF)); \
    res = 0; \
  } \
  return res; \
}

#define test(fn) if (!test_##fn()) { \
    printf("Error: " #fn " is different!\n"); \
  } else { \
    printf(#fn ": OK\n"); \
  }

void print_mem(void *ptr, int size);

void rand_str(char *dest, int len)
{
  dest[len - 1] = '\0';
  for (int i = 0; i < len - 1; i++) {
    dest[i] = rand() % 0xff;
  }
}

create_test_ctype(isalpha)
create_test_ctype(isdigit)
create_test_ctype(isalnum)
create_test_ctype(isascii)
create_test_ctype(isprint)

#define REPETITIONS 1000

char *escape_str(char *dest, char *src) {
  int i, j;
  for (i = 0, j = 0; src[i]; i++, j++) {
    if(isprint(src[i])) {
      dest[j] = src[i];
    } else {
      sprintf(dest + j, "\\%02x", (unsigned char)src[i]);
      j += 2;
    }
  }
  dest[j] = '\0';
  return dest;
}

int compare_strlen(char *str, char *escaped) {
  if (strlen(str) != ft_strlen(str)) {
    printf("strlen('%s') std: %li, yours: %li\n",
      escape_str(escaped, str),
      strlen(str),
      ft_strlen(str)
    );
    return 0;
  }
  return 1;
}

int test_strlen(void) {
  char str[100];
  char escaped[400];
  int res = 1;

  for (int n = 0; n < REPETITIONS; n++) {
    rand_str(str, rand() % 98 + 1);
    res = compare_strlen(str, escaped) && res;
  }
  res = compare_strlen("", escaped) && res;
  str[0] = EOF;
  str[1] = '\0';
  res = compare_strlen("", escaped) && res;
  return res;
}

void reset(void *m1, void *m2, int size) {
  memset(m1, 0x30, size);
  memset(m2, 0x30, size);
}

int same_ptr(void *ptr1, void *ptr2) {
  if (ptr1 != ptr2) {
    printf("Error: memset returned %p but should have returned %p\n",
      ptr1, ptr2
    );
    return 0;
  }
  return 1;
}

int same_mem(void *s, void *r, int size) {
  int equal = 1;
  char *std = s;
  char *res = r;
  for (int i = 0; i < size; i++) {
    if (std[i] != res[i]) {
      equal = 0;
    }
  }
  if (!equal) {
    printf("memset difference\nstd:\n");
    print_mem(std, size);
    printf("res:\n");
    print_mem(res, size);
  }
  return equal;
}

#define MEM_SIZE 0xff

int single_test_memset(char *m, char *ms, int c, int size) {
   char *res;
   char *res_std;

   reset(m, ms, MEM_SIZE);
   res_std = memset(ms, c, size);
   res = ft_memset(m, c, size);
   return (same_ptr(res, m)
       && same_mem(res_std, res, MEM_SIZE));
}

int test_memset(void) {
   char mem[MEM_SIZE];
   char mem_std[MEM_SIZE];

   int res = 1;

   res = single_test_memset(mem, mem_std, 'x', 0) && res;
   res = single_test_memset(mem, mem_std, 'g', 12) && res;
   res = single_test_memset(mem, mem_std, '5', -1) && res;
   res = single_test_memset(mem, mem_std, 300, 12) && res;
   res = single_test_memset(mem, mem_std, 257, 19) && res;

   int m2[MEM_SIZE];
   int ms2[MEM_SIZE];

   reset(m2, ms2, MEM_SIZE);
   int *rs = memset(ms2 + 5, 143562, 4);
   int *r = ft_memset(m2 + 5, 143562, 4);
   res = (same_ptr(r - 5, m2)
       && same_mem(rs - 5, r - 5, MEM_SIZE)) && res;
   return res;
}

int main()
{
  test(isalpha)
  test(isdigit)
  test(isalnum)
  test(isascii)
  test(isprint)
  test(strlen)
  test(memset)
  return 0;
}