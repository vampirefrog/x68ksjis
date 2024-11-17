#include <stdio.h>
#include <string.h>

#include "sjis.h"
#include "utf8.h"

int sjis_strlen(uint8_t *data, int len) {
	int l = 0;
	for(int i = 0; i < len; i++) {
		l++;
		if(data[i] >= 0x80) {
			l++;
			i++;
		}
	}
	return l;
}

int sjis_to_utf8(uint8_t *sjis_data, int sjis_len, uint8_t *utf8_data, int utf8_len) {
	if(sjis_len < 1) return 0;
	if(utf8_len < 1) return -1;

	int j = 0, last_byte = 0;
	for(int i = 0; i < sjis_len; i++) {
		uint8_t b = sjis_data[i];
		if(last_byte == 0 && SJIS_FIRST_CHAR(b)) {
			last_byte = b;
		} else {
			char buf[4];
			int l = utf8_encode(sjis_char_to_unicode((last_byte << 8) | b), buf);
			if(j + l >= utf8_len - 1)
				return j;
			strcpy((char *)utf8_data + j, buf);
			j += l;
			last_byte = 0;
		}
	}
	return j;
}

int sjis_print_utf8(uint8_t *sjis_data, int sjis_len) {
	return sjis_write_utf8(stdout, sjis_data, sjis_len);
}

int sjis_write_utf8(FILE *f, uint8_t *sjis_data, int sjis_len) {
	if(sjis_len < 1) return 0;

	int j = 0, last_byte = 0;
	for(int i = 0; i < sjis_len; i++) {
		uint8_t b = sjis_data[i];
		if(last_byte == 0 && SJIS_FIRST_CHAR(b)) {
			last_byte = b;
		} else {
			char buf[4];
			int l = utf8_encode(sjis_char_to_unicode((last_byte << 8) | b), buf);
			if(l) fwrite(buf, l, 1, f);
			j += l;
			last_byte = 0;
		}
	}

	return j;
}

int sjis_fputc(int sjis, FILE *f) {
	uint8_t b = (sjis >> 8) & 0xff;
	if(SJIS_FIRST_CHAR(b))
		fputc(b, f); // TODO: error check
	return fputc(sjis & 0xff, f);
}

int sjis_fgetc(FILE *f) {
	int b = fgetc(f);
	if(b == EOF) return EOF;
	if(!SJIS_FIRST_CHAR(b))
		return b;

	int b2 = fgetc(f);
	if(b2 == EOF) return EOF;
	return b << 8 | b2;
}


uint16_t jis_to_sjis(uint16_t val) {
	uint8_t j2 = val & 0xff;
	uint8_t j1 = val >> 8;
	uint8_t s1, s2;
	if(33 <= j1 && j1 <= 94)
		s1 = (j1 + 1) / 2 + 112;
	else
		s1 = (j1 + 1) / 2 + 176;
	if(j1 & 1)
		s2 = j2 + 31 + (j2 / 96);
	else
		s2 = j2 + 126;

	return (s1 << 8) | s2;
}

uint16_t jis_from_sjis(uint16_t val) {
	int status = 0;
	uint8_t j1 = 0;
	uint8_t j2 = 0;
	uint8_t b1 = val >> 8;
	uint8_t b2 = val & 0xff;
	if((b1 >= 0x81 && b1 <= 0x84) || (b1 >= 0x87 && b1 <= 0x9f)) {
		j1 = 2 * (b1 - 0x70) - 1;
		if (b2 >= 0x40 && b2 <= 0x9e) {
			j2 = b2 - 31;
			if (j2 > 95)
				j2 -= 1;
			status = 1;
		} else if (b2 >= 0x9f && b2 <= 0xfc) {
			j2 = b2 - 126;
			j1 += 1;
			status = 1;
		}
	} else if (b1 >= 0xe0 && b1 <= 0xef) {
		j1 = 2 * (b1 - 0xb0) - 1;
		if (b2 >= 0x40 && b2 <= 0x9e) {
			j2 = b2 - 31;
			if (j2 > 95)
				j2 -= 1;
			status = 1;
		} else if (b2 >= 0x9f && b2 <= 0xfc) {
			j2 = b2 - 126;
			j1 += 1;
			status = 1;
		}
	}

	if(status == 0) return 0;
	return (j1 << 8) | j2;
}
