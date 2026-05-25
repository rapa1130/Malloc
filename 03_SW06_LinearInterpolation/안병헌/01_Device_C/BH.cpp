#include "BH.h"
#include"bhError.h"
#include"bhGUID.h"
#include"bhMath.h"

using namespace BH;


class B3bh : public IBH
{
protected:
	ULONG m_ref = 0;
public:
	B3bh();
	virtual ~B3bh();
	virtual int CreateDevice(
		HWND hWnd,
		BHPRESENT_PARAMETERS* pp,
		DWORD vp,
		LPBHDEVICE9* pDev)
		;

	virtual ULONG AddRef(void);
	virtual ULONG Release(void);
	virtual int QueryInterface(BH_IID iid, _out_ void** ppvObj);
};

class B3bhDevice9 : public IBHDevice9
{
	friend class B3bh;
protected:
	ULONG m_ref = 0;
protected:
	HDC m_surfaceRT;
	HBITMAP m_hBitmap;
	HWND m_hWnd;
	COLORREF m_bkColor;
	BHPRESENT_PARAMETERS m_presentParam;
	DWORD m_vertexProcessing;

	IBHVertexBuffer9* m_pVB[1];
	DWORD m_fvf;
	UINT m_stride;

	UINT m_primCnt;
	UINT m_startVtx;

	DWORD m_rState[B3BRS_MAX_];

public:
	int Clear(COLORREF col);
	int BeginScene();
	int EndScene();
	int Present();

	HDC GetRT();
	COLORREF GetBkColor();

	B3bhDevice9();
	virtual ~B3bhDevice9();
	virtual ULONG AddRef(void);
	virtual ULONG Release(void);
	virtual int QueryInterface(BH_IID iid, _out_ void** ppvObj);

	virtual int CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, B3BPOOL Pool, _out_ IBHVertexBuffer9** ppVB, _in_opt_ HANDLE* pSharedHandle);
	virtual int SetStreamSource(UINT streamNumber, IBHVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride);
	virtual int SetFVF(DWORD fvf);
	virtual int DrawPrimitive(B3BPRIMITIBETYPE PrimitiveType, UINT startVertex, UINT PrimitiveCount);

	virtual int SetRenderState(B3BRENDERSTATETYPE State, DWORD Value);
	virtual int GetRenderState(B3BRENDERSTATETYPE rsType, _out_ DWORD* value);

protected:
	int _CreateRenderTarget();
	void _ReleaseRenderTarget();

	int _VertexPipeLine();
	int _GeometryPipeline();
	int _PixelPipeline();

	int _DrawLine(B3BVECTOR2 v0, B3BVECTOR2 v1);
	//int _DrawLine(B3BXVECTOR2 v0, B3BXVECTOR2 v1, DWORD c0, DWORD c1);
	int _DrawLine(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXCOLOR c0, B3BXCOLOR c1);
	int _DrawVerLine(B3BVECTOR2 v0, B3BVECTOR2 v1, DWORD c0, DWORD c1);
	int _DrawHorLine(B3BVECTOR2 v0, B3BVECTOR2 v1, DWORD c0, DWORD c1);

	int _DrawFace(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXVECTOR2 v2);
	//int _DrawFace(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXVECTOR2 v2, DWORD c0, DWORD c1, DWORD c2);
	int _DrawFace(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXVECTOR2 v2, B3BXCOLOR c0, B3BXCOLOR c1, B3BXCOLOR c2);
	int _EdgeFunction(B3BXVECTOR2 sp, B3BXVECTOR2 ep, B3BXVECTOR2 p);
	bool _FaceCulling(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXVECTOR2 v2);

	void _InitRenderState();
};

class B3BVertexBuffer9 : public IBHVertexBuffer9
{
	friend class B3bhDevice9;
protected:
	ULONG m_ref = 0;
protected:
	void* m_pVBuffer;
	DWORD m_sizeInBytes;
	UINT m_stride;
	DWORD m_fvf;
	BOOL m_bLocked;

	B3BVERTEXBUFFER_DESC m_desc;

public:
	B3BVertexBuffer9();
	virtual ~B3BVertexBuffer9();

