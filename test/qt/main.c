#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <windows.h>
#include <time.h>

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
static void pipeSend(uint64_t dstIA, uint8_t* bytes, int size);

DWORD WINAPI ProcessClientRequests(LPVOID lpParam)
{
    struct sockaddr_in remoteAddr;
    int ret = -1;
    int nAddrlen = sizeof(remoteAddr);
    char revdata[1024];
    char senddata[255];

    printf("thread!\n");

    //循环接收数据
    while(1)
    {
        ret = recvfrom(serSocket,revdata,1024,0,(SOCKADDR*)&remoteAddr,&nAddrlen);
        if(ret > 0)
        {
            revdata[ret] = 0x00;
            printf("<------------------------receive：%s %d\n",inet_ntoa(remoteAddr.sin_addr), ret);
//            printf("\n");
//            printf("客户：");
//            printf(revdata);
//            printf("\n");

            TZIotPipeReceive(gPipe, revdata, ret);
        }


//        if(strcmp(revdata,"bye") == 0)
//            break;

//        //发送数据
//        printf("我：");
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

    gMid = TZMallocRegister(RAM_INTERNAL, "tziot", 4096);
    AsyncLoad(gMid);
    gPipe = TZIotBindPipe(gMid, 0x2141000000010010, "123", pipeSend, isAllowSend);
    DComLogSetFilterLevel(LAGAN_LEVEL_DEBUG);

    //初始化wsa
    WORD sockVision = MAKEWORD(2,2);
    WSADATA wsadata;
    //其他变量
    struct sockaddr_in remoteAddr;
    struct sockaddr_in sin;

    if( WSAStartup(sockVision,&wsadata) != 0 )
    {
        printf("WSA初始化失败\n");
        return 0;
    }

    //创建套接字
    serSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(serSocket == INVALID_SOCKET)
    {
        printf("socket服务器创建失败\n");
        return 0;
    }

    //绑定IP和端口
    sin.sin_family = AF_INET;
    sin.sin_port = htons(14151);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if( bind(serSocket,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR )
    {
        printf("绑定IP和端口\n");
        return 0;
    }

    HANDLE threads[10];
    int existingClientCount=0;
    threads[existingClientCount++]=CreateThread(NULL, 0, ProcessClientRequests, NULL, 0, NULL);  //启动新线程，并且将socket传入

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

// DComCallbackFunc 注册DCOM服务回调函数
// 遵循谁调用谁释放原则,resp需要由回调函数使用TZMalloc开辟空间,DCom负责释放空间
// 返回值为0表示回调成功,否则是错误码
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

static void pipeSend(uint64_t dstIA, uint8_t* bytes, int size) {
    printf("------------------------->pipeSend:0x%x %p %d\n", (uint32_t)dstIA, (void*)bytes, size);
//    for (int i = 0; i < size; i++) {
//        printf("%02x ", bytes[i]);
//    }
//    printf("\n");

//    DComReceive(protocol, pipe, dstIA, bytes, size);

    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(14150);
    remoteAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.119");
    int nAddrlen = sizeof(remoteAddr);
    sendto(serSocket, bytes,size,0,(SOCKADDR*)&remoteAddr,nAddrlen);
}
