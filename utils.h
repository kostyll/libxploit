#ifndef _UTILS_H_
#define _UTILS_H_

// common headers needed by exploits and payloads
#include <stdio.h>
#include <stdlib.h>

// collection of common defines/utilities used here and there

#define XPLOIT_OK 0
#define XPLOIT_ERROR -1

#define TRUE 1
#define FALSE 0

#define MAX_NUM_OF_PARAMS 8
#define NOT_SET 0

extern int debugMode();

// bash colors
static const char *txtrst="\e[0m";
static const char *txtred="\e[0;31m";
static const char *txtgrn="\e[0;32m";
static const char *txtblu="\e[0;36m";

enum PLATFORM { LINUX, BSD, CMD };
enum ARCH { X86, X64, PHP };

enum PAYLOAD_TYPE { BIN, TXT };
enum EXPLOIT_TYPE { LOCAL, REMOTE };

enum PARAM_TYPE { INTEGER, STRING };
enum PARAM_REQUIREMENT { NOT_REQUIRED, REQUIRED, INTERNAL };

struct details {
    enum EXPLOIT_TYPE exploitType;
};

// output messages from modules 
enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
};

void printMsg(int level, const char * format, ...);
void debugMsg(const char * format, ...);

// asm opcodes library 

// function for random filenames

#endif
