#include "SpriteView.h"

#include "App.h"
#include "ToolView.h"

CSpriteView::CSpriteView()
	: m_pMyBitmap(nullptr)
{
}

CSpriteView::~CSpriteView()
{
}

void CSpriteView::InitResource(CBitmap* _pMyBitmap, std::vector<SLICE_RECT_POS>* _slicedSpritesVec)
{
	// 여기에 Slice된 Sprite에 대한 Resources를 가져올 코드 작성
	// ...

	m_pMyBitmap = _pMyBitmap;
	m_slicedSpritesVec = _slicedSpritesVec;
	
}

void CSpriteView::Render()
{
	ID2D1HwndRenderTarget* pRenderTarget = GetRT();
	//ID2D1Bitmap* pD2DBitmap = *(GetMyBitmap()->GetD2DBitmap());

	pRenderTarget->BeginDraw();
	D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

	// clear
	pRenderTarget->Clear(D2D1::ColorF(0xff00ff)); // violet
	//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	//pRenderTarget->DrawBitmap(
	//	pD2DBitmap,
	//	D2D1::RectF(
	//		0.f, 0.f,
	//		0.f + pD2DBitmap->GetSize().width,
	//		0.f + pD2DBitmap->GetSize().height
	//	),
	//	1.0f, // Alpha 값
	//	D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
	//	D2D1::RectF(
	//		0.f, 0.f,
	//		pD2DBitmap->GetSize().width,
	//		pD2DBitmap->GetSize().height
	//	)
	//);



	// 여기에 Slice된 Sprite들 출력하는 코드 작성
	// ...


	ID2D1Bitmap* pD2DBitmap = *(m_pMyBitmap->GetD2DBitmap());

	pRenderTarget->DrawBitmap(
		pD2DBitmap,
		D2D1::RectF(
			0.f, 0.f,
			0.f + pD2DBitmap->GetSize().width,
			0.f + pD2DBitmap->GetSize().height
		),
		1.0f, // Alpha 값
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(
			0.f, 0.f,
			pD2DBitmap->GetSize().width,
			pD2DBitmap->GetSize().height
		)
	);

	pRenderTarget->EndDraw();
}

LRESULT CSpriteView::WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	int width = 0;
	int height = 0;

	switch (_message)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		hdc = BeginPaint(_hWnd, &ps);
		EndPaint(_hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
	{
		// D2D1
		Update();
		Render();
	}
	case WM_MOUSEMOVE:
	{
		WORD x = HIWORD(_lParam);
		WORD y = LOWORD(_lParam);

		hdc = GetDC(_hWnd);

		COLORREF color = GetPixel(hdc, x, y);
		wchar_t str[100];
		wsprintf(str, L"Mouse Pos : (%.3d, %.3d)", x, y);
		TextOut(hdc, 10, 10, str, wcslen(str));
		wsprintf(str, L"RGB Value : %.8x", color);
		TextOut(hdc, 10, 30, str, wcslen(str));

		ReleaseDC(_hWnd, hdc);
	}
	break;

	case WM_SIZE:
	{
		width = LOWORD(_lParam);
		height = HIWORD(_lParam);

		if (m_pRenderTarget)
			m_pRenderTarget->Resize({ (UINT32)width, (UINT32)height });
	}
	break;

	case WM_COMMAND:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		break;
	}
	return 0;
}
