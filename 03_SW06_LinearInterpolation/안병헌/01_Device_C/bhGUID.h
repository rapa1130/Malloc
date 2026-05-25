#pragma once
#include<Windows.h>

struct BHID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data[8];
};

EXTERN_C const BHID BH_IID_IUnknown;
EXTERN_C const BHID BH_IID_IBH;
EXTERN_C const BHID BH_IID_IBHDevice9;
EXTERN_C const BHID BH_IID_IBHVertexBuffer9;


BOOL bhIsEqualIID(const BHID& left, const BHID& right);

