// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// ���븸·��
// Authors: jdh99 <jdh821@163.com>

#include "tziotapply.h"
#include "tziotconfig.h"
#include "tziotpipe.h"
#include "tziotdcom.h"
#include "tziotstandardlayer.h"

#include "knock.h"
#include "utz.h"
#include "lagan.h"
#include "dcom.h"
#include "async.h"
#include "tzmalloc.h"
#include "pt.h"

#include <string.h>

// dealAssignSlaveRouter �������ӻ�֡
// ��ѭ˭����˭�ͷ�ԭ��,resp��Ҫ�ɻص�����ʹ��TZMalloc���ٿռ�,���÷������ͷ�
// respΪNULL����respLenΪ0��ʾ����ҪӦ��
// argp�ǿɱ����,������ʽ�������ɵ���˫��Լ��
static void dealAssignSlaveRouter(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp);

static int applyTask(void);
static void sendApplyFrame(void);

// TZIotApplyInit ��ʼ��
void TZIotApplyInit(void) {
    KnockRegister(UTZ_HEADER_CMP, UTZ_CMP_MSG_TYPE_ASSIGN_SLAVE_ROUTER, dealAssignSlaveRouter);
    AsyncStart(applyTask, TZIOT_APPLY_INTERVAL * ASYNC_SECOND);
}

// dealAssignSlaveRouter �������ӻ�֡
// ��ѭ˭����˭�ͷ�ԭ��,resp��Ҫ�ɻص�����ʹ��TZMalloc���ٿռ�,���÷������ͷ�
// respΪNULL����respLenΪ0��ʾ����ҪӦ��
// argp�ǿɱ����,������ʽ�������ɵ���˫��Լ��
static void dealAssignSlaveRouter(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp) {
    TZ_UNUSED(resp);
    TZ_UNUSED(respLen);
    TZ_UNUSED(argp);

    if (req == NULL || reqLen == 0) {
        LW(TZIOT_TAG, "deal apply failed.payload len is 0");
        return;
    }

    int j = 0;
    if (req[j] != 0) {
        LW(TZIOT_TAG, "deal apply failed.error code:%d", req[j]);
        return;
    }
    j++;

    if (reqLen != 16) {
        LW(TZIOT_TAG, "deal apply failed.payload len is wrong:%d", reqLen);
        return;
    }

    Parent.IA = UtzBytesToIA(req + j);
    j += UTZ_IA_LEN;

    uint8_t ip[4] = {0};
    memcpy(ip, req + j, 4);
    j += 4;
    uint16_t port = (uint16_t)((req[j] << 8) + req[j + 1]);
    j += 2;
    Parent.Pipe = DComAddrToPipe(ip, port);

    LI(TZIOT_TAG, "apply success.parent ia:0x%llx ip:%d.%d.%d.%d port:%d cost:%d", Parent.IA, ip[0], ip[1], ip[2], 
        ip[3], port, req[j]);
    return;
}

static int applyTask(void) {
    static struct pt pt;

    PT_BEGIN(&pt);

    // �������ͨ������������������
    if (TZIotIsDirectConn() == false || TZIotPipeIsAllowSend(TZIotGetFirstPipe()) == false || TZIotDComIsInit() == false ||
        Parent.IA != UTZ_IA_INVALID) {
        PT_EXIT(&pt);
    }

    LI(TZIOT_TAG, "send apply frame");
    sendApplyFrame();

    PT_END(&pt);
}

static void sendApplyFrame(void) {
    TZBufferDynamic* securityHeaderBytes = NULL;
    TZBufferDynamic* flpFrame = NULL;
    uint8_t* payload = NULL;

    securityHeaderBytes = UtzSimpleSecurityHeaderDataToBytes(UTZ_HEADER_CMP, TZIotGetLocalPwd());
    if (securityHeaderBytes == NULL) {
        LE(TZIOT_TAG, "send apply frame failed!simple security header to bytes failed!");
        goto EXIT;
    }

    uint8_t body[9] = {0};
    body[0] = UTZ_CMP_MSG_TYPE_REQUEST_SLAVE_ROUTER;
    UtzIAToBytes(Parent.IA, body + 1);

    flpFrame = UtzBytesToFlpFrame(body, 9, true, 0);
    if (flpFrame == NULL) {
        LE(TZIOT_TAG, "send apply frame failed!bytes to flp frame failed!");
        goto EXIT;
    }

    int payloadSize = securityHeaderBytes->len + flpFrame->len;
    payload = TZMalloc(TZIotMid, payloadSize);
    if (payload == NULL) {
        LE(TZIOT_TAG, "send apply frame failed!malloc failed!");
        goto EXIT;
    }
    memcpy(payload, securityHeaderBytes->buf, (size_t)securityHeaderBytes->len);
    memcpy(payload + securityHeaderBytes->len, flpFrame->buf, (size_t)flpFrame->len);

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
