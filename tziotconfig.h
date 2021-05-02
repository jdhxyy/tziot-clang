// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// �����ļ�
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_CONFIG_H
#define TZIOT_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#define TZIOT_TAG "tziot"
#define TZIOT_MALLOC_SIZE 8192

// ���֡�ֽ���
#define TZIOT_FRAME_MAX_LEN 4096

#define TZIOT_PROTOCOL_NUM 0

// Ĭ��DCOM����
// dcom�ط�����
#define TZIOT_DCOM_RETRY_NUM 5
// dcom�ط����.��λ:ms
#define TZIOT_DCOM_RETRY_INTERVAL 500

// �ڴ�id
extern int TZIotMid;

// TZIotSetLocalIA ���ñ���IA��ַ
void TZIotSetLocalIA(uint64_t ia);

// TZIotGetLocalIA ��ȡ����IA��ַ
uint64_t TZIotGetLocalIA(void);

// TZIotSetLocalPwd ��������
void TZIotSetLocalPwd(char* pwd);

// TZIotGetLocalPwd ��ȡ����
char* TZIotGetLocalPwd(void);

// TZIotGetCoreIA ��ȡ������IA��ַ
uint64_t TZIotGetCoreIA(void);

// TZIotGetCorePipe ��ȡ�������ܵ�
uint64_t TZIotGetCorePipe(void);

// TZIotGetDComRetryNum ��ȡdcom�ط�����
int TZIotGetDComRetryNum(void);

// TZIotGetDComRetryInterval ��ȡdcom�ط����
int TZIotGetDComRetryInterval(void);

// TZIotIsDirectConn �Ƿ�ֱ��
bool TZIotIsDirectConn(void);

#endif
