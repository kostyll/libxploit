#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>

#include "xploit.h"

static unsigned int globalBitmask;

int xploitInitialize(unsigned int bitmask)
{
    payloadsInit();
    exploitsInit();

    globalBitmask = bitmask;

    return 1;
}

int debugMode()
{
    if(globalBitmask & XPLOIT_GLOBAL_DEBUG) {
        return 1;
    }

    return 0;
}

// TODO: move all below to utils.c

/* simplest header for x86 elf */
unsigned char tinyElf[] = {
  0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x54, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x04, 0x08, 0x00, 0x80, 0x04, 0x08, 0xef, 0xbe, 0xad, 0xde,
  0xef, 0xbe, 0xad, 0xde, 0x07, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
};

static unsigned long getesp()
{
	__asm__("mov %esp,%eax"); /* equiv. of 'return esp;' in C */
}

int create_x86_elf(char *filename, unsigned char *payload, const unsigned int payload_length)
{
    int fd;
	unsigned int len;
	const unsigned int position = 68;
	unsigned char elf_binary[sizeof(tinyElf) + payload_length];
	unsigned int tinyElf_length = sizeof(tinyElf);

    printf("create_x86_elf()\n");

	/* calculate overall length */
	len = tinyElf_length + payload_length;
    printf("size: %d\n", len);

	/* set elf size */
	memcpy(elf_binary, tinyElf, tinyElf_length);
	*((unsigned int *)&elf_binary[position]) = len;
	*((unsigned int *)&elf_binary[position + 4]) = len;

	/* add payload */
	memcpy(&elf_binary[tinyElf_length], payload, payload_length);

	/* create file */
    fd = open(filename, O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    write(fd, elf_binary, len); 
	close(fd);

	return 0;
}

/*
   Check kernel options that harden the kernel against attacks.
   ( based on checksec.sh function kernelcheck() )
   Following options are being checked:
   CONFIG_CC_STACKPROTECTOR
   CONFIG_DEBUG_STRICT_USER_COPY_CHECKS
   CONFIG_DEBUG_RODATA
   CONFIG_STRICT_DEVMEM
   CONFIG_DEVKMEM

   Kernel heap hardening options:
   CONFIG_KERNHEAP
   CONFIG_KERNHEAP_FULLPOISON
*/
int kernel_config_check()
{
	const char *prefix = "/boot/config-";
	system("uname -r");
	return 0;
}

enum LINUX_DISTRO os_check_distro()
{
    int result;

	if(system("test -f /etc/lsb-release") == 0)
        return Ubuntu;

	if(system("test -f /etc/debian_version") == 0)
        return Debian;

	if(system("test -f /etc/redhat-release") == 0)
        return RedHat;

	return Unrecognized;
}

int os_check_distro_details(enum LINUX_DISTRO distro)
{
    // TODO: base on toolbox/operational/osDetails.sh
    return 0;
}

int os_check_compiler()
{
	if(system("test -f /usr/bin/gcc") == 0)
        return 1;

    return 0;
}
