// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// �ܵ�����
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_PIPE_H
#define TZIOT_PIPE_H

#include <stdint.h>
#include <stdbool.h>

// TZIotPipeInit ��ʼ��
void TZIotPipeInit(void);

// TZIotPipeIsAllowSend �ܵ��Ƿ�������
bool TZIotPipeIsAllowSend(uint64_t pipe);

// TZIotPipeSend �ܵ�����
void TZIotPipeSend(uint64_t pipe, uint8_t* data, int size);

#endif
