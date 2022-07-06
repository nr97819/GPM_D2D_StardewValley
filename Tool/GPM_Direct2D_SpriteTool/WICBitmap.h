#pragma once

// ======== Direct2D ========
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
// ==========================

#include <string>
using std::wstring;

class CWICBitmap
{
private:
	IWICBitmap*		m_pWICBitmap;

	UINT			m_cbBufferSize;

	UINT			m_width;
	UINT			m_height;

public:
	CWICBitmap();
	~CWICBitmap();

public:
	HRESULT Create(const wstring& _wsFileName, ID2D1HwndRenderTarget* _pRenderTarget);
	void Release();

public:
	IWICBitmap* GetWICBitmap() { return m_pWICBitmap; }

public:
	DWORD GetPixelColor(UINT _x, UINT _y, UINT _width, UINT _height);
	
	void SetAlphaZero(DWORD _rgbaValue);

	void TestWICRed(UINT _x, UINT _y);
};

