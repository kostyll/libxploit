#ifndef _PAYLOADS_H_
#define _PAYLOADS_H_

#include "module.h" 
#include "utils.h" 

#define PAYLOADS_NUMBER 7
#define PAYLOAD_NAME_LENGTH 128

struct Xploit;

struct payloadParam {
    char name[PAYLOAD_NAME_LENGTH];
    enum PARAM_REQUIREMENT required;
    enum PARAM_TYPE type;
    char *strValue;
    unsigned int strValueLen;
};

struct payload {
	unsigned int id;
	char name[PAYLOAD_NAME_LENGTH];
	unsigned char *data;
	unsigned int len;
    struct payloadParam *params;
	unsigned int paramsNum;
    void (*initOptionsfunc)(struct Xploit *xp);
	void (*help)(void);
};

// payloads
extern struct payload nullPayload;
extern struct payload x86LnxExec;
extern struct payload cmdBindAwk;
extern struct payload cmdGeneric;
extern struct payload cmdBindNc;
extern struct payload cmdBindPerl;
extern struct payload cmdReversePerl;

/*
// exec payload
extern unsigned char execShellcode[];
extern struct payloadParam execOptions[];
void execInit();
void execInit2(struct Xploit *xp);
void execShellcodeHelp();

// null payload
extern unsigned char nullPayload[];
void nullHelp();

// cmd-bind-awk payload
extern unsigned char cmdBindAwkPayload[];
extern struct payloadParam cmdBindAwkOpts[];
void cmdBindAwkHelp();

// cmd-generic payload
extern unsigned char cmdGenericPayload[];
extern struct payloadParam cmdGenericOpts[];
void cmdGenericHelp();

// cmd-bind-nc payload
extern unsigned char cmdBindNcPayload[];
extern struct payloadParam cmdBindNcOpts[];
void cmdBindNcHelp();

// cmd-bind-perl payload
extern unsigned char cmdBindPerlPayload[];
extern struct payloadParam cmdBindPerlOpts[];
void cmdBindPerlHelp();

// cmd-reverse-perl payload
extern unsigned char cmdReversePerlPayload[];
extern struct payloadParam cmdReversePerlOpts[];
void cmdReversePerlHelp();

// payloads colletion
//extern struct payload payloads[];
*/

int xploitGetPayloadID(char *name);
int xploitSetPayload(struct Xploit *xp, char *name);
int xploitSetPayloadOpt(struct Xploit *xp, char *paramName, void *newValue);
void xploitShowPayloads();
void xploitShowPayloadHelp(char *name);

int checkReqPayloadOpts(struct payload *p);
int payloadsInit();

#endif
