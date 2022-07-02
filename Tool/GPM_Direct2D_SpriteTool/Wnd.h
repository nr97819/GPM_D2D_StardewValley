#pragma once

#include <Windows.h>

#include "Bitmap.h"
#include "resource1.h"

#include "WICBitmap.h"

struct SLICE_RECT_POS
{
	POINT _ptDragLeftTop = {};
	POINT _ptDragRightBottom = {};
};

typedef LRESULT (CALLBACK *PROC_FPTR)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT (*PROC_MSG_FPTR)(HWND, UINT, WPARAM, LPARAM);

class CWnd
{
protected:
	HINSTANCE				m_hInstance;
	int						m_nCmdShow;

	wstring					m_wClassName = {};
	wstring					m_wTitleName = {};

	HWND					m_hWnd;

	// Front RT
	ID2D1HwndRenderTarget*	m_pRenderTarget;
	CBitmap*				m_pMyBitmap;
	CWICBitmap*				m_pMyWICBitmap;

	// Back RT
	/*ID2D1HwndRenderTarget*	m_pBack_RenderTarget;
	CBitmap*				m_pBack_MyBitmap;
	CWICBitmap*				m_pBack_MyWICBitmap;*/

	ID2D1SolidColorBrush*	m_pD2D1RedBrush = NULL;

public:
	CWnd();
	virtual ~CWnd();

public:
	HWND Create(HINSTANCE _hInstance, int _nCmdShow, 
		const wstring& _wsClassName, const wstring& _wsTitleName, PROC_FPTR _fpProc, bool _bMenu);
	HRESULT InitTarget();
	HRESULT InitBitmap(const wstring& _wsImageFileName);
	void _ShowWindow();
	void Adjust(const POINT& _ptResolution, const POINT& _ptPos);

	void InitRedBursh()
	{
		m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f)),
			&m_pD2D1RedBrush);
	}

public:
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	// 자식 Wnd에서 구현
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) = 0;

public:
	void Update();
	virtual void Render();

public:
	void Release();

public:
	CBitmap* GetMyBitmap() { return m_pMyBitmap; }
	HWND GetHwnd() { return m_hWnd; }
	ID2D1HwndRenderTarget* GetRT() { return m_pRenderTarget; }
	//ID2D1HwndRenderTarget* GetBackRT() { return m_pBack_RenderTarget; }
};

