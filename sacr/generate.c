#include "sacr.h"

#include <stdio.h>
#include <stddef.h>

#include <err.h>
#include <fcntl.h>
#include <unistd.h>


#define ISPRIME_TESTCOUNT 1

static void gennum(Number *res)
{
	*res = Number(0);
	static int fd = 0;
	if (!fd)
	{
		fd = open("/dev/random", 0);
		if (fd == -1)
			err(1, "open()");
	}
	read(fd, res->buffer, sizeof(res->buffer) / 4);
}

static int isprime(const Number *n)
{
	Number a, n1, res;
	neg(&Number(1), &n1);
	add(n, &n1, &n1);

	for (size_t i = 0; i < ISPRIME_TESTCOUNT; ++i)
	{
		gennum(&a), mod(&a, n, &a);
		expmod(&a, &n1, n, &res);
		if (cmp(&res, &Number(1)) != 0)
			return 0;
	}

	return 1;
}

static void genprime(Number *res)
{
	gennum(res);
	res->buffer[0] |= 1;
	for(;;)
		if (printf("%u\n", res->buffer[0]), isprime(res))
			break;
		else
			add(res, &Number(2), res);
}

int generate(void)
{
	Number res;
	genprime(&res);
	print(&res);
	return 0;
}
