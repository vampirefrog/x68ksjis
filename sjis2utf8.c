#include <stdio.h>

#include "sjis.h"
#include "utf8.h"

int main(int argc, char **argv) {
	FILE *in = stdin, *out = stdout;
	if(argc > 1) in = fopen(argv[1], "rb");
	if(argc > 2) out = fopen(argv[2], "rb");

	int last_byte = 0;
	int b = 0;
	while((b = fgetc(in)) != EOF) {
		if(last_byte == 0 && SJIS_FIRST_CHAR(b)) {
			last_byte = b;
		} else {
			utf8_fputc(sjis_char_to_unicode((last_byte << 8) | b), out);
			last_byte = 0;
		}
	}

	return 0;
}
