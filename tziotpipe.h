// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 管道操作
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_PIPE_H
#define TZIOT_PIPE_H

#include <stdint.h>
#include <stdbool.h>

// TZIotPipeInit 初始化
void TZIotPipeInit(void);

// TZIotPipeIsAllowSend 管道是否允许发送
bool TZIotPipeIsAllowSend(uint64_t pipe);

// TZIotPipeSend 管道发送
void TZIotPipeSend(uint64_t pipe, uint8_t* data, int size);

#endif
