#include <stdio.h>

#include "sjis.h"
#include "utf8.h"

int main(int argc, char **argv) {
	FILE *in = stdin, *out = stdout;
	if(argc > 1) in = fopen(argv[1], "rb");
	if(argc > 2) out = fopen(argv[2], "wb");

	int b;
	while((b = utf8_fgetc(in)) != EOF) {
		if(b == -2) fprintf(stderr, "Invalid char found at %d\n", ftell(in));
		sjis_fputc(unicode_char_to_sjis(b), out);
	}

	return 0;
}
