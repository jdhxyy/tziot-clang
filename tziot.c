// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 海萤物联网sdk
// Authors: jdh99 <jdh821@163.com>

#include "tziot.h"
#include "tziotconfig.h"
#include "tziotpipe.h"

#include "dcom.h"
#include "utz.h"

#include <string.h>

// TZIotCallCreateHandle 创建同步调用句柄
// pipe是通信管道
// timeout是超时时间,单位:ms
// resp为NULL,respLen为NULL,timeout为0,有一个条件满足就表示不需要应答
// 本函数调用后需调用DComRpcCallCoroutine进行RPC通信,调用结果result中存储的是错误码.非DCOM_OK表示调用失败
// 调用成功后,应答数据保存在resp中,注意要释放
// 返回句柄.非0表示创建成功
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

// TZIotCall 通过协程的方式进行DCOM的RPC同步调用
// 注意本函数是PT协程方式调用的,需要使用PT_WAIT_THREAD等待函数调用结束
// 返回值是PT行号
int TZIotCall(intptr_t handle) {
    return DComCall(handle);
}

// TZIotRegister 注册服务回调函数
// 如果rid对应的回调已经存在,则会使用新的替换旧的
// 如果注册失败,原因是内存不足
bool TZIotRegister(int rid, DComCallbackFunc callback) {
    return DComRegister(TZIOT_PROTOCOL_NUM, rid, callback);
}
