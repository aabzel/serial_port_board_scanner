#include "tcp_client.h"

#if !defined(_WIN32) && !defined(__CYGWIN__)
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <Assert.h>
#include <Iphlpapi.h>
#include <stdio.h>

#include "convert.h"
#include "str_ops.h"
#include "tcp_server.h"

ComputerParams_t workBenchParam;

bool sent_to_tcp_server (char *txText, uint16_t txTextLen, uint16_t tcpServerPort, uint32_t tcp_server_ip) {
    char *ipAddrStr;
    ipAddrStr = ip_to_str ((uint8_t *)&tcp_server_ip);
    // printf ("\n server IP %s port %u",ipAddrStr,tcpServerPort);
    bool res = false;
    WSADATA wsa;
    SOCKET socDescriptor;
    // printf ("\nInitialising Winsock...");
    if (0 == WSAStartup (MAKEWORD (2, 2), &wsa)) {
#if DEBUG_TCP_CLIENT
        printf ("\nInitialised");
#endif
        res = true;
        socDescriptor = socket (AF_INET, SOCK_STREAM, 0);
        if (socDescriptor == INVALID_SOCKET) {
            printf ("Could not create socket : %d", WSAGetLastError ());
        } else {
#if DEBUG_TCP_CLIENT
            printf ("Socket created.\n");
#endif
            struct sockaddr_in server;
            // server.sin_addr.s_addr = inet_addr ("192.168.0.11");
            server.sin_addr.s_addr = inet_addr (ipAddrStr);
            // printf ("\nip paced %08x des %08x",server.sin_addr.s_addr, tcp_server_ip);
            server.sin_family = AF_INET;
            server.sin_port = htons (tcpServerPort);
            // Connect to remote server
            int con_stat = connect (socDescriptor, (struct sockaddr *)&server, sizeof (server));
            if (con_stat < 0) {
                printf ("\nTCP server connect error %u", con_stat);
                res = false;
            } else {
#if DEBUG_TCP_CLIENT
                puts ("Connected");
#endif
                int stat = send (socDescriptor, txText, txTextLen, 0);
                if (stat < 0) {
                    printf ("\nSend failed stat %d", stat);
                    res = false;
                } else {
#if DEBUG_TCP_CLIENT
                    printf ("\nData Send");
#endif
                    res = true;
                }
                closesocket (socDescriptor);
            }
        }

    } else {
        printf ("Failed. Error Code : %d", WSAGetLastError ());
        res = false;
    }

    return res;
}
