// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 配置文件
// Authors: jdh99 <jdh821@163.com>

#include "tziotconfig.h"
#include "tziot.h"

#include "tzmalloc.h"
#include "lagan.h"
#include "tztype.h"
#include "dcom.h"

#include <stdio.h>
#include <string.h>

// 内存id
int TZIotMid = -1;

// 本机单播地址
static uint64_t localIA = 0;
static char* localPwd = NULL;

// dcom参数
// dcom重发次数
static int dcomRetryNum = TZIOT_DCOM_RETRY_NUM;
// dcom重发间隔.单位:ms
static int dcomRetryInterval = TZIOT_DCOM_RETRY_INTERVAL;

// 连接模式
static bool isDirectConn = true;

// TZIotConfigDComParam 配置dcom参数
// retryNum: 重发次数
// retryInterval: 重发间隔.单位:ms
void TZIotConfigDComParam(int retryNum, int retryInterval) {
    dcomRetryNum = retryNum;
    dcomRetryInterval = retryInterval;
}

// TZIotConfigConnMode 配置连接模式.默认直连
// 直连时tziot包会自动向核心网注册连接
void TZIotConfigConnMode(bool isDirect) {
    isDirectConn = isDirect;
}

// TZIotSetLocalIA 设置本地IA地址
void TZIotSetLocalIA(uint64_t ia) {
    localIA = ia;
}

// TZIotGetLocalIA 读取本地IA地址
uint64_t TZIotGetLocalIA(void) {
    return localIA;
}

// TZIotSetLocalPwd 设置密码
void TZIotSetLocalPwd(char* pwd) {
    localPwd = TZMalloc(TZIotMid, (int)strlen(pwd) + 1);
    if (localPwd == NULL) {
        LE(TZIOT_TAG, "set local pwd failed!");
        return;
    }
    strcpy(localPwd, pwd);
}

// TZIotGetLocalPwd 读取密码
char* TZIotGetLocalPwd(void) {
    return localPwd;
}

// TZIotGetDComRetryNum 读取dcom重发次数
int TZIotGetDComRetryNum(void) {
    return dcomRetryNum;
}

// TZIotGetDComRetryInterval 读取dcom重发间隔
int TZIotGetDComRetryInterval(void) {
    return dcomRetryInterval;
}

// TZIotIsDirectConn 是否直连
bool TZIotIsDirectConn(void) {
    return isDirectConn;
}
