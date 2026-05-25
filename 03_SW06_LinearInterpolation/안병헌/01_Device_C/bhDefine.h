#pragma once
#include<Windows.h>
#pragma warning(disable : 4305)

#define YES 0
#define NO 1

#define BH_INVALIED(res)  ((res) == NULL)
#define BH_VALIED(res)  ((res) != NULL)
#define BH_FAILED(res)  ((res) < 0)
#define BH_SUCCEEDED(res) ((res) == BH_OK)

#define BH_ENABLED(val) ((val) == TRUE)
#define BH_DISABLED(val) ((val)== FALSE)

#define BH_CHECK BH_ENABLED

#define BH_OK 0
#define BH_FAIL -1
#define BH_FALSE BH_FAIL

#include<assert.h>
#define ASSERT(val) assert(val)

#define BH_PI 3.141592
#define bhConvertToRadian(deg) ((float)deg)*((float)BH_PI/180.0f)
#define B3B_PI BH_PI
#define MAKERADIAN bhConvertToRadian

#ifndef SafeDelete
#define SafeDelete(pBuff) if(pBuff){delete (pBuff); (pBuff) = NULL;}
#define SafeDelArray(pBuff) if(pBuff){delete[] (pBuff); (pBuff) = NULL;}
#define SafeRelease(pBuff) if(pBuff){(pBuff)->Release(); (pBuff) = NULL;}
#endif 

