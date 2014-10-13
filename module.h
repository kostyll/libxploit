#ifndef _MODULE_H_
#define _MODULE_H_

// common headers needed by exploits and payloads
#include <stdio.h>
#include <stdlib.h>

#define XPLOIT_NO_ERROR 0x0
#define XPLOIT_UNRECOGNIZED_EXPLOIT_ERROR 0x1
#define XPLOIT_UNRECOGNIZED_PAYLOAD_ERROR 0x2
#define XPLOIT_INIT_ERROR 0x3

extern int exploitErrCode;

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
