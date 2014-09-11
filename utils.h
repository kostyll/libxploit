#ifndef _UTILS_H_
#define _UTILS_H_

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
