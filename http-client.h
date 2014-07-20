#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

//TODO: tiny synchronous http library

typedef enum {
    ERROR = -1,
    ERROR400 = 400
} HTTP_RESPONSE_CODE;

HTTP_RESPONSE_CODE httpHEADRequest();
HTTP_RESPONSE_CODE httpGETRequest(char *ip, int port, char *uri, char **responseBody, char *vhost);
HTTP_RESPONSE_CODE httpPOSTRequest(char *ip, int port, char *uri, char **responseBody, char *vhost);
int httpUriEncode(char *uri, char **uriEncoded);

#endif