	virtual int Lock(UINT offsetToLock, UINT sizeToLock, void** ppbData, DWORD flags);
	virtual int Unlock(void);
	virtual int GetDesc(_out_ B3BVERTEXBUFFER_DESC* pDesc);

	virtual int GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData);
	virtual void* GetBuffer(void);
	virtual UINT GetVertexCount(void);

	virtual ULONG AddRef(void);
	virtual ULONG Release(void);
	virtual int QueryInterface(BH_IID bhid, _out_ void** ppvObj);
protected:

	int _Create(B3BVERTEXBUFFER_DESC desc);
	int _Create(UINT length, DWORD fvf, B3BPOOL pool);

	DWORD _GetSize() { return m_sizeInBytes; }
	DWORD _GetStride() { return m_stride; }
	DWORD _GetFVF() { return m_fvf; }
	void* _GetVBuffer() { return m_pVBuffer; }


};
typedef B3BVertexBuffer9* LPB3BVERTEXBUFFER9;

IBH* BHCreate9(DWORD ver)
{
	B3bh* bh = new B3bh;
	assert(bh != NULL);

	IBH* pBh = NULL;
	if (BH_FAILED(bh->QueryInterface(BH_IID_IBH, (void**)&pBh)))
	{

	}
	return pBh;
}





B3bh::B3bh()
{
#ifdef _DEBUG
	bhLog(_T("B3bh 생성됨..."));
#endif 

}

B3bh::~B3bh()
{
#ifdef _DEBUG
	bhLog(_T("B3bh 제거됨..."));
#endif 
}

int B3bh::CreateDevice(HWND hWnd, BHPRESENT_PARAMETERS* pp, DWORD vp, LPBHDEVICE9* ppDevice)
{
	if (BH_INVALIED(hWnd)) return BH_FAIL;
	if (BH_INVALIED(pp)) return BH_FAIL;
	if (BH_INVALIED(ppDevice)) return BH_FAIL;

	B3bhDevice9* pDevice = new B3bhDevice9;
	assert(pDevice != NULL);


	pDevice->m_hWnd = hWnd;
	pDevice->m_presentParam = *pp;
	pDevice->m_vertexProcessing = vp;
	pDevice->_CreateRenderTarget();

	IBHDevice9* pDev = NULL;
	if (BH_FAILED(pDevice->QueryInterface(BH_IID_IBHDevice9, (void**)&pDev)))
	{

	}

	*ppDevice = pDev;
	return BH_OK;
}

ULONG B3bh::AddRef(void)
{
	return ++m_ref;
}

ULONG B3bh::Release(void)
{
	ULONG ref = --m_ref;
	if (ref <= 0) delete this;
	return ref;
}


int B3bh::QueryInterface(BH_IID iid, _out_ void** ppvObj)
{
	if (bhIsEqualIID(iid, BH_IID_IBH))
	{
		*ppvObj = dynamic_cast<B3bh*>(this);
		AddRef();
	}
	return BH_OK;
}







int B3bhDevice9::Clear(COLORREF col)
{
	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rc = { 0,0,(LONG)m_presentParam.width,(LONG)m_presentParam.height };
	FillRect(m_surfaceRT, &rc, hBrush);
	DeleteObject(hBrush);

	m_bkColor = col;

	return BH_OK;
}

int B3bhDevice9::BeginScene()
{
	SetBkMode(m_surfaceRT, TRANSPARENT);

	HPEN  hPen = (HPEN)GetStockObject(WHITE_PEN);
	SelectObject(m_surfaceRT, hPen);

	return BH_OK;
}

int B3bhDevice9::EndScene()
{
	return BH_OK;
}

int B3bhDevice9::Present()
{
	HDC hdc = GetDC(m_hWnd);
	BitBlt(hdc, 0, 0, 
		m_presentParam.width, m_presentParam.height, 
		m_surfaceRT, 0, 0, 
		SRCCOPY);
	ReleaseDC(m_hWnd, hdc);

	return BH_OK;
}

HDC B3bhDevice9::GetRT()
{
	return m_surfaceRT;
}

