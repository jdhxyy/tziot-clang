// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// �ܵ�����
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
// ��������Ӵӻ�Ĭ�ϴӵ�һ�ܵ���
static uint64_t firstPipe = 0;

static TZListNode* createNode(void);
static uint64_t getPipeNum(void);
static tItem* getItem(uint64_t pipe);

// TZIotPipeInit ��ʼ��
void TZIotPipeInit(void) {
    list = TZListCreateList(TZIotMid);
    if (list == 0) {
        LE(TZIOT_TAG, "bind pipe failed!create list failed");
        return 0;
    }
}

// TZIotBindPipe �󶨹ܵ�.�󶨳ɹ��󷵻عܵ���,�ܵ��������0��ʾ��ʧ��
// ע��:�󶨵ĵ�һ������ܵ������Ӻ������Ĺܵ�
uint64_t TZIotBindPipeNet(TZIotNetSendFunc send, TZIsAllowSendFunc isAllowSend) {

}

// TZIotBindPipe �󶨹ܵ�.�󶨳ɹ��󷵻عܵ���,�ܵ��������0��ʾ��ʧ��
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

// TZIotPipeReceive �ܵ�����.pipe�ǽ��չܵ���
// ip��4�ֽ�����,port�Ƕ˿ں�.���������ܵ�����Ҫʹ������������.����Ҫʹ��ip������ΪNULL,port����Ϊ0
// �û��ڹܵ��н��յ�����ʱ��ص�������
void TZIotPipeReceive(uint64_t pipe, uint8_t* data, int size, uint8_t* ip, int port) {
	TZIotStandardLayerRx(pipe, data, size, ip, port);
}

// TZIotPipeIsAllowSend �ܵ��Ƿ�������
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

// TZIotPipeSend �ܵ�����
void TZIotPipeSend(uint64_t pipe, uint8_t* data, int size, uint8_t* ip, int port) {
    tItem* item = getItem(srcPipe);
    if (item == NULL) {
        return;
    }
    item->send(dstPipe, data, size, ip, port);
}

// TZIotGetFirstPipe ��ȡ��һ�ܵ�
uint64_t TZIotGetFirstPipe(void) {
    return firstPipe;
}
