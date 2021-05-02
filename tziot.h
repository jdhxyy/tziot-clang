// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// ��ө������sdk
// Authors: jdh99 <jdh821@163.com>

#ifndef TZIOT_H
#define TZIOT_H

#include "dcom.h"
#include "tztype.h"
#include "tzaccess.h"

// TZIotNetSendFunc ����ܵ����ͺ�������
// dstIP��Ŀ��IP��ַ,4�ֽ�����.dstPort��Ŀ��˿ں�
typedef void (*TZIotNetSendFunc)(uint8_t* data, int size, uint8_t* dstIP, int dstPort);

// TZIotLoad ģ������
// mid���ڴ�ID.ia�Ǳ��ڵ�IA��ַ,pwd�Ǳ��ڵ�����
void TZIotLoad(uint64_t ia, char* pwd);

// TZIotBindPipeCore �󶨺������ܵ�.�󶨳ɹ��󷵻عܵ���,�ܵ��������0��ʾ��ʧ��
// ע��:�������ܵ�ֻ�ܰ�һ��
uint64_t TZIotBindPipeCore(TZAccessSendFunc send, TZIsAllowSendFunc isAllowSend);

// TZIotPipeCoreReceive �������ܵ�����.pipe�ǽ��չܵ���
// srcIP��ԴIP��ַ,4�ֽ�����.srcPort��Դ�˿ں�
// �û��ڹܵ��н��յ�����ʱ��ص�������
void TZIotPipeCoreReceive(uint8_t* data, int size, uint8_t* srcIP, uint16_t srcPort);

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

// TZIotGetParentAddr ��ȡ���ڵ�ĵ�ַ
// ip��ַ�����ֽ�����.������ڵ㲻����,��ip��port��Ϊ0
void TZIotGetParentAddr(uint8_t* ip, uint16_t* port);

// TZIotConfigCoreParam ���ú���������.ip��4�ֽ�����
void TZIotConfigCoreParam(uint64_t ia, uint8_t* ip, uint16_t port);

// TZIotConfigDComParam ����dcom����
// retryNum: �ط�����
// retryInterval: �ط����.��λ:ms
void TZIotConfigDComParam(int retryNum, int retryInterval);

#endif
