#ifndef _MODULE_H_
#define _MODULE_H_

// common headers needed by exploits and payloads
#include <stdio.h>
#include <stdlib.h>

enum PLATFORM { LINUX, BSD, CMD };
enum ARCH { X86, X64, PHP };

enum PAYLOAD_TYPE { BIN, TXT };
enum EXPLOIT_TYPE { LOCAL, REMOTE };

enum PARAM_TYPE { INTEGER, STRING };
enum PARAM_REQUIREMENT { NOT_REQUIRED, REQUIRED, INTERNAL };

struct details {
    enum EXPLOIT_TYPE exploitType;
};

#endif
