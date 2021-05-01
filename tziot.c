// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// ��ө������sdk
// Authors: jdh99 <jdh821@163.com>

#include "tziot.h"
#include "tziotconfig.h"
#include "tziotpipe.h"

#include "dcom.h"
#include "utz.h"

#include <string.h>

// TZIotCallCreateHandle ����ͬ�����þ��
// pipe��ͨ�Źܵ�
// timeout�ǳ�ʱʱ��,��λ:ms
// respΪNULL,respLenΪNULL,timeoutΪ0,��һ����������ͱ�ʾ����ҪӦ��
// ���������ú������DComRpcCallCoroutine����RPCͨ��,���ý��result�д洢���Ǵ�����.��DCOM_OK��ʾ����ʧ��
// ���óɹ���,Ӧ�����ݱ�����resp��,ע��Ҫ�ͷ�
// ���ؾ��.��0��ʾ�����ɹ�
intptr_t TZIotCallCreateHandle(uint64_t pipe, uint64_t dstIA, int rid, int timeout, uint8_t* req, int reqLen,
    uint8_t** resp, int* respLen, int* result) {
    if (Parent.IA == UTZ_IA_INVALID || Parent.IsConn == false) {
        if (result != NULL) {
            *result = DCOM_SYSTEM_ERROR_CODE_RX_TIMEOUT;
        }
        return 0;
    }
    return DComCallCreateHandle(TZIOT_PROTOCOL_NUM, pipe, dstIA, rid, timeout, req, reqLen, resp, respLen, result);
}

// TZIotCall ͨ��Э�̵ķ�ʽ����DCOM��RPCͬ������
// ע�Ȿ������PTЭ�̷�ʽ���õ�,��Ҫʹ��PT_WAIT_THREAD�ȴ��������ý���
// ����ֵ��PT�к�
int TZIotCall(intptr_t handle) {
    return DComCall(handle);
}

// TZIotRegister ע�����ص�����
// ���rid��Ӧ�Ļص��Ѿ�����,���ʹ���µ��滻�ɵ�
// ���ע��ʧ��,ԭ�����ڴ治��
bool TZIotRegister(int rid, DComCallbackFunc callback) {
    return DComRegister(TZIOT_PROTOCOL_NUM, rid, callback);
}
