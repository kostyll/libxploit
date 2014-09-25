#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/tcp.h>

#include "http-client.h"

#define SIZE 512

static int makeConnection(char *ip, int port)
{
    int s;
    struct hostent *hp;
    struct sockaddr_in server;
    struct timeval timeout;

    if (hp = gethostbyname(ip)) {
        memset((char *) &server,0, sizeof(server));
        memmove((char *) &server.sin_addr, hp->h_addr, hp->h_length);
        server.sin_family = hp->h_addrtype;
        server.sin_port = (unsigned short) htons(port);
    } 
    else
        return -1;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    //setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, 0, 0);
    timeout.tv_sec = 0;
    timeout.tv_usec = 900000;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&timeout, sizeof(struct timeval));

    if (connect(s, &server, sizeof(server)) < 0)
        return -1;

    return s;
}

// FIXME: come up with more efficient way
static int readLine(int fd, char *buf, int max) 
{
    int n = 0;
    while (n < max) {
        if (read(fd, buf, 1) != 1) {
            n = -n;
            break;
        }
        n++;
        if (*buf == '\015') continue; /* ignore CR */
        if (*buf == '\012') break;    /* LF is the separator */
        buf++;
    }

    *buf = 0;
    return n;
}

static int readBuffer(int fd, void *buffer, size_t len)
{
    size_t nleft;
    int nread;
    char *ptr;

    ptr = buffer;
    nleft = len;
    while(nleft > 0) {
        if((nread = read(fd, ptr, nleft)) <= 0) {
            if(nread < 0 && errno == EINTR)
                nread = 0;
            else
                return -1;
        }

        nleft -= nread;
        ptr += nread;
    }

    return len;
}

static int readCrlf(int fd)
{
    char buf[2];
    int n;
    n = readBuffer(fd, buf, 2);

    return n;
}

