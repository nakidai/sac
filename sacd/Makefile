RM ?= rm -f
PREFIX ?= /usr/local/
BINDIR ?= ${PREFIX}/bin
MANDIR ?= ${PREFIX}/man


.PHONY: all
all: sacd

install: all
	install -d ${BINDIR} ${MANDIR}/man1
	install -m755 sacd ${BINDIR}
	install -m644 sacd.1 ${MANDIR}/man1

uninstall:
	${RM} ${BINDIR}/sacd
	${RM} ${MANDIR}/man1/sacd.1

.PHONY: clean
clean:
	${RM} sacd

README: ttb.1
	mandoc -Ios=ttb -Tascii sacd.1 | col -b > README
