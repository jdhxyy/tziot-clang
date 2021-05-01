// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// ���Ӹ�·��
// Authors: jdh99 <jdh821@163.com>

#include "tziotconn.h"
#include "tziotconfig.h"
#include "tziotpipe.h"
#include "tziotdcom.h"
#include "tziotstandardlayer.h"

#include "knock.h"
#include "utz.h"
#include "async.h"
#include "lagan.h"
#include "tztime.h"
#include "pt.h"
#include "tzmalloc.h"

#include <string.h>

static int connNum = 0;

// dealAckConnectParentRouter ����Ӧ������֡
// ��ѭ˭����˭�ͷ�ԭ��,resp��Ҫ�ɻص�����ʹ��TZMalloc���ٿռ�,���÷������ͷ�
// respΪNULL����respLenΪ0��ʾ����ҪӦ��
// argp�ǿɱ����,������ʽ�������ɵ���˫��Լ��
static void dealAckConnectParentRouter(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp);

static int connTask(void);
static void sendConnFrame(void);

// TZIotConnInit ��ʼ��
void TZIotConnInit(void) {
    KnockRegister(UTZ_HEADER_CMP, UTZ_CMP_MSG_TYPE_CONNECT_PARENT_ROUTER, dealAckConnectParentRouter);
    AsyncStart(connTask, TZIOT_CONN_INTERVAL * ASYNC_SECOND);
}

// dealAckConnectParentRouter ����Ӧ������֡
// ��ѭ˭����˭�ͷ�ԭ��,resp��Ҫ�ɻص�����ʹ��TZMalloc���ٿռ�,���÷������ͷ�
// respΪNULL����respLenΪ0��ʾ����ҪӦ��
// argp�ǿɱ����,������ʽ�������ɵ���˫��Լ��
static void dealAckConnectParentRouter(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp) {
    TZ_UNUSED(resp);
    TZ_UNUSED(respLen);
    TZ_UNUSED(argp);

    if (req == NULL || reqLen == 0) {
        LW(TZIOT_TAG, "deal conn failed.payload len is 0");
        return;
    }

    int j = 0;
    if (req[j] != 0) {
        LW(TZIOT_TAG, "deal conn failed.error code:%d", req[j]);
        return;
    }
    j++;

    if (reqLen != 2) {
        LW(TZIOT_TAG, "deal conn failed.payload len is wrong:%d", reqLen);
        return;
    }

    connNum = 0;
    Parent.IsConn = true;
    Parent.Cost = req[j];
    Parent.Timestamp = TZTimeGet();
    LI(TZIOT_TAG, "conn success.parent ia:0x%llx cost:%d", Parent.IA, Parent.Cost);
    return;
}

static int connTask(void) {
    static struct pt pt;

    PT_BEGIN(&pt);

    if (TZIotIsDirectConn() == false || TZIotPipeIsAllowSend(TZIotGetFirstPipe()) == false || TZIotDComIsInit() == false ||
        Parent.IA == UTZ_IA_INVALID) {
        PT_EXIT(&pt);
    }

    connNum++;
    if (connNum > TZIOT_CONN_NUM_MAX) {
        connNum = 0;
        Parent.IA = UTZ_IA_INVALID;
        LW(TZIOT_TAG, "conn num is too many!");
        PT_EXIT(&pt);
    }

    LI(TZIOT_TAG, "send conn frame");
    sendConnFrame();

    PT_END(&pt);
}

static void sendConnFrame(void) {
    TZBufferDynamic* securityHeaderBytes = NULL;
    TZBufferDynamic* flpFrame = NULL;
    uint8_t* payload = NULL;

    securityHeaderBytes = UtzSimpleSecurityHeaderDataToBytes(UTZ_HEADER_CMP, TZIotGetLocalPwd());
    if (securityHeaderBytes == NULL) {
        LE(TZIOT_TAG, "send conn frame failed!simple security header to bytes failed!");
        goto EXIT;
    }

    uint8_t body[TZ_BUFFER_TINY_LEN] = {0};
    int j = 0;
    body[j++] = UTZ_CMP_MSG_TYPE_REQUEST_SLAVE_ROUTER;
    // ǰ׺����
    body[j++] = 64;
    // ��Ĥ�ӻ��̶�������ַ
    j += UTZ_IA_LEN;
    // ����ֵ
    body[j++] = 0;

    flpFrame = UtzBytesToFlpFrame(body, j, true, 0);
    if (flpFrame == NULL) {
        LE(TZIOT_TAG, "send conn frame failed!bytes to flp frame failed!");
        goto EXIT;
    }

    int payloadSize = securityHeaderBytes->len + flpFrame->len;
    payload = TZMalloc(TZIotMid, payloadSize);
    if (payload == NULL) {
        LE(TZIOT_TAG, "send conn frame failed!malloc failed!");
        goto EXIT;
    }
    memcpy(payload, securityHeaderBytes->buf, (uint64_t)securityHeaderBytes->len);
    memcpy(payload + securityHeaderBytes->len, flpFrame->buf, (uint64_t)flpFrame->len);

    UtzStandardHeader header;
    header.Version = UTZ_NLV1_PROTOCOL_VERSION;
    header.FrameIndex = UtzGenerateFrameIndex();
    header.PayloadLen = (uint16_t)payloadSize;
    header.NextHead = UTZ_HEADER_SIMPLE_SECURITY;
    header.HopsLimit = 0xff;
    header.SrcIA = TZIotGetLocalIA();
    header.DstIA = TZIotGetCoreIA();

    TZIotStandardLayerSend(payload, payloadSize, &header, TZIotGetFirstPipe(), TZIotGetCorePipe());

EXIT:
    TZFree(securityHeaderBytes);
    TZFree(flpFrame);
    TZFree(payload);
}

// TZIotIsConn �Ƿ����Ӻ�����
bool TZIotIsConn(void) {
    return Parent.IA != UTZ_IA_INVALID && Parent.IsConn;
}
