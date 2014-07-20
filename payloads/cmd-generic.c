
#include "../payloads.h"
#include "../exploits.h"
#include <stdlib.h>

// payload's content
unsigned char cmdGenericPayload[] = 
"%s";

void cmdGenericHelp()
{
    printf("executes supplied command line\n");
}

// payload's options
struct payloadParam cmdGenericOpts[MAX_NUM_OF_PARAMS] = {
    { "CMD", REQUIRED, STRING, NOT_SET, NULL, NOT_SET }
};

struct payload cmdGeneric = {
    3,
    "cmd-generic",
    cmdGenericPayload,
    2,
    cmdGenericOpts,
    1,
    NULL,
    cmdGenericHelp
};
