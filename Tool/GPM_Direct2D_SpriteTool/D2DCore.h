#pragma once

// ======== Direct2D ========
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
// ==========================

#include <Windows.h>
#include <string>
using std::wstring;

#include <stdexcept>
#include <limits>

template<typename T>
UINT SafeRelease(T*& pUnk)
{
	UINT ret = 0;
	::IUnknown* pUnknown = nullptr;

	if (pUnk == nullptr)
	{
		return 0;
	}

	pUnknown = dynamic_cast<::IUnknown*>(pUnk);

	if (pUnknown == nullptr)
	{
		//throw std::InvalidAccessException(L"Is not derived from IUnknown");
		throw std::invalid_argument("Is not derived from IUnknown");
	}

	ret = pUnknown->Release();
	pUnknown = nullptr;

	return ret;
}

class CD2DCore
{
private:
	ID2D1Factory*				m_pD2D1Factory;
	IDWriteFactory*				m_pDWriteFactory;
	IWICImagingFactory*			m_pIWICFactory;

	IWICBitmap*					m_pIWICBitmap;
	//IDWriteTextFormat*		m_pDWriteTextFormat;

public:
	static CD2DCore* GetInst()
	{
		static CD2DCore* m_inst = new CD2DCore;
		return m_inst;
	}

private:
	CD2DCore();
	~CD2DCore();

public:
	HRESULT InitFactory();
	HRESULT InitRenderTarget(HWND _hWnd, ID2D1HwndRenderTarget** _pRenderTarget);
	void Release();

public:
	ID2D1Factory* GetD2D1Factory() { return m_pD2D1Factory; }
	IDWriteFactory* GetDWriteFactory() { return m_pDWriteFactory; }
	IWICImagingFactory* GetWICImagingFactory() { return m_pIWICFactory; }

	HRESULT CreateD2D1Bitmap(const wstring& _wsFileName, ID2D1HwndRenderTarget* _pRenderTarget, ID2D1Bitmap** _pD2D1Bitmap);
	//IDWriteTextFormat** CreateMyTextFormat(const WCHAR* _fontName, FLOAT _fontSize);

	IWICBitmap** GetWICBitmap() { return &m_pIWICBitmap; }

public:
	void Test()
	{
		// 1. IWICImagingFactory ��ü�� ����� WIC(Windows Imaging Component) ��ü�� ����ϴ�.
		// (D2DCore->Init()���� �̹� ����)

		// 2. CreateDecoderFromFilename �޼��带 ����Ͽ� �̹��� ���Ͽ��� IWICBitmapDecoder�� �����
		HRESULT hr = S_OK;

		IWICBitmapDecoder* pIDecoder = NULL;
		IWICBitmapFrameDecode* pIDecoderFrame = NULL;

		// Make "Decoder" from "Image File"
		hr = m_pIWICFactory->CreateDecoderFromFilename(
			L"images\\woman.png",					// Image to be decoded
			NULL,							// Do not prefer a particular vendor
			GENERIC_READ,					// Desired read access to the file
			WICDecodeMetadataCacheOnDemand,	// Cache metadata when needed
			&pIDecoder						// Pointer to the decoder
		);

		// 3. �̹����� ù ��° IWICBitmapFrameDecode �� �����´�.
		if (SUCCEEDED(hr))
		{
			hr = pIDecoder->GetFrame(0, &pIDecoderFrame);
		}

		IWICBitmap* pIBitmap = NULL;
		IWICBitmapLock* pILock = NULL;

		int uiWidth = 10;
		int uiHeight = 10;

		WICRect rcLock = { 0, 0, uiWidth, uiHeight };

		// 4. ������ ������ �̹��� �����ӿ��� IWICBitmap �� �����
		if (SUCCEEDED(hr))
		{
			hr = m_pIWICFactory->CreateBitmapFromSource(
				pIDecoderFrame,			// Create a bitmap from the image frame
				WICBitmapCacheOnDemand,	// Cache bitmap pixels on first access
				&pIBitmap);				// Pointer to the bitmap
		}

		// 5. IWICBitmap�� ������ �簢���� ���� IWICBitmapLock�� �����´�
		if (SUCCEEDED(hr))
		{
			// Obtain a bitmap lock for exclusive write.
			// The lock is for a 10x10 rectangle starting at the top left of the bitmap.
			hr = pIBitmap->Lock(&rcLock, WICBitmapLockWrite, &pILock);
		}

		// 6. ���� IWICBitmapLock ��ü�� ���� ��� �ȼ� �����͸� ó���մϴ�.
		if (SUCCEEDED(hr))
		{
			UINT cbBufferSize = 0;
			BYTE* pv = NULL;

			if (SUCCEEDED(hr))
			{
				hr = pILock->GetDataPointer(&cbBufferSize, &pv);
			}

			// Pixel manipulation using the image data pointer pv.
			// ...
			// Release the bitmap lock.
			
			//SafeRelease(pILock);
			pILock->Release();

			// IWICBitmap�� ����� �����Ϸ��� IWICBitmap�� ����� 
			//		��� IWICBitmapLock ��ü���� IUnknown::Release�� ȣ���մϴ�.
		}

		// 7. ���� ��ü�� �����մϴ�.
		::SafeRelease(pIBitmap);
		::SafeRelease(pIDecoder);
		::SafeRelease(pIDecoderFrame);
		/*pIBitmap->Release();
		pIDecoder->Release();
		pIDecoderFrame->Release();*/
	}

	HRESULT CreateD2D1BitampFromWICBitmap(ID2D1HwndRenderTarget* _pRT, IWICBitmap* _iWICBitmap, ID2D1Bitmap** _iD2D1bitmap);

	/*HWND GetMainHwnd() { return m_hMainWnd; }
	ID2D1HwndRenderTarget* GetMainRT() { return m_pMainRT; }
	ID2D1Bitmap* GetMainD2D1Bitmap() { return m_pD2DMainBitmap; }*/
	
	/*
	HWND GetToolHwnd() { return m_hToolWnd; }
	ID2D1HwndRenderTarget* GetToolRT() { return m_pToolRT; }
	ID2D1Bitmap* GetToolD2D1Bitmap() { return m_pD2DToolBitmap; }*/
};