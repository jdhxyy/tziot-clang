// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// ��׼�㴦��ģ��
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_STANDARD_LAYER_H
#define TZIOT_STANDARD_LAYER_H

#include <stdint.h>
#include <stdbool.h>

#include "utz.h"

// TZIotStandardLayerRxCallback ���ջص�����
typedef void (*TZIotStandardLayerRxCallback)(uint8_t* data, int size, UtzStandardHeader* standardHeader, uint64_t pipe, 
    uint8_t* ip, int port);

// TZIotStandardLayerRx ��׼�����
void TZIotStandardLayerRx(uint64_t pipe, uint8_t* data, int size, uint8_t* ip, int port);

// TZIotStandardLayerRegisterRxObserver ע��۲���
void TZIotStandardLayerRegisterRxObserver(TZIotStandardLayerRxCallback callback);

// TZIotStandardLayerSend ���ڱ�׼ͷ������
void TZIotStandardLayerSend(uint8_t* data, int dataLen, UtzStandardHeader* standardHeader, uint64_t pipe, 
    uint8_t* ip, int port);

#endif
