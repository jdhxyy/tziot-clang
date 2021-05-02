#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <windows.h>
#include <time.h>
#include <stdint.h>

#include "tziot.h"
#include "dcom.h"
#include "tztime.h"
#include "tzmalloc.h"
#include "pt.h"
#include "async.h"

#pragma comment(lib,"ws2_32.lib")

#define RAM_INTERNAL 0

SOCKET serSocket;

static int gMid = -1;
static uint64_t gPipe = 0;

static int service1(uint64_t pipe, uint64_t srcIA, uint8_t* req, int reqLen, uint8_t** resp, int* respLen);
static int case1(void);

static void print(uint8_t* bytes, int size);
static LaganTime getLaganTime(void);
static uint64_t getTime(void);

static uint64_t getTime(void);
static bool isAllowSend(void);
static void corePipeSend(uint8_t* data, int size, uint8_t* dstIP, uint16_t dstPort);

DWORD WINAPI ProcessClientRequests(LPVOID lpParam)
{
    struct sockaddr_in remoteAddr;
    int ret = -1;
    int nAddrlen = sizeof(remoteAddr);
    char revdata[1024];
    char senddata[255];

    printf("thread!\n");

    //ѭ����������
    while(1)
    {
        ret = recvfrom(serSocket,revdata,1024,0,(SOCKADDR*)&remoteAddr,&nAddrlen);
        if(ret > 0)
        {
            revdata[ret] = 0x00;
            printf("<------------------------receive��%s:%d %d\n",inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port), ret);
//            printf("\n");
//            printf("�ͻ���");
//            printf(revdata);
//            printf("\n");

            uint8_t ip[4] = {0};
            ip[0] = remoteAddr.sin_addr.S_un.S_un_b.s_b1;
            ip[1] = remoteAddr.sin_addr.S_un.S_un_b.s_b2;
            ip[2] = remoteAddr.sin_addr.S_un.S_un_b.s_b3;
            ip[3] = remoteAddr.sin_addr.S_un.S_un_b.s_b4;
            TZIotPipeCoreReceive(revdata, ret, ip, ntohs(remoteAddr.sin_port));
        }


//        if(strcmp(revdata,"bye") == 0)
//            break;

//        //��������
//        printf("�ң�");
//        scanf("%s",senddata);
//        sendto(serSocket,senddata,strlen(senddata),0,(SOCKADDR*)&remoteAddr,nAddrlen);
    }


    return 0;
}

int main(void)
{
    LaganLoad(print, getLaganTime);

    TZTimeLoad(getTime);
    TZMallocLoad(RAM_INTERNAL, 20, 100 * 1024, malloc(100 * 1024));

    gMid = TZMallocRegister(RAM_INTERNAL, "test", 4096);
    TZIotLoad(0x2141000000010010, "123");
    gPipe = TZIotBindPipeCore(corePipeSend, isAllowSend);
    DComLogSetFilterLevel(LAGAN_LEVEL_DEBUG);

    //��ʼ��wsa
    WORD sockVision = MAKEWORD(2,2);
    WSADATA wsadata;
    //��������
    struct sockaddr_in remoteAddr;
    struct sockaddr_in sin;

    if( WSAStartup(sockVision,&wsadata) != 0 )
    {
        printf("WSA��ʼ��ʧ��\n");
        return 0;
    }

    //�����׽���
    serSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(serSocket == INVALID_SOCKET)
    {
        printf("socket����������ʧ��\n");
        return 0;
    }

    //��IP�Ͷ˿�
    sin.sin_family = AF_INET;
    sin.sin_port = htons(14151);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if( bind(serSocket,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR )
    {
        printf("��IP�Ͷ˿�\n");
        return 0;
    }

    HANDLE threads[10];
    int existingClientCount=0;
    threads[existingClientCount++]=CreateThread(NULL, 0, ProcessClientRequests, NULL, 0, NULL);  //�������̣߳����ҽ�socket����

    TZIotRegister(1, service1);
    AsyncStart(case1, ASYNC_ONLY_ONE_TIME);

    while (1) {
        //case1();
        AsyncRun();
    }

    closesocket(serSocket);
    WSACleanup();
    return 0;
}

// DComCallbackFunc ע��DCOM����ص�����
// ��ѭ˭����˭�ͷ�ԭ��,resp��Ҫ�ɻص�����ʹ��TZMalloc���ٿռ�,DCom�����ͷſռ�
// ����ֵΪ0��ʾ�ص��ɹ�,�����Ǵ�����
static int service1(uint64_t pipe, uint64_t srcIA, uint8_t* req, int reqLen, uint8_t** resp, int* respLen) {
    printf("service1:pipe:%d srcIA:0x%x reqLen:%d\n", pipe, srcIA, reqLen);

    for (int i = 0; i < reqLen; i++) {
        printf("0x%02x ", req[i]);
    }
    printf("\n");
    uint8_t* arr = TZMalloc(gMid, 260);
    if (arr == NULL) {
        return 0;
    }
    for (int i = 0; i < 260; i++) {
        arr[i] = i;
    }
    *resp = arr;
    *respLen = 260;
    return 0;
}

static int case1(void) {
    static struct pt pt;
    static uint8_t arr[3] = {1,2,3};
    static int result = 0;
    static uint8_t* resp = NULL;
    static int respLen = 0;
    static intptr_t handle;

    PT_BEGIN(&pt);

    PT_WAIT_UNTIL(&pt, TZIotIsConn());

    handle = TZIotCallCreateHandle(gPipe, 0x2141000000000004, 1, 5000, NULL, 0, &resp, &respLen, &result);
    PT_WAIT_THREAD(&pt, TZIotCall(handle));

    if (result == 0 && resp != NULL) {
        for (int i = 0; i < respLen; i++) {
            printf("0x%02x\t", resp[i]);
        }
        printf("\n");
    } else {
        printf("call failed:0x%x\n", result);
    }

    PT_END(&pt);
}

static void print(uint8_t* bytes, int size) {
    printf("%s", bytes);
}

static LaganTime getLaganTime(void) {
    SYSTEMTIME t1;
    GetSystemTime(&t1);

    LaganTime time;
    time.Year = t1.wYear;
    time.Month = t1.wMonth;
    time.Day = t1.wDay;
    time.Hour = t1.wHour;
    time.Minute = t1.wMinute;
    time.Second = t1.wSecond;
    time.Us = t1.wMilliseconds * 1000;
    return time;
}

static uint64_t getTime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000000 + t.tv_usec;
}

static bool isAllowSend(void) {
    return true;
}

static void corePipeSend(uint8_t* data, int size, uint8_t* dstIP, uint16_t dstPort) {
    //printf("------------------------->pipeSend:0x%x %p %d\n", (uint32_t)dstIA, (void*)bytes, size);
//    for (int i = 0; i < size; i++) {
//        printf("%02x ", bytes[i]);
//    }
//    printf("\n");

//    DComReceive(protocol, pipe, dstIA, bytes, size);

    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(dstPort);
    //remoteAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.119");
    remoteAddr.sin_addr.S_un.S_un_b.s_b1 = dstIP[0];
    remoteAddr.sin_addr.S_un.S_un_b.s_b2 = dstIP[1];
    remoteAddr.sin_addr.S_un.S_un_b.s_b3 = dstIP[2];
    remoteAddr.sin_addr.S_un.S_un_b.s_b4 = dstIP[3];

    int nAddrlen = sizeof(remoteAddr);
    sendto(serSocket, data, size, 0, (SOCKADDR*)&remoteAddr, nAddrlen);
}