COLORREF B3bhDevice9::GetBkColor()
{
	return m_bkColor;
}



B3bhDevice9::B3bhDevice9()
{
	m_hWnd = NULL;
	m_bkColor = RGB(0, 0, 255);
	m_hBitmap = NULL;
	m_surfaceRT = NULL;
	ZeroMemory(&m_presentParam, sizeof(m_presentParam));

	m_fvf = 0;
	m_primCnt = 0;
	m_pVB[0] = NULL;
	m_startVtx = 0;
	m_stride = 0;

	_InitRenderState();

#ifdef _DEBUG
	bhLog(_T("B3bh Device 생성됨..."));
#endif 
}
B3bhDevice9::~B3bhDevice9()
{
	_ReleaseRenderTarget();

#ifdef _DEBUG
	bhLog(_T("B3bh Device 제거됨..."));
#endif 
}


int B3bhDevice9::_CreateRenderTarget()
{
	HDC hdc = GetDC(m_hWnd);

	m_surfaceRT = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, m_presentParam.width, m_presentParam.height);
	SelectObject(m_surfaceRT, m_hBitmap);

	ReleaseDC(m_hWnd, hdc);

	return BH_OK;
}

void B3bhDevice9::_ReleaseRenderTarget()
{
	DeleteObject(m_hBitmap);
	DeleteDC(m_surfaceRT);
}






ULONG B3bhDevice9::AddRef(void)
{
	return ++m_ref;
}

ULONG B3bhDevice9::Release(void)
{
	ULONG ref = --m_ref;
	if (ref <= 0) delete this;
	return ref;
}


int B3bhDevice9::QueryInterface(BH_IID iid, _out_ void** ppvObj)
{
	if (bhIsEqualIID(iid, BH_IID_IBHDevice9))
	{
		*ppvObj = dynamic_cast<B3bhDevice9*>(this);
		AddRef();
	}
	return BH_OK;
}



int B3bhDevice9::CreateVertexBuffer( UINT length, 
									 DWORD usage, 
									 DWORD fvf, 
									 B3BPOOL pool, 
									 _out_ IBHVertexBuffer9** ppVB, 
									 _in_opt_ HANDLE* pSharedHandle )
{

	LPB3BVERTEXBUFFER9 pB3VB = new B3BVertexBuffer9();
	if (BH_INVALIED(pB3VB)) return BH_FAIL;

	B3BVERTEXBUFFER_DESC desc = {};
	desc.FVF = fvf;
	desc.Pool = pool;
	desc.Size = length;
	desc.Usage = B3BUSAGE_WRITEONLY;
	desc.Format = B3BFMT_VERTEXDATA;
	desc.Type = B3BTYPE_VERTEXBUFFER;

	if (BH_FAILED(pB3VB->_Create(desc))) return BH_FAIL;

	IBHVertexBuffer9* pVB = nullptr;
	if (BH_FAILED(pB3VB->QueryInterface(BH_IID_IBHVertexBuffer9, (void**)&pVB))) return BH_FAIL;

	*ppVB = pVB;
	return BH_OK;
}

int B3bhDevice9::SetStreamSource(UINT streamNumber, IBHVertexBuffer9* pVB, UINT offsetBytes, UINT stride)
{
	if (BH_INVALIED(pVB)) return BH_FAIL;

	m_pVB[0] = pVB;
	m_stride = stride;

	return BH_OK;
}

int B3bhDevice9::SetFVF(DWORD fvf)
{
	m_fvf = fvf;
	return BH_OK;
}

int B3bhDevice9::DrawPrimitive(B3BPRIMITIBETYPE primitiveType, UINT startVertex, UINT primitiveCount)
{
	if (BH_INVALIED(m_pVB)) return BH_FAIL;

	m_primCnt = primitiveCount;
	m_startVtx = startVertex;

	_VertexPipeLine();
	_GeometryPipeline();
	_PixelPipeline();

	return BH_OK;
}

int B3bhDevice9::SetRenderState(B3BRENDERSTATETYPE state, DWORD value)
{
	m_rState[state] = value;
	return BH_OK;
}

