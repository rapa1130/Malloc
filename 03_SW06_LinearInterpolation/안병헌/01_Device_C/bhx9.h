#include"bhUnknownbase.h"
#include"bhx9types.h"
#include"bhDefine.h"

bhInterface IBH;
bhInterface IBHDevice9;
typedef IBHDevice9 IBHDEVICE9;
typedef IBHDevice9* LPBHDEVICE9;

typedef IBH* LPBH;
typedef IBH IBH9;
typedef IBH9* LPBH9;

#define BH_VERSION 9


bhInterface IBH : public bhIUnknown
{
public:
	virtual int CreateDevice(
		HWND hWnd,
		BHPRESENT_PARAMETERS* pp,
		DWORD vp,
		_out_ LPBHDEVICE9* pDev) 
		pure;
};


IBH* BHCreate9(DWORD ver);

bhInterface IBHResource9 : public bhIUnknown
{
	virtual int GetPrivateData(_out_ void** ppBuffer, _out_ UINT * pSizeData) pure;
	virtual void* GetBuffer(void) pure;
};

bhInterface IBHVertexBuffer9 : public IBHResource9
{
	virtual int Lock(UINT OffsetToLock,UINT SizeToLock,void** ppbData, DWORD Flags) pure;
	virtual int Unlock(void) pure;
	virtual int GetDesc(_out_ B3BVERTEXBUFFER_DESC* pDesc) pure;

	virtual int GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;
	virtual void* GetBuffer(void) pure;
	virtual UINT GetVertexCount(void) pure;
};

typedef IBHVertexBuffer9* LPVB;
typedef IBHVertexBuffer9* LPBHVERTEXBUFFER9;
typedef IBHVertexBuffer9* LPVBBUFFER;


bhInterface IBHDevice9 : public bhIUnknown
{
	
	virtual int Clear(COLORREF col) pure;
	virtual int BeginScene() pure;
	virtual int EndScene() pure;
	virtual int Present() pure;

	virtual HDC GetRT() pure;
	virtual COLORREF GetBkColor() pure;

	virtual int CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, B3BPOOL Pool,_out_ IBHVertexBuffer9** ppVB, _in_opt_ HANDLE* pSharedHandle) pure;
	virtual int SetStreamSource(UINT streamNumber, IBHVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride) pure;
	virtual int SetFVF(DWORD fvf) pure;
	virtual int DrawPrimitive(B3BPRIMITIBETYPE PrimitiveType, UINT OffsetBytes, UINT PrimitiveCount) pure;

	virtual int SetRenderState(B3BRENDERSTATETYPE State, DWORD Value) pure;
	virtual int GetRenderState(B3BRENDERSTATETYPE State, DWORD* Value) pure;
};


