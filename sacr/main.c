#include "sacr.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define ENCRYPT_INPUT 245
#define DECRYPT_INPUT 256

int main(int argc, char **argv)
{
	enum { ENCRYPT, DECRYPT } mode = DECRYPT;

	if (!argv[1])
		return generate();

	if (argv[1][0] == '-')
		if (!strcmp(argv++[1], "-e"))
			mode = ENCRYPT;
		else
			errx(1, "invalid usage, see manpage");

	if (!argv[1])
		errx(1, "key file must be provided");

	char input[mode == ENCRYPT ? ENCRYPT_INPUT : DECRYPT_INPUT];
	char output[mode == ENCRYPT ? DECRYPT_INPUT : ENCRYPT_INPUT];

	struct Key key;

	for (;;)
	{
		size_t readedall = 0;
		do
		{
			ssize_t readed = read(0, input + readedall, sizeof(input) - readedall);
			if (readed == -1)
				err(1, "read()");
			readedall += readed;
		} while (readedall < sizeof(input));

		int res = (mode == ENCRYPT ? encrypt : decrypt)(input, output, &key, sizeof(input), sizeof(output));
		if (res == -1)
			errx(1, "%1$scrypt: unable to %1$scrypt", mode == ENCRYPT ? "en" : "de");

		size_t writtenall = 0;
		do
		{
			ssize_t written = write(1, output + writtenall, sizeof(output) - writtenall);
			if (written == -1)
				err(1, "write()");
			writtenall += written;
		} while (writtenall < sizeof(output));
	}
}
