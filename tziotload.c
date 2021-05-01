// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// 模块载入
// Authors: jdh99 <jdh821@163.com>

#include "tziotload.h"
#include "tziotpipe.h"
#include "tziotdcom.h"
#include "tziotconfig.h"
#include "tziotapply.h"
#include "tziotconn.h"

// TZIotLoad 模块载入
// mid是内存ID.ia是本节点IA地址,pwd是本节点密码
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
