// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 海萤物联网sdk
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_H
#define TZIOT_H

#include "dcom.h"
#include "tztype.h"

// TZIotNetSendFunc 网络管道发送函数类型
// dstIP是目标IP地址,4字节数组.dstPort是目标端口号
typedef void (*TZIotNetSendFunc)(uint8_t* data, int size, uint8_t* dstIP, int dstPort);

// TZIotLoad 模块载入
// mid是内存ID.ia是本节点IA地址,pwd是本节点密码
void TZIotLoad(int mid, uint64_t ia, char* pwd);

// TZIotBindPipe 绑定管道.绑定成功后返回管道号,管道号如果是0表示绑定失败
// 注意:绑定的第一个网络管道是连接核心网的管道
uint64_t TZIotBindPipeNet(TZIotNetSendFunc send, TZIsAllowSendFunc isAllowSend);

// TZIotPipeNetReceive 网络管道接收.pipe是接收管道号
// dstIP是目标IP地址,4字节数组.dstPort是目标端口号
// 用户在管道中接收到数据时需回调本函数
void TZIotPipeNetReceive(uint64_t pipe, uint8_t* data, int size, uint8_t* dstIP, int dstPort);

// TZIotBindPipe 绑定管道.绑定成功后返回管道号,管道号如果是0表示绑定失败
uint64_t TZIotBindPipe(TZDataFunc send, TZIsAllowSendFunc isAllowSend);

// TZIotPipeReceive 管道接收.pipe是接收管道号
// 用户在管道中接收到数据时需回调本函数
void TZIotPipeReceive(uint64_t pipe, uint8_t* data, int size);

// TZIotRegister 注册服务回调函数
// 如果rid对应的回调已经存在,则会使用新的替换旧的
// 如果注册失败,原因是内存不足
bool TZIotRegister(int rid, DComCallbackFunc callback);

// TZIotCallCreateHandle 创建同步调用句柄
// pipe是通信管道
// timeout是超时时间,单位:ms
// resp为NULL,respLen为NULL,timeout为0,有一个条件满足就表示不需要应答
// 本函数调用后需调用TZIotCall进行RPC通信,调用结果result中存储的是错误码.非DCOM_OK表示调用失败
// 调用成功后,应答数据保存在resp中,注意要释放
// 返回句柄.非0表示创建成功
intptr_t TZIotCallCreateHandle(uint64_t pipe, uint64_t dstIA, int rid, int timeout, uint8_t* req, int reqLen,
    uint8_t** resp, int* respLen, int* result);

// TZIotCall 通过协程的方式进行DCOM的RPC同步调用
// 注意本函数是PT协程方式调用的,需要使用PT_WAIT_THREAD等待函数调用结束
// 返回值是PT行号
int TZIotCall(intptr_t handle);

// TZIotIsConn 是否连接核心网
bool TZIotIsConn(void);

// TZIotConfigCoreParam 配置核心网参数
void TZIotConfigCoreParam(uint64_t ia, char* ip, uint16_t port);

// TZIotConfigDComParam 配置dcom参数
// retryNum: 重发次数
// retryInterval: 重发间隔.单位:ms
void TZIotConfigDComParam(int retryNum, int retryInterval);

#endif
