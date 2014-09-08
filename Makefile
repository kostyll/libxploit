
PROGNAME = pick
# CFLAGS flags:
# '-z execstack' - turns off NX protection making stack executable
CFLAGS   =
LDFLAGS  =

$(PROGNAME): payloads/x86-exec.c payloads/null.c payloads/cmd-bind-awk.c payloads/cmd-generic.c payloads/cmd-bind-nc.c payloads/cmd-bind-perl.c payloads/cmd-reverse-perl.c libxploit.c xploit.h exploits.c exploits.h payloads.c payloads.h exploits/dummy-mod.c exploits/vmware-mount.c exploits/setuid-nmap.c exploits/fdisc-webmin.c exploits/exec-awstats.c http-client.c utils.c pick.c
	        $(CC) -static $(CFLAGS) $(LDFLAGS) pick.c libxploit.c utils.c exploits.c payloads.c http-client.c payloads/x86-exec.c payloads/null.c payloads/cmd-bind-awk.c payloads/cmd-generic.c payloads/cmd-bind-nc.c payloads/cmd-bind-perl.c payloads/cmd-reverse-perl.c exploits/dummy-mod.c exploits/vmware-mount.c exploits/setuid-nmap.c exploits/fdisc-webmin.c exploits/exec-awstats.c -o $(PROGNAME)

clean:
	        rm -f $(PROGNAME) *.o *.so *.a
