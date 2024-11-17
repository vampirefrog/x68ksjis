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

int utf8_fgetc(FILE *f) {
	int state = 0;
	int codepoint = 0;
	int b = 0;
	while((b = fgetc(f)) != EOF) {
		if(state == 0) {
			if (b < 0x80) {  // Single-byte (ASCII)
				return b;
			} else if ((b & 0xE0) == 0xC0) {  // Start of 2-byte sequence
				codepoint = b & 0x1F;
				state = 1;
			} else if ((b & 0xF0) == 0xE0) {  // Start of 3-byte sequence
				codepoint = b & 0x0F;
				state = 2;
			} else if ((b & 0xF8) == 0xF0) {  // Start of 4-byte sequence
				codepoint = b & 0x07;
				state = 3;
			} else {
				return -2;
			}
		} else {
			if((b & 0xC0) == 0x80) {  // Valid continuation byte
				codepoint = (codepoint << 6) | (b & 0x3F);
				state--;  // Move to the next expected byte or finish
				if(state == 0)
					return codepoint;
			} else {
				return -2;
			}
		}
	}
	return EOF;
}
