// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// �ܵ�����
// Authors: jdh99 <jdh821@163.com>

#include "tziotpipe.h"
#include "tziot.h"
#include "tziotconfig.h"
#include "tziotstandardlayer.h"

#include "tzlist.h"
#include "tzmalloc.h"
#include "lagan.h"
#include "tzbox.h"
#include "tzaccess.h"
#include "tztype.h"

#include <string.h>

#define PIPE_CORE 1

#pragma pack(1)

typedef struct {
    uint64_t pipe;
    TZDataFunc send;
    TZIsAllowSendFunc isAllowSend;
} tItem;

#pragma pack()

static intptr_t list = 0;

static TZAccessSendFunc corePipeSend = NULL;
static TZIsAllowSendFunc corePipeIsAllowSend = NULL;

static TZListNode* createNode(void);
static uint64_t getPipeNum(void);
static tItem* getItem(uint64_t pipe);

// TZIotPipeInit ��ʼ��
void TZIotPipeInit(void) {
    list = TZListCreateList(TZIotMid);
    if (list == 0) {
        LE(TZIOT_TAG, "bind pipe failed!create list failed");
    }
}

// TZIotBindPipeCore �󶨺������ܵ�.�󶨳ɹ��󷵻عܵ���,�ܵ��������0��ʾ��ʧ��
// ע��:�������ܵ�ֻ�ܰ�һ��
uint64_t TZIotBindPipeCore(TZAccessSendFunc send, TZIsAllowSendFunc isAllowSend) {
    corePipeSend = send;
    corePipeIsAllowSend = isAllowSend;
    TZAccessLoad(TZIotGetLocalIA(), TZIotGetLocalPwd(), send, isAllowSend);
    return PIPE_CORE;
}

// TZIotPipeCoreReceive �������ܵ�����.pipe�ǽ��չܵ���
// srcIP��ԴIP��ַ,4�ֽ�����.srcPort��Դ�˿ں�
// �û��ڹܵ��н��յ�����ʱ��ص�������
void TZIotPipeCoreReceive(uint8_t* data, int size, uint8_t* srcIP, uint16_t srcPort) {
    TZAccessReceive(data, size, srcIP, srcPort);
    if (TZAccessIsConn()) {
        TZIotStandardLayerRx(PIPE_CORE, data, size);
    }
}

// TZIotBindPipe �󶨹ܵ�.�󶨳ɹ��󷵻عܵ���,�ܵ��������0��ʾ��ʧ��
uint64_t TZIotBindPipe(TZDataFunc send, TZIsAllowSendFunc isAllowSend) {
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
    static uint64_t pipeNum = PIPE_CORE;
    pipeNum++;
    return pipeNum;
}

// TZIotPipeReceive �ܵ�����.pipe�ǽ��չܵ���
// �û��ڹܵ��н��յ�����ʱ��ص�������
void TZIotPipeReceive(uint64_t pipe, uint8_t* data, int size) {
	TZIotStandardLayerRx(pipe, data, size);
}

// TZIotPipeIsAllowSend �ܵ��Ƿ�������
bool TZIotPipeIsAllowSend(uint64_t pipe) {
    if (pipe == PIPE_CORE) {
        return corePipeIsAllowSend() && TZAccessIsConn();
    }

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
void TZIotPipeSend(uint64_t pipe, uint8_t* data, int size) {
    if (pipe == PIPE_CORE) {
        uint8_t ip[4] = {0};
        uint16_t port = 0;
        TZAccessGetParentAddr(ip, &port);
        if (port == 0) {
            return;
        }
        corePipeSend(data, size, ip, port);
        return;
    }

    tItem* item = getItem(pipe);
    if (item == NULL) {
        return;
    }
    item->send(data, size);
}
