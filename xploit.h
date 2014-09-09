#ifndef _LIBXPLOIT_H_
#define _LIBXPLOIT_H_

#include <stdlib.h>
#include "payloads.h"
#include "exploits.h"

#define XPLOIT_GLOBAL_DEBUG 0x00000001
#define XPLOIT_GLOBAL_ALL 0xffffffff

int xploitInitialize(unsigned int bitmask);

/*** information gathering ***/
enum LINUX_DISTRO {Unrecognized, Debian, Ubuntu, RedHat};

enum LINUX_DISTRO os_check_distro();
int os_check_distro_details(enum LINUX_DISTRO distro);
int os_check_compiler();

int kernel_config_check();

int create_x86_elf(char *filename, unsigned char *payload, const unsigned int payload_length);

#endif
