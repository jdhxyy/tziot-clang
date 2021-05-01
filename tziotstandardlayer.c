// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 标准层处理模块
// Authors: jdh99 <jdh821@163.com>

#include "tziotstandardlayer.h"
#include "tziotconfig.h"
#include "tziotpipe.h"

#include "lagan.h"
#include "tzlist.h"
#include "tzmalloc.h"

#include <string.h>

#pragma pack(1)

typedef struct {
    TZIotStandardLayerRxCallback callback;
} tItem;

#pragma pack()

// 观察者列表
static intptr_t list = 0;

static int getStandardHeader(uint8_t* data, int size, UtzStandardHeader* header);
static void notifyObserver(uint8_t* data, int size, UtzStandardHeader* header, uint64_t pipe);
static TZListNode* createNode(void);

// TZIotStandardLayerRx 标准层接收
void TZIotStandardLayerRx(uint64_t pipe, uint8_t* data, int size, uint8_t* ip, int port) {
    UtzStandardHeader header;
    int offset = getStandardHeader(data, size, &header);
    if (offset == 0) {
        return;
    }
    notifyObserver(data + offset, size - offset, &header, pipe);
}

static int getStandardHeader(uint8_t* data, int size, UtzStandardHeader* header) {
    int offset = UtzBytesToStandardHeader(data, size, header);
    if (offset == 0) {
        LD(TZIOT_TAG, "get standard header failed:bytes to standard header failed");
        return 0;
    }
    if (header->Version != UTZ_NLV1_PROTOCOL_VERSION) {
        LD(TZIOT_TAG, "get standard header failed:protocol version is not match:%d", header->Version);
        return 0;
    }
    if (header->PayloadLen + offset != size) {
        LD(TZIOT_TAG, "get standard header failed:payload len is not match:%d", header->PayloadLen);
        return 0;
    }
    return offset;
}

static void notifyObserver(uint8_t* data, int size, UtzStandardHeader* header, uint64_t pipe) {
    TZListNode* node = TZListGetHeader(list);
    tItem* item = NULL;
    for (;;) {
        if (node == NULL) {
            break;
        }

        item = (tItem*)node->Data;
        if (item->callback) {
            item->callback(data, size, header, pipe);
        }

        node = node->Next;
    }
}

// TZIotStandardLayerRegisterRxObserver 注册观察者
void TZIotStandardLayerRegisterRxObserver(TZIotStandardLayerRxCallback callback) {
    if (list == 0) {
        list = TZListCreateList(TZIotMid);
    }

    TZListNode* node = createNode();
    if (node == NULL) {
        return;
    }
    tItem* item = (tItem*)node->Data;
    item->callback = callback;
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

// TZIotStandardLayerSend 基于标准头部发送
void TZIotStandardLayerSend(uint8_t* data, int dataLen, UtzStandardHeader* standardHeader, uint64_t pipe, 
    uint8_t* ip, int port) {
    if (dataLen > TZIOT_FRAME_MAX_LEN) {
        LE(TZIOT_TAG, "standard layer send failed!data len is too long:%d src ia:0x%llx dst ia:0x%llx", dataLen, 
            standardHeader->SrcIA, standardHeader->DstIA);
        return;
    }
    if (standardHeader->PayloadLen != (uint16_t)dataLen) {
        standardHeader->PayloadLen = (uint16_t)dataLen;
    }

    int frameSize = UTZ_NLV1_HEAD_LEN + dataLen;
    uint8_t* frame = TZMalloc(TZIotMid, frameSize);
    if (frame == NULL) {
        LE(TZIOT_TAG, "standard layer send failed!malloc failed!");
        return;
    }
    (void)UtzStandardHeaderToBytes(standardHeader, frame, UTZ_NLV1_HEAD_LEN);
    memcpy(frame + UTZ_NLV1_HEAD_LEN, data, (uint64_t)dataLen);
    TZIotPipeSend(pipe, frame, frameSize, ip, port);
    TZFree(frame);
}
