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
	CWICBitmap(IWICBitmap* _wicb, UINT _cbSize, UINT _w, UINT _h);
	~CWICBitmap();

public:
	HRESULT Create(const wstring& _wsFileName, ID2D1HwndRenderTarget* _pRenderTarget);
	void Release();
	CWICBitmap* Clone()
	{
		CWICBitmap* clone = new CWICBitmap(this->m_pWICBitmap, this->m_cbBufferSize, this->m_width, this->m_height);
		return clone;
	}

public:
	IWICBitmap* GetWICBitmap() { return m_pWICBitmap; }
	UINT GetCBufferSize() { return m_cbBufferSize; }

public:
	DWORD GetPixelColor(UINT _x, UINT _y, UINT _width, UINT _height);
	UINT GetWidth() { return m_width; }
	UINT GetHeight() { return m_height; } // ¾È ¾¸
	
	void SetAlphaZero(DWORD _rgbaValue);

	void SetSpecificPosAlphaZero(UINT _x, UINT _y);
};