int writeBuffer(int fd, void *buffer, size_t len)
{
    size_t nleft;
    int nwritten;
    char *ptr;

    ptr = buffer;
    nleft = len;
    while(nleft > 0) {
        if((nwritten = write(fd, ptr, nleft)) <= 0) {
            if(nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return len;
}

char *readChunks(int fd, size_t *len)
{
    size_t total = 0;
    char *data = NULL;

    *len = 0;

    int i;
    for(;;) {
        char chunkline[MAXLINE];
        int n;
        size_t chunksize;

        n = readLine(fd, chunkline, sizeof(chunkline) - 1);

        // error while reading data
        if(n < 0) {
            if(data)
                free(data);
            return NULL;
        }

        // no more data on socket
        if(n == 0)
            break;

        chunkline[n] = '\0';

        sscanf(chunkline, "%x", &chunksize); 

        // last chunk
        if(chunksize == 0) 
            break;

        if(data == NULL) {
            data = (char *)malloc(chunksize);
        }
        else {
            // TODO: precheck chunksize + total against overflow
            data = (char *)realloc(data, chunksize + total);
        }
        if(data == NULL)
            return NULL;

        readBuffer(fd, data + total, chunksize);
        total += chunksize;

        readCrlf(fd);
    }

    *len = total;
    return data;
}

static int getResponse(int fd, char **response)
{
    char respHeader[SIZE];
    int contentLen = 0;
    int retCode;
    char contentType[32] = {0};
    char transferEnc[32] = {0};
    int n;
    char *ptr;

    // read response header
    while (1) {
        n = readLine(fd, respHeader, SIZE - 1);
        if (n <= 0) {
            return -1;
        }

        // end of header?
        if ( n > 0 && (*respHeader)=='\0') 
            break;

        for (ptr = respHeader; (*ptr != ':' && *ptr) ; ptr++) 
            *ptr = tolower(*ptr);
        sscanf(respHeader,"http/1.%*d %03d",(int*)&retCode);
        sscanf(respHeader,"content-type: %s", contentType);
        sscanf(respHeader,"content-length: %d", &contentLen);
        sscanf(respHeader,"transfer-encoding: %s", transferEnc);

        printf("header: %s\n", respHeader);
    }

    //printf("content: code: %d len: %d type: %s\n", retCode, contentLen, contentType);

    // read response body:

    // normal trnasfer
    if(contentLen != 0) {
        *response = (char *)malloc(contentLen);
        if(*response == NULL)
            return -1;

        if(readBuffer(fd, *response, contentLen) <= 0) {
            return -1;
        }
    }
    // chunked transfer
    else if(strcmp(transferEnc, "chunked") == 0) {
        printf("chunked transfer\n");
        size_t len;
        *response = readChunks(fd, &len);
        if(*response == NULL) {
            return -1;
        }
        printf("content len: %d\n", len);
    }

    return retCode;
}

int httpUriEncode(char *uri, char **uriEncoded)
{
    size_t len;
    char *ptr;
    char c;
    int isReserved(char c) {
        char reservedChars[] = { '/', ' ', '{', '}', '<', '>', '%', '!', '*', '\'','(', ')', ';', ':', '@', '&', '=', '+', '$', ',', '?', '#', '[', ']', '\0' };
        char *p = reservedChars;
        while(*p) {
            if(c == *p)
                return 1;
            ++p;
        }
        return 0;
    };

    len = strlen(uri);
    
    // at most len*3 characters will be needed
    *uriEncoded = (char *)malloc(len*3);
    if(*uriEncoded == NULL)
        return -1;

    ptr = *uriEncoded;
    while((c = *uri++)) {
        if(isReserved(c)) { 
            char a[4];
            *ptr++ = '%';
            snprintf(a, 4, "%x", c);
            *ptr++ = a[0];
            *ptr++ = a[1];
        }
        else
            *ptr++ = c;
    }
    *ptr = '\0';

    return 0;
}

HTTP_RESPONSE_CODE httpGETRequest(char *ip, int port, char *uri, char **responseBody, char *vhost)
{
    HTTP_RESPONSE_CODE ret;
    int s;
    char *header;
    char *host;
    unsigned int len;

    len = strlen(uri) + SIZE;
    header = (char *)malloc(len);
    if(!header)
        return -1;

    if((s = makeConnection(ip, port)) == -1) {
        // TODO: http lib error: can't connect
        fprintf(stderr, "http lib error: can't connect\n");
        free(header);
        return -1;
    }

    // BUILD request header:
    // 1) prepare content for 'Host' field
    if(!vhost) {
        host = (char *)malloc(strlen(ip) + 16);
        if(!host)
            return -1;
        memset(host, '\0', strlen(ip) + 16);
        snprintf(host, strlen(ip) + 16, "%s:%d", ip, port);  
    }
    else
        host = vhost;


    // 2) prepare rest of fields
    memset(header, '\0', len);
    snprintf(header, len, "GET %s HTTP/1.1\015\012"
                          "Host: %s\015\012"
                          "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\015\012"
                          "Connection: Close\015\012"
                          "Content-Length: 0\015\012"
                          "\015\012",
                           uri, host);
    if(!vhost)
        free(host);

    // calculate real length
    len = strlen(header);

    if(writeBuffer(s, header, len) != len) {
        // TODO: http lib error: failed to write header
        fprintf(stderr, "http lib error: failed to write header\n");
        free(header);
        close(s);
        return -1;
    }

    if((ret = getResponse(s, responseBody)) == -1) {
        // TODO: http lib error: failed to read response
        fprintf(stderr, "http lib error: failed to read response\n");
        free(header);
        close(s);
        return -1;
    }

    close(s);
    free(header);
    return ret;
}

HTTP_RESPONSE_CODE httpPOSTRequest(char *ip, int port, char *uri, char **responseBody, char *vhost)
{
    HTTP_RESPONSE_CODE ret;

    return ret;
}
