// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// �����ļ�
// Authors: jdh99 <jdh821@163.com>

#include "tziotconfig.h"
#include "tziot.h"

#include "tzmalloc.h"
#include "lagan.h"
#include "tztype.h"
#include "dcom.h"

#include <stdio.h>
#include <string.h>

// �ڴ�id
int TZIotMid = -1;

// ����������ַ
static uint64_t localIA = 0;
static char* localPwd = NULL;

// dcom����
// dcom�ط�����
static int dcomRetryNum = TZIOT_DCOM_RETRY_NUM;
// dcom�ط����.��λ:ms
static int dcomRetryInterval = TZIOT_DCOM_RETRY_INTERVAL;

// ����ģʽ
static bool isDirectConn = true;

// TZIotConfigDComParam ����dcom����
// retryNum: �ط�����
// retryInterval: �ط����.��λ:ms
void TZIotConfigDComParam(int retryNum, int retryInterval) {
    dcomRetryNum = retryNum;
    dcomRetryInterval = retryInterval;
}

// TZIotConfigConnMode ��������ģʽ.Ĭ��ֱ��
// ֱ��ʱtziot�����Զ��������ע������
void TZIotConfigConnMode(bool isDirect) {
    isDirectConn = isDirect;
}

// TZIotSetLocalIA ���ñ���IA��ַ
void TZIotSetLocalIA(uint64_t ia) {
    localIA = ia;
}

// TZIotGetLocalIA ��ȡ����IA��ַ
uint64_t TZIotGetLocalIA(void) {
    return localIA;
}

// TZIotSetLocalPwd ��������
void TZIotSetLocalPwd(char* pwd) {
    localPwd = TZMalloc(TZIotMid, (int)strlen(pwd) + 1);
    if (localPwd == NULL) {
        LE(TZIOT_TAG, "set local pwd failed!");
        return;
    }
    strcpy(localPwd, pwd);
}

// TZIotGetLocalPwd ��ȡ����
char* TZIotGetLocalPwd(void) {
    return localPwd;
}

// TZIotGetDComRetryNum ��ȡdcom�ط�����
int TZIotGetDComRetryNum(void) {
    return dcomRetryNum;
}

// TZIotGetDComRetryInterval ��ȡdcom�ط����
int TZIotGetDComRetryInterval(void) {
    return dcomRetryInterval;
}

// TZIotIsDirectConn �Ƿ�ֱ��
bool TZIotIsDirectConn(void) {
    return isDirectConn;
}
