// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 管道操作
// Authors: jdh99 <jdh821@163.com>

#include "tziotpipe.h"
#include "tziot.h"
#include "tziotload.h"
#include "tziotconfig.h"
#include "tziotstandardlayer.h"

#include "tzlist.h"
#include "tzmalloc.h"
#include "lagan.h"
#include "tzbox.h"

#include <string.h>

#pragma pack(1)

typedef struct {
    uint64_t pipe;
    TZIotSendFunc send;
    TZIotIsAllowSendFunc isAllowSend;
} tItem;

#pragma pack()

static intptr_t list = 0;
// 申请和连接从机默认从第一管道走
static uint64_t firstPipe = 0;

static TZListNode* createNode(void);
static uint64_t getPipeNum(void);
static tItem* getItem(uint64_t pipe);

// TZIotPipeInit 初始化
void TZIotPipeInit(void) {
    list = TZListCreateList(TZIotMid);
    if (list == 0) {
        LE(TZIOT_TAG, "bind pipe failed!create list failed");
        return 0;
    }
}

// TZIotBindPipe 绑定管道.绑定成功后返回管道号,管道号如果是0表示绑定失败
// 注意:绑定的第一个网络管道是连接核心网的管道
uint64_t TZIotBindPipeNet(TZIotNetSendFunc send, TZIsAllowSendFunc isAllowSend) {

}

// TZIotBindPipe 绑定管道.绑定成功后返回管道号,管道号如果是0表示绑定失败
uint64_t TZIotBindPipe(TZDataFunc send, TZIsAllowSendFunc isAllowSend); {
    static bool first = true;

    if (first) {
        first = false;
        TZIotMid = mid;
        list = TZListCreateList(TZIotMid);
        if (list == 0) {
            LE(TZIOT_TAG, "bind pipe failed!create list failed");
            return 0;
        }
        TZIotInitSystem();
    }
    
    TZListNode* node = createNode();
    if (node == NULL) {
        LE(TZIOT_TAG, "bind pipe failed!create node failed");
        return 0;
    }

    tItem* item = (tItem*)node->Data;
    item->pipe = getPipeNum();
    item->send = send;
    item->isAllowSend = isAllowSend;
    TZListAppend(list, node);

    if (firstPipe == 0) {
        firstPipe = item->pipe;
    }
    return item->pipe;
}

static TZListNode* createNode(void) {
    TZListNode* node = TZListCreateNode(list);
    if (node == NULL) {
        return NULL;
    }
    node->Data = TZMalloc(TZIotMid, sizeof(tItem));
    if (node->Data == NULL) {
        TZFree(node);
        return NULL;
    }
    return node;
}

static uint64_t getPipeNum(void) {
    static uint64_t pipeNum = 0;
    pipeNum++;
    return pipeNum;
}

// TZIotPipeReceive 管道接收.pipe是接收管道号
// ip是4字节数组,port是端口号.如果是网络管道则需要使用这两个参数.不需要使用ip可设置为NULL,port设置为0
// 用户在管道中接收到数据时需回调本函数
void TZIotPipeReceive(uint64_t pipe, uint8_t* data, int size, uint8_t* ip, int port) {
	TZIotStandardLayerRx(pipe, data, size, ip, port);
}

// TZIotPipeIsAllowSend 管道是否允许发送
bool TZIotPipeIsAllowSend(uint64_t pipe) {
    tItem* item = getItem(pipe);
    if (item == NULL) {
        return false;
    }
    return item->isAllowSend();
}

static tItem* getItem(uint64_t pipe) {
    TZListNode* node = TZListGetHeader(list);
    tItem* item = NULL;
    for (;;) {
        if (node == NULL) {
            break;
        }
        
        item = (tItem*)node->Data;
        if (item->pipe == pipe) {
            return item;
        }
        
        node = node->Next;
    }
    return NULL;
}

// TZIotPipeSend 管道发送
void TZIotPipeSend(uint64_t pipe, uint8_t* data, int size, uint8_t* ip, int port) {
    tItem* item = getItem(srcPipe);
    if (item == NULL) {
        return;
    }
    item->send(dstPipe, data, size, ip, port);
}

// TZIotGetFirstPipe 读取第一管道
uint64_t TZIotGetFirstPipe(void) {
    return firstPipe;
}