int B3bhDevice9::GetRenderState(B3BRENDERSTATETYPE state, _out_ DWORD* value)
{
	*value = m_rState[state];
	return BH_OK;
}


int B3bhDevice9::_VertexPipeLine()
{
	return BH_OK;
}

int B3bhDevice9::_GeometryPipeline()
{
	return BH_OK;
}

int B3bhDevice9::_PixelPipeline()
{
	B3BVERTEXBUFFER_DESC desc = {};
	m_pVB[0]->GetDesc(&desc);
	UINT vtxCount = desc.Size / m_stride;
	void* pBuff = m_pVB[0]->GetBuffer();

	UINT primCnt = 0;
	BYTE* currVB = (BYTE*)pBuff;
	BYTE* currVtx = NULL;

	for (UINT i = m_startVtx; i < vtxCount; i += 3)
	{

		currVB = (BYTE*)pBuff + i * m_stride;

		currVtx = currVB;
		B3BXVECTOR2 v0 = *(B3BXVECTOR2*)(currVtx);
		//DWORD c0 = RGB(255, 255, 0);
		B3BXCOLOR c0(1.0f, 0.0f, 0.0f, 0.0f);

		currVtx = currVB + m_stride;
		B3BXVECTOR2 v1 = *(B3BXVECTOR2*)(currVtx);
		//DWORD c1 = RGB(0, 255, 0);
		B3BXCOLOR c1(0.0f, 1.0f, 0.0f, 0.0f);


		currVtx = currVB + m_stride * 2;
		B3BXVECTOR2 v2 = *(B3BXVECTOR2*)(currVtx);
		//DWORD c2 = RGB(0, 0, 255);
		B3BXCOLOR c2(0.0f, 0.0f, 1.0f, 0.0f);



		if (_FaceCulling(v0, v1, v2))
		{
			if (++primCnt >= m_primCnt)
				break;
			else
				continue;
		}


		if (m_rState[B3BRS_FILLMODE] == B3BFILLMODE::B3BFILL_SOLID)
		{
			//_DrawFace(v0, v1, v2);
			_DrawFace(v0, v1, v2, c0, c1, c2);
		}
		if(m_rState[B3BRS_FILLMODE]== B3BFILLMODE::B3BFILL_WIREFRAME)
		{
			//_DrawLine(v0, v1);
			//_DrawLine(v1, v2);
			//_DrawLine(v2, v0);
			_DrawLine(v0, v1, c0, c1);
			_DrawLine(v1, v2, c1, c2);
			_DrawLine(v2, v0, c2, c0);
		}

		if (++primCnt >= m_primCnt) break;
	}

	return BH_OK;
}



int B3bhDevice9::_DrawLine(B3BVECTOR2 v0, B3BVECTOR2 v1)
{
	COLORREF col = RGB(255, 255, 255);
	if (v0.x == v1.x)
	{
		//_DrawVerLine(v0, v1,);
		return BH_OK;
	}
	if (v0.y == v1.y)
	{
		//_DrawHorLine(v0, v1, col);
		return BH_OK;
	}
	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;
	int dxAbs = std::abs(dx);
	int dyAbs = std::abs(dy);

	if (dx > 0 && dy > 0)
	{
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX < v1.x; ++nowX)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY < v1.y; ++nowY)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else if (dx > 0 && dy < 0)
	{
		dy = std::abs(dy);
		if (dx > dy)
		{

			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX < v1.x; ++nowX)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY > v1.y; --nowY)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else if (dx < 0 && dy > 0)
	{
		dx = std::abs(dx);
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX > v1.x; --nowX)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY < v1.y; ++nowY)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else
	{
		dx = std::abs(dx);
		dy = std::abs(dy);
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX > v1.x; --nowX)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY > v1.y; --nowY)
			{
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	return BH_OK;
}

