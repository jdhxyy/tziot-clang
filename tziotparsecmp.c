// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// CMP协议解析处理
// Authors: jdh99 <jdh821@163.com>

#include "tziotparsecmp.h"
#include "tziotstandardlayer.h"
#include "tziotconfig.h"

#include "utz.h"
#include "tzmalloc.h"
#include "knock.h"
#include "lagan.h"

#include <string.h>

static void dealStandardLayerRx(uint8_t* data, int size, UtzStandardHeader* standardHeader, uint64_t pipe);

// TZIotParseCmpInit 初始化
void TZIotParseCmpInit(void) {
    TZIotStandardLayerRegisterRxObserver(dealStandardLayerRx);
}

static void dealStandardLayerRx(uint8_t* data, int size, UtzStandardHeader* standardHeader, uint64_t pipe) {
    TZ_UNUSED(pipe);

    if (standardHeader->DstIA != TZIotGetLocalIA() || standardHeader->NextHead != UTZ_HEADER_CMP) {
        return;
    }

    TZBufferDynamic* payload = UtzFlpFrameToBytes(data, size);
    if (payload == NULL) {
        LW(TZIOT_TAG, "parse cmp failed.flp frame to bytes failed");
        return;
    }

    KnockCall(UTZ_HEADER_CMP, payload->buf[0], payload->buf + 1, payload->len - 1, NULL, NULL);
    TZFree(payload);
}
