#ifndef __SACR_H__
#define __SACR_H__

#include <stddef.h>
#include <stdint.h>


struct Number
{
	uint32_t buffer[128];
};
typedef struct Number Number;

struct Key
{
	Number e, d, n;
};

#define lengthof(x) (sizeof(x) / sizeof(*(x)))

void add(Number *a, Number *b, Number *res);
void neg(Number *src, Number *res);
int cmp(Number *a, Number *b);
void mod(Number *a, Number *b, Number *res);
void mul(Number *a, Number *b, Number *res);

int decrypt(char *input, char *output, struct Key *key, size_t inputsize, size_t outputsize);
int encrypt(char *input, char *output, struct Key *key, size_t inputsize, size_t outputsize);
int generate(void);

#endif /* __SACR_H__ */
