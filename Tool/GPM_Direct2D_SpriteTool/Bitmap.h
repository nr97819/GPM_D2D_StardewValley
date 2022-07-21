#pragma once

// ======== Direct2D ========
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
// ==========================

#include <string>
using std::wstring;

#include "Res.h"

class CBitmap : public CRes
{
private:
	ID2D1Bitmap* m_pD2DBitmap;

public:
	CBitmap();
	~CBitmap();

public:
	HRESULT Create(const wstring& _wsFileName, ID2D1HwndRenderTarget* _pRenderTarget);
	void Release();

public:
	ID2D1Bitmap** GetD2DBitmap() { return &m_pD2DBitmap; }
	void SetD2D1Bitmap(ID2D1Bitmap** _pD2D1Bitmap) { m_pD2DBitmap = *_pD2D1Bitmap; }
};

