#include "sacr.h"

#include <stdio.h>


void add(const Number *a, const Number *b, Number *res)
{
	uint64_t x, carry = 0;
	for (size_t i = 0; i < lengthof(res->buffer); ++i)
	{
		x = (uint64_t)a->buffer[i] + b->buffer[i] + carry;
		carry = x >> 32;
		res->buffer[i] = x;
	}
}

void neg(const Number *src, Number *res)
{
	for (size_t i = 0; i < lengthof(res->buffer); ++i)
		res->buffer[i] = ~src->buffer[i];
	add(res, &Number(1), res);
}

void shr(const Number *a, size_t b, Number *res)
{
	int64_t x, carry = 0;
	for (size_t i = lengthof(res->buffer) - 1;; --i)
	{
		x = ((uint64_t)a->buffer[i] << (32 - b)) + carry;
		carry = x << 32;
		res->buffer[i] = x >> 32;

		if (i == 0)
			break;
	}
}

int cmp(const Number *a, const Number *b)
{
	for (size_t i = lengthof(a->buffer) - 1;; --i)
		if (a->buffer[i] != b->buffer[i])
			return a->buffer[i] - b->buffer[i];
		else if (i == 0)
			break;
	return 0;
}

static int isn(const Number *n)
{
	for (size_t i = 0; i < lengthof(n->buffer); ++i)
		if (n->buffer[i])
			return 1;
	return 0;
}

static size_t log2floor(const Number *n)
{
	Number test = *n;
	for (size_t size = lengthof(test.buffer) * 32;; --size)
	{
		if (test.buffer[lengthof(test.buffer) - 1] & 0x80000000)
			return size;
		else
			add(&test, &test, &test);

		if (size == 0)
			return size;
	}
}

void mod(const Number *a, const Number *b, Number *res)
{
	Number shiftedb;

	*res = *a;
	while (cmp(res, b) >= 0)
	{
		shiftedb = *b;

		while (cmp(&shiftedb, res) <= 0)
			add(&shiftedb, &shiftedb, &shiftedb);
		shr(&shiftedb, 1, &shiftedb);

		neg(&shiftedb, &shiftedb);
		add(res, &shiftedb, res);
	}
}

void mul(const Number *a, const Number *b, Number *res)
{
	Number sumnum = *a, mulnum = *b;

	*res = Number(0);
	for (size_t i = 0; i < lengthof(mulnum.buffer); ++i)
		for (size_t j = 0; j < 32; ++j)
		{
			if (mulnum.buffer[i] & (1 << j))
				add(res, &sumnum, res);
			add(&sumnum, &sumnum, &sumnum);
		}
}

void expmon(const Number *a, const Number *b, const Number *m, Number *res)
{
	Number base = *a, left = *b;
	*res = Number(1);

	if (cmp(b, &Number(0)) == 0)
		return;

	while (cmp(&left, &Number(0)) != 0)
	{
		if (left.buffer[0] & 1)
		{
			left.buffer[0] ^= 1;
			mul(res, &base, res), mod(res, m, res);
		} else
		{
			shr(&left, 1, &left);
			mul(&base, &base, &base), mod(&base, m, &base);
		}
	}
}

void expmod(const Number *a, const Number *b, const Number *m, Number *res)
{
	Number base = *a;
	*res = Number(1);

	size_t iters = log2floor(b);
	for (size_t i = 0; i < iters; ++i)
	{
		if (b->buffer[i >> 5] & (1 << (i & 31)))
			mul(res, &base, res), mod(res, m, res);
		mul(&base, &base, &base), mod(&base, m, &base);
	}
}