int B3bhDevice9::_DrawLine(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXCOLOR c0, B3BXCOLOR c1)
{
	COLORREF col = RGB(255, 255, 255);
	if (v0.x == v1.x)
	{
		_DrawVerLine(v0, v1, c0, c1);
		return BH_OK;
	}
	if (v0.y == v1.y)
	{
		_DrawHorLine(v0, v1, c0, c1);
		return BH_OK;
	}
	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;

	if (dx > 0 && dy > 0)
	{
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX < v1.x; ++nowX)
			{
				bhLerp(&col, c0, c1, (float)(nowX - v0.x) / dx);
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY < v1.y; ++nowY)
			{
				bhLerp(&col, c0, c1, (float)(nowY - v0.y) / dy);
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else if (dx > 0 && dy < 0)
	{
		dy = std::abs(dy);
		if (dx > dy)
		{

			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX < v1.x; ++nowX)
			{
				bhLerp(&col, c0, c1, (float)(nowX - v0.x) / dx);

				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY > v1.y; --nowY)
			{
				bhLerp(&col, c0, c1, (float)(v0.y - nowY) / dy);
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else if (dx < 0 && dy > 0)
	{
		dx = std::abs(dx);
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX > v1.x; --nowX)
			{
				bhLerp(&col, c0, c1, (float)(v0.x - nowX) / dx);
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					++nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY < v1.y; ++nowY)
			{
				bhLerp(&col, c0, c1, (float)(nowY - v0.y) / dy);
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else
	{
		dx = std::abs(dx);
		dy = std::abs(dy);
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = v0.y;
			for (int nowX = v0.x; nowX > v1.x; --nowX)
			{
				bhLerp(&col, c0, c1, (float)(v0.x - nowX) / dx);
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = v0.x;
			for (int nowY = v0.y; nowY > v1.y; --nowY)
			{
				bhLerp(&col, c0, c1, (float)(v0.y - nowY) / dy);
				SetPixel(m_surfaceRT, nowX, nowY, col);
				if (f > 0)
				{
					--nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	return BH_OK;
}

int B3bhDevice9::_DrawVerLine(B3BVECTOR2 v0, B3BVECTOR2 v1,DWORD c0, DWORD c1)
{
	int dy = v1.y - v0.y;
	COLORREF col;
	if (dy < 0)
	{
		for (int nowY = v0.y; nowY > v1.y; --nowY)
		{
			bhLerp(&col, c0, c1, (float)(nowY - v0.y) / dy);
			SetPixel(m_surfaceRT, v0.x, nowY, col);
		}
	}
	else
	{
		for (int nowY = v0.y; nowY < v1.y; ++nowY)
		{
			bhLerp(&col, c0, c1, (float)(nowY - v0.y) / dy);
			SetPixel(m_surfaceRT, v0.x, nowY, col);
		}
	}
	
	return BH_OK;
}

int B3bhDevice9::_DrawHorLine(B3BVECTOR2 v0, B3BVECTOR2 v1, DWORD c0, DWORD c1)
{
	int dx = v1.x - v0.x;
	COLORREF col;
	if (dx < 0)
	{
		for (int nowX = v0.x; nowX > v1.x; nowX--)
		{
			bhLerp(&col, c0, c1, (float)(nowX - v0.x) / dx);
			SetPixel(m_surfaceRT, nowX, v0.y, col);
		}
	}
	else
	{
		for (int nowX = v0.x; nowX < v1.x; nowX++)
		{
			bhLerp(&col, c0, c1, (float)(nowX - v0.x) / dx);
			SetPixel(m_surfaceRT, nowX, v0.y, col);
		}

	}
	
	return BH_OK;
}

int B3bhDevice9::_DrawFace(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXVECTOR2 v2)
{
	int xMin = min(v0.x, min(v1.x, v2.x));
	int xMax = max(v0.x, max(v1.x, v2.x));
	int yMax = max(v0.y, max(v1.y, v2.y));
	int yMin = min(v0.y, min(v1.y, v2.y)); 


	for (int x = xMin; x < xMax; x++)
	{
		for (int y = yMin; y < yMax; y++)
		{
			float e1 = _EdgeFunction(v0, v2, B3BXVECTOR2(x, y));
			float e2 = _EdgeFunction(v2, v1, B3BXVECTOR2(x, y));
			float e3 = _EdgeFunction(v1, v0, B3BXVECTOR2(x, y));
			if (e1 >= 0 && e2 >= 0 && e3 >= 0 || e1 <= 0 && e2 <= 0 && e3 <= 0)
			{
				SetPixel(m_surfaceRT, x, y, RGB(255, 255, 255));
			}
		}
	}
	return BH_OK;
}

int B3bhDevice9::_DrawFace(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXVECTOR2 v2, B3BXCOLOR c0, B3BXCOLOR c1, B3BXCOLOR c2)
{
	int xMin = min(v0.x, min(v1.x, v2.x));
	int xMax = max(v0.x, max(v1.x, v2.x));
	int yMax = max(v0.y, max(v1.y, v2.y));
	int yMin = min(v0.y, min(v1.y, v2.y));

	B3BXVECTOR2 p;
	B3BXCOLOR col;
	float area = _EdgeFunction(v0, v1, v2);
	for (int x = xMin; x < xMax; x++)
	{
		for (int y = yMin; y < yMax; y++)
		{
			p = B3BXVECTOR2(x, y);
			float e0 = _EdgeFunction(v1, v2, p) / area;
			float e1 = _EdgeFunction(v2, v0, p) / area;
			float e2 = _EdgeFunction(v0, v1, p) / area;
			if (e0 >= 0 && e1 >= 0 && e2 >= 0 )
			{
				bhLerp(&col, c0, c1, c2, e0, e1, e2);
				SetPixel(m_surfaceRT, x, y, (DWORD)col);
			}
		}
	}
	return BH_OK;
}


	
int B3bhDevice9::_EdgeFunction(B3BXVECTOR2 sp, B3BXVECTOR2 ep, B3BXVECTOR2 p)
{
	int deltaX = ep.x - sp.x;
	int deltaY = ep.y - sp.y;
	return deltaY * p.x - deltaX * p.y + deltaX * sp.y - deltaY * sp.x;
}

bool B3bhDevice9::_FaceCulling(B3BXVECTOR2 v0, B3BXVECTOR2 v1, B3BXVECTOR2 v2)
{
	bool bCull = false;

	switch (m_rState[B3BRS_CULLMODE])
	{
		case B3BCULL_NONE:
		{
			bCull = false;
		}
		break;
		case B3BCULL_CW:
		{
			B3BXVECTOR2 l = v1 - v0;
			B3BXVECTOR2 r = v2 - v0;
			float z = B3BXVec2Cross(l, r);
			if (z > 0) bCull = true;
		}
		break;
		case B3BCULL_CCW:
		{
			B3BXVECTOR2 l = v1 - v0;
			B3BXVECTOR2 r = v2 - v0;
			float z = B3BXVec2Cross(l, r);
			if (z < 0) bCull = true;
		}
		break;
	}

	return bCull;
}

void B3bhDevice9::_InitRenderState()
{
	::ZeroMemory(m_rState, sizeof(DWORD) * B3BRS_MAX_);

	m_rState[B3BRS_FILLMODE] = B3BFILL_SOLID;
	m_rState[B3BRS_CULLMODE] = B3BCULL_NONE;
}




B3BVertexBuffer9::B3BVertexBuffer9()
{
	m_pVBuffer = NULL;
	m_bLocked = FALSE;
	m_fvf = 0;
	m_sizeInBytes = 0;
	m_stride = 0;

	ZeroMemory(&m_desc, sizeof(m_desc));


#ifdef _DEBUG
	bhLog(_T("VertexBuffer 생성됨..."));
#endif

}

B3BVertexBuffer9::~B3BVertexBuffer9()
{
	SafeDelArray(m_pVBuffer);


#ifdef _DEBUG
	bhLog(_T("VertexBuffer 파괴됨..."));
#endif
}

int B3BVertexBuffer9::Lock(UINT offsetToLock, UINT sizeToLock, void** ppbData, DWORD flags)
{
	if (BH_ENABLED(m_bLocked))
	{
		bhLog(_T("이 버퍼는 이미 Lock 되어 있습니다. (%s=%x)"),bhToString(m_pVBuffer), m_pVBuffer);
		return BH_FAIL;
	}

	if (BH_INVALIED(ppbData)) return BH_FAIL;
	if (BH_INVALIED(m_pVBuffer)) return BH_FAIL;

	BYTE* pVB = (BYTE*)m_pVBuffer + offsetToLock;
	*ppbData = (void*)pVB;

	m_bLocked = TRUE;

	return BH_OK;
}

int B3BVertexBuffer9::Unlock(void)
{
	if (BH_DISABLED(m_bLocked))
	{
		bhLog(_T("이 버퍼는 이미 UnLock 되어있습니다. (%s=%x)"), bhToString(m_pVBuffer), m_pVBuffer);
	}
	m_bLocked = FALSE;

	return BH_OK;
}

int B3BVertexBuffer9::GetDesc(_out_ B3BVERTEXBUFFER_DESC* pDesc)
{
	if (BH_INVALIED(pDesc))
	{
		return BH_FAIL;
	}

	*pDesc = m_desc;

	return BH_OK;
}

int B3BVertexBuffer9::GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData)
{
	void* pVB = GetBuffer();
	UINT vtxSize = GetVertexCount();

	*ppBuffer = pVB;
	*pSizeData = vtxSize;

	return BH_OK;
}

void* B3BVertexBuffer9::GetBuffer(void)
{
	if (BH_INVALIED(m_pVBuffer))
	{
		bhLog(_T("NULL 객체를 참조하고 있습니다. %s = NULL, Size=%d, Stride=%d"),
			bhToString(m_pVBuffer), m_desc.Size, m_stride);
	}

	return m_pVBuffer;
}

UINT B3BVertexBuffer9::GetVertexCount(void)
{
	if (BH_INVALIED(m_pVBuffer))
	{
		bhLog(_T("NULL 객체를 참조하고 있습니다. %s = NULL, Size=%d, Stride=%d"),
			bhToString(m_pVBuffer), m_desc.Size, m_stride);
	}
	if (m_desc.Size <= 0 || m_stride <= 0)
	{
		bhLog(_T("미설정 객체를 참조하고 있습니다. %s = %x, Size=%d, Stride=%d"),
			bhToString(m_pVBuffer), m_pVBuffer, m_desc.Size, m_stride);
	}

	return m_desc.Size / m_stride;
}

ULONG B3BVertexBuffer9::AddRef(void)
{
	return ++m_ref;	
}
ULONG B3BVertexBuffer9::Release(void)
{
	UINT ref = --m_ref;
	if (m_ref <= 0) delete this;
	return ref;
}
int B3BVertexBuffer9::QueryInterface(BH_IID bhid, _out_ void** ppvObj)
{
	if(bhIsEqualIID(bhid, BH_IID_IBHVertexBuffer9))
	{
		*ppvObj = dynamic_cast<IBHVertexBuffer9*>(this);
		AddRef();
	}
	return BH_OK;
}

int B3BVertexBuffer9::_Create(B3BVERTEXBUFFER_DESC desc)
{
	if (BH_VALIED(m_pVBuffer)) return BH_FAIL;

	m_desc = desc;
	m_pVBuffer = static_cast<void*>(new BYTE[m_desc.Size]); assert(m_pVBuffer);
	m_fvf = m_desc.FVF;
	m_sizeInBytes = m_desc.Size;
	
	m_stride = 0;
	if (CHECK(desc.FVF, B3BFVF_XY)) m_stride += sizeof(float) * 2;
	if (CHECK(desc.FVF, B3BFVF_DIFFUSE)) m_stride += sizeof(DWORD);

	if (m_stride <= 0) return BH_FAIL;

	return BH_OK;
}

int B3BVertexBuffer9::_Create(UINT length, DWORD fvf, B3BPOOL pool)
{
	B3BVERTEXBUFFER_DESC desc = {};
	desc.FVF = fvf;
	desc.Pool = pool;
	desc.Size = length;
	desc.Format = B3BFMT_VERTEXDATA;
	desc.Usage = B3BUSAGE_WRITEONLY;
	desc.Type = B3BTYPE_VERTEXBUFFER;

	int result = _Create(desc);

	return result;
}
