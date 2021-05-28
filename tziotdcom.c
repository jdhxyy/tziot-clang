// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// dcom操作
// Authors: jdh99 <jdh821@163.com>

#include "tziotdcom.h"
#include "tziotconfig.h"
#include "tziotpipe.h"
#include "tziotstandardlayer.h"

#include "dcom.h"
#include "utz.h"
#include "lagan.h"
#include "tzmalloc.h"
#include "async.h"
#include "pt.h"

#include <string.h>

static int dcomRun(void);
static void dcomSend(int protocol, uint64_t pipe, uint64_t dstIA, uint8_t* bytes, int size);

// dcomDealStandardLayerRx 处理标准层回调函数
static void dcomDealStandardLayerRx(uint8_t* data, int size, UtzStandardHeader* standardHeader, uint64_t pipe);

// TZIotDComInit 初始化
void TZIotDComInit(void) {
    DComLoadParam param;
    param.Mid = TZIotMid;
    param.BlockRetryInterval = TZIotGetDComRetryInterval();
    param.BlockRetryMaxNum = TZIotGetDComRetryNum();
    param.IsAllowSend = TZIotPipeIsAllowSend;
    param.Send = dcomSend;
    DComLoad(param);
    AsyncStart(dcomRun, ASYNC_NO_WAIT);

    TZIotStandardLayerRegisterRxObserver(dcomDealStandardLayerRx);
}

static int dcomRun(void) {
    static struct pt pt;

    PT_BEGIN(&pt);

    DComRun();

    PT_END(&pt);
}

static void dcomSend(int protocol, uint64_t pipe, uint64_t dstIA, uint8_t* bytes, int size) {
    TZ_UNUSED(protocol);

    TZBufferDynamic* flpFrame = UtzBytesToFlpFrame(bytes, size, true, 0);
    if (flpFrame == NULL) {
        LE(TZIOT_TAG, "bytes to flp frame failed");
        return;
    }

    UtzStandardHeader header;
    header.Version = UTZ_NLV1_PROTOCOL_VERSION;
    header.FrameIndex = UtzGenerateFrameIndex();
    header.PayloadLen = (uint16_t)flpFrame->len;
    header.NextHead = UTZ_HEADER_FLP;
    header.HopsLimit = 0xff;
    header.SrcIA = TZIotGetLocalIA();
    header.DstIA = dstIA;

    TZIotStandardLayerSend(flpFrame->buf, flpFrame->len, &header, pipe);
    TZFree(flpFrame);
}

// dcomDealStandardLayerRx 处理标准层回调函数
static void dcomDealStandardLayerRx(uint8_t* data, int size, UtzStandardHeader* standardHeader, uint64_t pipe) {
    if (standardHeader->DstIA != TZIotGetLocalIA() || standardHeader->NextHead != UTZ_HEADER_FLP) {
        return;
    }

    TZBufferDynamic* body = UtzFlpFrameToBytes(data, size);
    if (body == NULL) {
        LW(TZIOT_TAG, "flp frame to bytes failed!src ia:0x%llx", standardHeader->SrcIA);
        return;
    }
    DComReceive(TZIOT_PROTOCOL_NUM, pipe, standardHeader->SrcIA, body->buf, body->len);
    TZFree(body);
}
