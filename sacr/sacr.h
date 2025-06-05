#ifndef __SACR_H__
#define __SACR_H__

#include <stddef.h>
#include <stdint.h>


struct Number
{
	uint32_t buffer[16];
};
typedef struct Number Number;

struct Key
{
	Number e, d, n;
};

#define lengthof(x) (sizeof(x) / sizeof(*(x)))

#define Number(...) ((Number){.buffer={__VA_ARGS__}})

void add(const Number *a, const Number *b, Number *res);
void neg(const Number *src, Number *res);
void shr(const Number *a, size_t b, Number *res);
int cmp(const Number *a, const Number *b);
void mod(const Number *a, const Number *b, Number *res);
void mul(const Number *a, const Number *b, Number *res);
void expmod(const Number *a, const Number *b, const Number *m, Number *res);
void print(const Number *n);

int decrypt(char *input, char *output, struct Key *key, size_t inputsize, size_t outputsize);
int encrypt(char *input, char *output, struct Key *key, size_t inputsize, size_t outputsize);
int generate(void);


#endif /* __SACR_H__ */
