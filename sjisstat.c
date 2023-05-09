#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sjis.h"
#include "utf8.h"

struct charstat {
	int chr, cnt;
};

struct charstat stats[65536];

void push_charstat(int chr) {
	chr &= 0xffff;
	stats[chr].chr = chr;
	stats[chr].cnt++;
}

int cmpstatfn(const void *p1, const void *p2) {
	return ((struct charstat *)p2)->cnt - ((struct charstat *)p1)->cnt;
}

int main(int argc, char **argv) {
	FILE *in = stdin;

	uint8_t buf[1024];
	int last_byte = 0;
	int l;
	while((l = fread(buf, 1, 1024, in)) > 0) {
		for(int i = 0; i < l; i++) {
			uint8_t b = buf[i];
			if(last_byte == 0 && SJIS_FIRST_CHAR(b)) {
				last_byte = b;
			} else {
				push_charstat((last_byte << 8) | b);
				last_byte = 0;
			}
		}
	}

	qsort(stats, sizeof(stats) / sizeof(stats[0]), sizeof(stats[0]), cmpstatfn);

	for(int i = 0; i < 65536; i++) {
		if(stats[i].cnt == 0) break;
		int chr = stats[i].chr;
		char utfbuf[5];
		if(chr < 0x20) {
			snprintf(utfbuf, 5, "0x%02x", chr);
		} else {
			int l = utf8_encode(sjis_char_to_unicode(chr), utfbuf);
			utfbuf[l] = 0;
		}
		if(chr > 0xff)
			printf("0x%02x 0x%02x %d %s\n", chr >> 8, chr & 0xff, stats[i].cnt, utfbuf);
		else
			printf("     0x%02x %d %s\n", chr & 0xff, stats[i].cnt, utfbuf);
	}

	return 0;
}
