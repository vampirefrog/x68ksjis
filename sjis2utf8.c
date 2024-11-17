#include <stdio.h>

#include "sjis.h"
#include "utf8.h"

int main(int argc, char **argv) {
	FILE *in = stdin, *out = stdout;
	if(argc > 1) in = fopen(argv[1], "rb");
	if(argc > 2) out = fopen(argv[2], "rb");

	int last_byte = 0;
	int b = 0;
	while((b = sjis_fgetc(in)) != EOF) {
		if(b == -2) fprintf(stderr, "Invalid char found at %d\n", ftell(in));
		utf8_fputc(sjis_char_to_unicode(b), out);
	}

	return 0;
}
