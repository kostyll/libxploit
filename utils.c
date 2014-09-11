#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void printMsg(int level, const char * format, ...)
{
    va_list ap;

    switch (level) {
        case LOG_INFO:  fprintf(stderr, "[+] "); break;
        case LOG_WARN:  fprintf(stderr, "[-] "); break;
        case LOG_ERROR: fprintf(stderr, "[!] "); break;
    }

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fputc('\n', stderr);

    if (level == LOG_ERROR) {
        _exit(EXIT_FAILURE);
    }
}

void debugMsg(const char * format, ...)
{
    va_list ap;

    fprintf(stderr, "[DBG] ");
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fputc('\n', stderr);
}
