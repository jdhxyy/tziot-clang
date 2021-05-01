// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 标准层处理模块
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_STANDARD_LAYER_H
#define TZIOT_STANDARD_LAYER_H

#include <stdint.h>
#include <stdbool.h>

#include "utz.h"

// TZIotStandardLayerRxCallback 接收回调函数
typedef void (*TZIotStandardLayerRxCallback)(uint8_t* data, int size, UtzStandardHeader* standardHeader, uint64_t pipe, 
    uint8_t* ip, int port);

// TZIotStandardLayerRx 标准层接收
void TZIotStandardLayerRx(uint64_t pipe, uint8_t* data, int size, uint8_t* ip, int port);

// TZIotStandardLayerRegisterRxObserver 注册观察者
void TZIotStandardLayerRegisterRxObserver(TZIotStandardLayerRxCallback callback);

// TZIotStandardLayerSend 基于标准头部发送
void TZIotStandardLayerSend(uint8_t* data, int dataLen, UtzStandardHeader* standardHeader, uint64_t pipe, 
    uint8_t* ip, int port);

#endif
