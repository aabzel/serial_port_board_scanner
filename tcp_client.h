#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
// https://www.binarytides.com/winsock-socket-programming-tutorial/
#ifdef __cplusplus
extern "C" {
#endif

#include "custom_type.h"

#define MAX_SIZE_OF_TCP_DATA_BYTE (400U)

typedef struct {
    char userName[100];
    uint8_t mac_addr[6];
    uint32_t clientIP;
    uint32_t serverIP;
    uint16_t serverPort;
    char clientIPstr[100];
    char serverIPstr[100];
} ComputerParams_t;

extern ComputerParams_t workBenchParam;

bool sent_to_tcp_server (char *txText, uint16_t txTextLen, uint16_t tcpServerPort, uint32_t tcp_server_ip);

#ifdef __cplusplus
}
#endif

#endif /* TCP_CLIENT_H */
