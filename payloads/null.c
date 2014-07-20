#include "../payloads.h"
#include <stdlib.h>

unsigned char empty[] = "";

void nullHelp()
{
    printf("empty payload\n");
}

struct payload nullPayload = { 
    0, 
    "null", 
    empty, 
    0, 
    NULL, 
    0, 
    NULL, 
    nullHelp 
};
