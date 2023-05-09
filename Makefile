CC=gcc

.PHONY: all

all: sjis2utf8 sjisstat

sjis2utf8: sjis2utf8.o sjis.o sjis_unicode.o utf8.o
	$(CC) $^ -o $@

sjisstat: sjisstat.o sjis.o sjis_unicode.o utf8.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f sjis2utf8 sjis2utf8.exe sjisstat sjisstat.exe *.o
