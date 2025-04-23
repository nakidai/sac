#include "sacr.h"

#include <stdio.h>
#include <string.h>


void add(const Number *a, const Number *b, Number *res)
{
	int carry = 0;
	for (size_t i = 0; i < lengthof(res->buffer); ++i)
	{
		uint64_t x = (uint64_t)a->buffer[i] + b->buffer[i] + carry;
		carry = x > UINT32_MAX;
		res->buffer[i] = x & UINT32_MAX;
	}
}

void neg(const Number *src, Number *res)
{
	Number tmp, one = {.buffer = {1}};
	for (size_t i = 0; i < lengthof(tmp.buffer); ++i)
		tmp.buffer[i] = ~src->buffer[i];
	add(&tmp, &one, res);
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

void mod(const Number *a, const Number *b, Number *res)
{
	Number minus, cmpnum;
	memcpy(cmpnum.buffer, b->buffer, sizeof(cmpnum.buffer));
	neg(b, &minus);

	memset(res->buffer, 0, sizeof(res->buffer));
	for (size_t i = lengthof(a->buffer);; --i)
	{
		for (size_t j = 31;; --j)
		{
			add(res, res, res);
			res->buffer[0] += (a->buffer[i] & (1 << j)) >> j;
			if (cmp(res, &cmpnum) >= 0)
				add(res, &minus, res);

			if (j == 0)
				break;
		}

		if (i == 0)
			break;
	}
}

void mul(const Number *a, const Number *b, Number *res)
{
	Number sumnum, mulnum;
	memcpy(sumnum.buffer, a->buffer, sizeof(sumnum.buffer));
	memcpy(mulnum.buffer, b->buffer, sizeof(mulnum.buffer));

	memset(res->buffer, 0, sizeof(res->buffer));
	for (size_t i = 0; i < lengthof(mulnum.buffer); ++i)
		for (size_t j = 0; j < 32; ++j)
		{
			if (mulnum.buffer[i] & (1 << j))
				add(res, &sumnum, res);
			add(&sumnum, &sumnum, &sumnum);
		}
}
