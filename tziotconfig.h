// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 配置文件
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_CONFIG_H
#define TZIOT_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#define TZIOT_TAG "tziot"

// 最大帧字节数
#define TZIOT_FRAME_MAX_LEN 4096

#define TZIOT_PROTOCOL_NUM 0

// 申请父路由间隔.单位:s
#define TZIOT_APPLY_INTERVAL 10

// 连接间隔.单位:s
#define TZIOT_CONN_INTERVAL 10
// 最大连接次数.超过连接次数这回清除父路由IA地址,重连父路由
#define TZIOT_CONN_NUM_MAX 5

// 默认核心网地址
#define TZIOT_CORE_IA 0x2141000000000002
#define TZIOT_CORE_IP "115.28.86.171"
#define TZIOT_CORE_PORT 12914

// 默认DCOM参数
// dcom重发次数
#define TZIOT_DCOM_RETRY_NUM 5
// dcom重发间隔.单位:ms
#define TZIOT_DCOM_RETRY_INTERVAL 500

#pragma pack(1)

// 父节点信息
typedef struct {
    uint64_t IA;
    uint64_t Pipe;
    uint8_t Cost;
    bool IsConn;
    uint64_t Timestamp;
} ParentInfo;

#pragma pack()

extern ParentInfo Parent;

// 内存id
extern int TZIotMid;

// TZIotConfigInit 初始化
void TZIotConfigInit(void);

// TZIotSetLocalIA 设置本地IA地址
void TZIotSetLocalIA(uint64_t ia);

// TZIotGetLocalIA 读取本地IA地址
uint64_t TZIotGetLocalIA(void);

// TZIotSetLocalPwd 设置密码
void TZIotSetLocalPwd(char* pwd);

// TZIotGetLocalPwd 读取密码
char* TZIotGetLocalPwd(void);

// TZIotGetCoreIA 读取核心网IA地址
uint64_t TZIotGetCoreIA(void);

// TZIotGetCorePipe 读取核心网管道
uint64_t TZIotGetCorePipe(void);

// TZIotGetDComRetryNum 读取dcom重发次数
int TZIotGetDComRetryNum(void);

// TZIotGetDComRetryInterval 读取dcom重发间隔
int TZIotGetDComRetryInterval(void);

// TZIotIsDirectConn 是否直连
bool TZIotIsDirectConn(void);

#endif
