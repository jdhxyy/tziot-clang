// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// ��ө������sdk
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_H
#define TZIOT_H

#include "dcom.h"
#include "tztype.h"

// TZIotNetSendFunc ����ܵ����ͺ�������
// dstIP��Ŀ��IP��ַ,4�ֽ�����.dstPort��Ŀ��˿ں�
typedef void (*TZIotNetSendFunc)(uint8_t* data, int size, uint8_t* dstIP, int dstPort);

// TZIotLoad ģ������
// mid���ڴ�ID.ia�Ǳ��ڵ�IA��ַ,pwd�Ǳ��ڵ�����
void TZIotLoad(int mid, uint64_t ia, char* pwd);

// TZIotBindPipe �󶨹ܵ�.�󶨳ɹ��󷵻عܵ���,�ܵ��������0��ʾ��ʧ��
// ע��:�󶨵ĵ�һ������ܵ������Ӻ������Ĺܵ�
uint64_t TZIotBindPipeNet(TZIotNetSendFunc send, TZIsAllowSendFunc isAllowSend);

// TZIotPipeNetReceive ����ܵ�����.pipe�ǽ��չܵ���
// dstIP��Ŀ��IP��ַ,4�ֽ�����.dstPort��Ŀ��˿ں�
// �û��ڹܵ��н��յ�����ʱ��ص�������
void TZIotPipeNetReceive(uint64_t pipe, uint8_t* data, int size, uint8_t* dstIP, int dstPort);

// TZIotBindPipe �󶨹ܵ�.�󶨳ɹ��󷵻عܵ���,�ܵ��������0��ʾ��ʧ��
uint64_t TZIotBindPipe(TZDataFunc send, TZIsAllowSendFunc isAllowSend);

// TZIotPipeReceive �ܵ�����.pipe�ǽ��չܵ���
// �û��ڹܵ��н��յ�����ʱ��ص�������
void TZIotPipeReceive(uint64_t pipe, uint8_t* data, int size);

// TZIotRegister ע�����ص�����
// ���rid��Ӧ�Ļص��Ѿ�����,���ʹ���µ��滻�ɵ�
// ���ע��ʧ��,ԭ�����ڴ治��
bool TZIotRegister(int rid, DComCallbackFunc callback);

// TZIotCallCreateHandle ����ͬ�����þ��
// pipe��ͨ�Źܵ�
// timeout�ǳ�ʱʱ��,��λ:ms
// respΪNULL,respLenΪNULL,timeoutΪ0,��һ����������ͱ�ʾ����ҪӦ��
// ���������ú������TZIotCall����RPCͨ��,���ý��result�д洢���Ǵ�����.��DCOM_OK��ʾ����ʧ��
// ���óɹ���,Ӧ�����ݱ�����resp��,ע��Ҫ�ͷ�
// ���ؾ��.��0��ʾ�����ɹ�
intptr_t TZIotCallCreateHandle(uint64_t pipe, uint64_t dstIA, int rid, int timeout, uint8_t* req, int reqLen,
    uint8_t** resp, int* respLen, int* result);

// TZIotCall ͨ��Э�̵ķ�ʽ����DCOM��RPCͬ������
// ע�Ȿ������PTЭ�̷�ʽ���õ�,��Ҫʹ��PT_WAIT_THREAD�ȴ��������ý���
// ����ֵ��PT�к�
int TZIotCall(intptr_t handle);

// TZIotIsConn �Ƿ����Ӻ�����
bool TZIotIsConn(void);

// TZIotConfigCoreParam ���ú���������
void TZIotConfigCoreParam(uint64_t ia, char* ip, uint16_t port);

// TZIotConfigDComParam ����dcom����
// retryNum: �ط�����
// retryInterval: �ط����.��λ:ms
void TZIotConfigDComParam(int retryNum, int retryInterval);

#endif
