CC=gcc

.PHONY: all

all: sjis2utf8 utf82sjis sjisstat

sjis2utf8: sjis2utf8.o sjis.o sjis_unicode.o utf8.o sjis.o
	$(CC) $^ -o $@

utf82sjis: utf82sjis.o sjis.o sjis_unicode.o utf8.o sjis.o
	$(CC) $^ -o $@

sjisstat: sjisstat.o sjis.o sjis_unicode.o utf8.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f sjis2utf8 sjis2utf8.exe utf82sjis utf82sjis.exe sjisstat sjisstat.exe *.o
