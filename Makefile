all: sacd sacr
SUBS += sacd
SUBS += sacr


.PHONY: all clean
all clean install:
	@for sub in ${SUBS}; do \
		echo "make -C $$sub $@"; make -C $$sub $@; \
	done

README: sac.7
	mandoc -Ios=ttb -Tascii sac.7 | col -b > README
