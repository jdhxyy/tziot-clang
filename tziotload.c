// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// ģ������
// Authors: jdh99 <jdh821@163.com>

#include "tziotload.h"
#include "tziotpipe.h"
#include "tziotdcom.h"
#include "tziotconfig.h"
#include "tziotapply.h"
#include "tziotconn.h"

// TZIotLoad ģ������
// mid���ڴ�ID.ia�Ǳ��ڵ�IA��ַ,pwd�Ǳ��ڵ�����
void TZIotLoad(int mid, uint64_t ia, char* pwd) {
    TZIotMid = mid;
    TZIotSetLocalIA(ia);
    TZIotSetLocalPwd(pwd);

    TZIotConfigInit();
    TZIotPipeInit();
    TZIotDComInit();
    TZIotApplyInit();
    TZIotConnInit();
}
