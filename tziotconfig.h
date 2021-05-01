// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// �����ļ�
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_CONFIG_H
#define TZIOT_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#define TZIOT_TAG "tziot"

// ���֡�ֽ���
#define TZIOT_FRAME_MAX_LEN 4096

#define TZIOT_PROTOCOL_NUM 0

// ���븸·�ɼ��.��λ:s
#define TZIOT_APPLY_INTERVAL 10

// ���Ӽ��.��λ:s
#define TZIOT_CONN_INTERVAL 10
// ������Ӵ���.�������Ӵ�����������·��IA��ַ,������·��
#define TZIOT_CONN_NUM_MAX 5

// Ĭ�Ϻ�������ַ
#define TZIOT_CORE_IA 0x2141000000000002
#define TZIOT_CORE_IP "115.28.86.171"
#define TZIOT_CORE_PORT 12914

// Ĭ��DCOM����
// dcom�ط�����
#define TZIOT_DCOM_RETRY_NUM 5
// dcom�ط����.��λ:ms
#define TZIOT_DCOM_RETRY_INTERVAL 500

#pragma pack(1)

// ���ڵ���Ϣ
typedef struct {
    uint64_t IA;
    uint64_t Pipe;
    uint8_t Cost;
    bool IsConn;
    uint64_t Timestamp;
} ParentInfo;

#pragma pack()

extern ParentInfo Parent;

// �ڴ�id
extern int TZIotMid;

// TZIotConfigInit ��ʼ��
void TZIotConfigInit(void);

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
