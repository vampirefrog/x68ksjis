#include "utf8.h"

int utf8_encode(int c, char *buf) {
	if(c < 0x80) {
		buf[0] = c;
		return 1;
	} else if(c < 0x800) {
		buf[0] = 192 + c / 64;
		buf[1] = 128 + c % 64;
		return 2;
	} else if(c - 0xd800u < 0x800) {
	} else if(c < 0x10000) {
		buf[0] = 224 + c / 4096;
		buf[1] = 128 + c / 64 % 64;
		buf[2] = 128 + c % 64;
		return 3;
	} else if (c<0x110000) {
		buf[0] = 240+c/262144;
		buf[1] = 128+c/4096%64;
		buf[2] = 128+c/64%64;
		buf[3] = 128+c%64;
		return 4;
	}

	buf[0] = c;
	return 1;
}

int utf8_fputc(int c, FILE *f) {
	char buf[4];
	int chars = utf8_encode(c, buf);
	if(chars) {
		if(fwrite(buf, chars, 1, f) < 1)
			return EOF;
		return c;
	}
	return EOF;
}
