#pragma once

#include <stdio.h>

int utf8_encode(int c, char *buf);
int utf8_strlen(char *buf);
int utf8_strnlen(char *buf, int n);
int utf8_fputc(int c, FILE *f);
