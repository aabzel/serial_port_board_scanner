#include "win_utils.h"

#include <Assert.h>
#include <Iphlpapi.h>
#include <stdio.h>
#include <windows.h>

#include "convert.h"
#include "tcp_client.h"
#ifdef DEPLOY_TCP_SERVER
#include "tcp_server.h"
#endif

void clear_tui (void) { system ("cmd /c cls"); }

bool get_adapter_info (void) {
    bool res = false;
    PIP_ADAPTER_INFO AdapterInfo;
    DWORD dwBufLen = sizeof (IP_ADAPTER_INFO);
    char mac_addr[18];

    // printf ("MAX_ADAPTER_ADDRESS_LENGTH: %u", (unsigned int) (MAX_ADAPTER_ADDRESS_LENGTH));
    printf ("sizeof IP_ADAPTER_INFO: %u", (unsigned int)sizeof (IP_ADAPTER_INFO));
    AdapterInfo = (IP_ADAPTER_INFO *)malloc (sizeof (IP_ADAPTER_INFO));
    if (AdapterInfo == NULL) {
        printf ("Error allocating memory needed to call GetAdaptersinfo\n");
        return NULL; // it is safe to call free(NULL)
    }

    // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
    if (GetAdaptersInfo (AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
        free (AdapterInfo);
        AdapterInfo = (IP_ADAPTER_INFO *)malloc (dwBufLen);
        if (AdapterInfo == NULL) {
            printf ("Error allocating memory needed to call GetAdaptersinfo\n");
            return NULL;
        }
    }
    uint16_t numOfAdapters = 0;
    printf ("\n");
    if (GetAdaptersInfo (AdapterInfo, &dwBufLen) == NO_ERROR) {
        // Contains pointer to current adapter info
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        do {
            // technically should look at pAdapterInfo->AddressLength
            //   and not assume it is 6.
            uint32_t ipVal;
            res = try_strl2ipv4 (pAdapterInfo->IpAddressList.IpAddress.String,
                                 strlen (pAdapterInfo->IpAddressList.IpAddress.String), &ipVal);
            if (true == res) {
                if (0 < ipVal) {
                    res = true;
                    workBenchParam.serverIP = ipVal;
#ifdef DEPLOY_TCP_SERVER
                    serverPC.serverIP = ipVal;
                    memcpy (serverPC.mac_addr, pAdapterInfo->Address, 6);
                    strncpy (serverPC.serverIPstr, pAdapterInfo->IpAddressList.IpAddress.String,
                             sizeof (serverPC.serverIPstr));
#endif
                    sprintf (mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", pAdapterInfo->Address[0],
                             pAdapterInfo->Address[1], pAdapterInfo->Address[2], pAdapterInfo->Address[3],
                             pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

                    printf ("Address %s, MAC %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
                    memcpy (workBenchParam.mac_addr, pAdapterInfo->Address, 6);
                    strncpy (workBenchParam.clientIPstr, pAdapterInfo->IpAddressList.IpAddress.String,
                             sizeof (workBenchParam.clientIPstr));
                }
            }

            pAdapterInfo = pAdapterInfo->Next;
            numOfAdapters++;
        } while (pAdapterInfo);
    }
    printf ("\n numOfAdapters %u", numOfAdapters);

    free (AdapterInfo);
    return res;
}
