#include "WICBitmap.h"

#include "D2DCore.h"
#include "MainView.h"

CWICBitmap::CWICBitmap()
	: m_pWICBitmap(nullptr)
	, m_cbBufferSize(0)
	, m_width(0)
	, m_height(0)
{
}

CWICBitmap::~CWICBitmap()
{
}

HRESULT CWICBitmap::Create(const wstring& _wsFileName, ID2D1HwndRenderTarget* _pRenderTarget)
{
	HRESULT hr = S_OK;

	m_pWICBitmap = *(CD2DCore::GetInst()->GetWICBitmap());

	return S_OK;
}

void CWICBitmap::Release()
{
	if (m_pWICBitmap)
	{
		m_pWICBitmap->Release();
		m_pWICBitmap = nullptr;
	}
}

DWORD CWICBitmap::GetPixelColor(UINT _x, UINT _y, UINT _width, UINT _height)
{
	HRESULT hr = S_OK;

	m_width = _width;
	m_height = _height;

	// 크기 초과 예외 처리
	if (_y >= _height)	return 0;
	if (_x >= _width)	return 0;

	DWORD dwResult = 0x00'00'00'00;

	IWICBitmapLock* pILock = NULL;
	WICRect rcLock = { 0, 0, m_width, m_height };

	// 5. IWICBitmap의 지정된 사각형에 대한 IWICBitmapLock을 가져온다
	if (SUCCEEDED(hr))
	{
		// Obtain a bitmap lock for exclusive write.
		// The lock is for a 10x10 rectangle starting at the top left of the bitmap.
		hr = m_pWICBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);
	}

	// 6. 이제 IWICBitmapLock 개체에 의해 잠긴 픽셀 데이터를 처리합니다.
	if (SUCCEEDED(hr))
	{
		UINT cbBufferSize = 0;
		BYTE* pv = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pILock->GetDataPointer(&cbBufferSize, &pv);
			m_cbBufferSize = cbBufferSize;
		}

		DWORD* temp = (DWORD*)pv;
		temp = temp + (_y * _width) + _x;
		dwResult = *temp;

		//SafeRelease(pILock);
		pILock->Release();

		// IWICBitmap의 잠금을 해제하려면 IWICBitmap과 연결된 
		//		모든 IWICBitmapLock 개체에서 IUnknown::Release를 호출합니다.
	}

	return dwResult;
}

void CWICBitmap::SetAlphaZero(DWORD _rgbaValue)
{
	HRESULT hr = S_OK;

	IWICBitmapLock* pILock = NULL;
	WICRect rcLock = { 0, 0, m_width, m_height };

	// 5. IWICBitmap의 지정된 사각형에 대한 IWICBitmapLock을 가져온다
	if (SUCCEEDED(hr))
	{
		// Obtain a bitmap lock for exclusive write.
		// The lock is for a 10x10 rectangle starting at the top left of the bitmap.
		hr = m_pWICBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);
	}

	// 6. 이제 IWICBitmapLock 개체에 의해 잠긴 픽셀 데이터를 처리합니다.
	if (SUCCEEDED(hr))
	{
		UINT cbBufferSize = 0;
		BYTE* pv = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pILock->GetDataPointer(&cbBufferSize, &pv);
		}

		/*DWORD* temp = (DWORD*)pv;
		temp = temp + (_y * _width) + _x;
		dwResult = *temp;*/

		/* ================================================== */
		for (UINT i = 0; i < m_cbBufferSize - 1; ++i)
		{
			BYTE* addr = pv + i;

			DWORD* temp = (DWORD*)addr;
			if (*temp == _rgbaValue)
				*temp &= 0x00ffffff;
		}
		/* ================================================== */

		//SafeRelease(pILock);
		pILock->Release();

		// IWICBitmap의 잠금을 해제하려면 IWICBitmap과 연결된 
		//		모든 IWICBitmapLock 개체에서 IUnknown::Release를 호출합니다.
	}
}

void CWICBitmap::TestWICRed(UINT _x, UINT _y)
{
	HRESULT hr = S_OK;

	IWICBitmapLock* pILock = NULL;
	WICRect rcLock = { 0, 0, m_width, m_height };

	// 5. IWICBitmap의 지정된 사각형에 대한 IWICBitmapLock을 가져온다
	if (SUCCEEDED(hr))
	{
		// Obtain a bitmap lock for exclusive write.
		// The lock is for a 10x10 rectangle starting at the top left of the bitmap.
		hr = m_pWICBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);
	}

	// 6. 이제 IWICBitmapLock 개체에 의해 잠긴 픽셀 데이터를 처리합니다.
	if (SUCCEEDED(hr))
	{
		UINT cbBufferSize = 0;
		BYTE* pv = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pILock->GetDataPointer(&cbBufferSize, &pv);
		}

		/*DWORD* temp = (DWORD*)pv;
		temp = temp + (_y * _width) + _x;
		dwResult = *temp;*/

		/* ================================================== */
		/*for (UINT i = 0; i < m_cbBufferSize - 1; ++i)
		{
			UINT posY = _x;
			UINT posY = _y * m_width;

			BYTE* addr = pv + i;

			DWORD* temp = (DWORD*)addr;
			if (*temp == _rgbaValue)
				*temp &= 0x00ffffff;
		}*/
		
		BYTE* addr = pv + (_x) + (_y * m_width);

		DWORD* temp = (DWORD*)addr;
		//if (*temp == _rgbaValue)

		*temp = 0xffff0000;

		//for (std::vector<POS>::iterator it = CMainView::m_vecPixelPos.begin(); it != CMainView::m_vecPixelPos.end(); ++it)
		//{
		//	// test용

		//	BYTE* addr = pv + _x + _y * m_width;

		//	DWORD* temp = (DWORD*)addr;
		//	//if (*temp == _rgbaValue)

		//	*temp = 0xffff0000;
		//}
		/* ================================================== */

		//SafeRelease(pILock);
		pILock->Release();

		// IWICBitmap의 잠금을 해제하려면 IWICBitmap과 연결된 
		//		모든 IWICBitmapLock 개체에서 IUnknown::Release를 호출합니다.
	}
}
