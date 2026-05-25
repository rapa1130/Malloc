#pragma once

#include<windows.h>
#include"bhGUID.h"

#ifndef interface
#define __STRUCT__ struct
#define interface __STRUCT__
#else
#define bhInterface __interface
#endif


#define BH_IID const BHID&
#define BH_DECLSPEC_UUID(x) __declspec(__uuidof(x))
#define BH_DECLSPEC_NOVTABLE __declspec(novtable) 
#define BH_MIDL_INTERFACE(x) struct BH_DECLSPEC_UUID(x) BH_DECLSPEC_NOVTABLE
#define BH_INTERFACE  struct BH_DECLSPEC_UUID(x) BH_DECLSPEC_NOVTABLE


#define BH_DECLARE_INTERFACE(iFace) interface BH_DECLSPEC_NOVTABLE iFace
#define BH_DECLARE_INTERFACE_(iFace,baseiFace) interface BH_DECLSPEC_NOVTABLE iFace : public baseiFace


#define BEGIN_INTERFACE
#define END_INTERFACE


#define BH_STDMETHODCALLTYPE __stdcall


#ifndef _in_
#define _in_
#define _out_
#define _opt_
#define _in_out_
#define _out_opt_
#define _in_opt_
#endif 


#ifndef pure
#define pure =0
#endif 


bhInterface bhIUnknown
{
public:
	virtual ULONG AddRef (void) pure;
	virtual ULONG Release(void) pure;
	virtual int QueryInterface(BH_IID bhid, _out_ void** ppvObj) pure;
};
