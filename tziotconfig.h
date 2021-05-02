// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 配置文件
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_CONFIG_H
#define TZIOT_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#define TZIOT_TAG "tziot"
#define TZIOT_MALLOC_SIZE 8192

// 最大帧字节数
#define TZIOT_FRAME_MAX_LEN 4096

#define TZIOT_PROTOCOL_NUM 0

// 默认DCOM参数
// dcom重发次数
#define TZIOT_DCOM_RETRY_NUM 5
// dcom重发间隔.单位:ms
#define TZIOT_DCOM_RETRY_INTERVAL 500

// 内存id
extern int TZIotMid;

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
