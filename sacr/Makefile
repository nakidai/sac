RM ?= rm -f
PREFIX ?= /usr/local/
BINDIR ?= ${PREFIX}/bin
MANDIR ?= ${PREFIX}/man

CFLAGS += -O3

OBJS += decrypt.o
OBJS += encrypt.o
OBJS += generate.o
OBJS += long.o
OBJS += main.o


.PHONY: all
all: sacr

${OBJS}: sacr.h

sacr: ${OBJS}
	${CC} -o sacr ${LDFLAGS} ${OBJS} ${LDLIBS}

install: all
	install -d ${BINDIR} ${MANDIR}/man1
	install -m755 sacr ${BINDIR}
	install -m644 sacr.1 ${MANDIR}/man1

uninstall:
	${RM} ${BINDIR}/sacr
	${RM} ${MANDIR}/man1/sacr.1

.PHONY: clean
clean:
	${RM} ${OBJS} sacr

README: ttb.1
	mandoc -Ios=ttb -Tascii sacr.1 | col -b > README
