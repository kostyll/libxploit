#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#define MAXLINE 4096

typedef enum {
    ERROR = -1,
    HTTP200 = 200,
    HTTP400 = 400,
    HTTP401 = 401,
} HTTP_RESPONSE_CODE;

HTTP_RESPONSE_CODE httpHEADRequest();
HTTP_RESPONSE_CODE httpGETRequest(char *ip, int port, char *uri, char **responseBody, char *vhost);
HTTP_RESPONSE_CODE httpPOSTRequest(char *ip, int port, char *uri, char **responseBody, char *vhost);
int httpUriEncode(char *uri, char **uriEncoded);

#endif
