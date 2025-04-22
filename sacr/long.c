#include "sacr.h"

#include <stdio.h>
#include <string.h>


void add(Number *a, Number *b, Number *res)
{
	int carry = 0;
	for (size_t i = 0; i < lengthof(res->buffer); ++i)
	{
		uint64_t x = (uint64_t)a->buffer[i] + b->buffer[i] + carry;
		carry = x > UINT32_MAX;
		res->buffer[i] = x & UINT32_MAX;
	}
}

void neg(Number *src, Number *res)
{
	Number tmp, one = {.buffer = {1}};
	for (size_t i = 0; i < lengthof(tmp.buffer); ++i)
		tmp.buffer[i] = ~src->buffer[i];
	add(&tmp, &one, res);
}

int cmp(Number *a, Number *b)
{
	for (size_t i = lengthof(a->buffer) - 1;; --i)
		if (a->buffer[i] != b->buffer[i])
			return a->buffer[i] - b->buffer[i];
		else if (i == 0)
			break;
	return 0;
}

void mod(Number *a, Number *b, Number *res)
{
	Number minus;
	neg(b, &minus);

	memset(res->buffer, 0, sizeof(res->buffer));
	for (size_t i = lengthof(a->buffer);; --i)
	{
		for (size_t j = 31;; --j)
		{
			add(res, res, res);
			res->buffer[0] += (a->buffer[i] & (1 << j)) >> j;
			if (cmp(res, b) >= 0)
				add(res, &minus, res);

			if (j == 0)
				break;
		}

		if (i == 0)
			break;
	}
}

void mul(Number *a, Number *b, Number *res)
{
	Number sumnum;
	memcpy(sumnum.buffer, a->buffer, sizeof(sumnum.buffer));

	memset(res->buffer, 0, sizeof(res->buffer));
	for (size_t i = 0; i < lengthof(b->buffer); ++i)
		for (size_t j = 0; j < 32; ++j)
		{
			if (b->buffer[i] & (1 << j))
				add(res, &sumnum, res);
			add(&sumnum, &sumnum, &sumnum);
		}
}
