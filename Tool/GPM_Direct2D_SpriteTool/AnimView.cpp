#include "AnimView.h"

//std::vector<SPRITE_INFO> CAnimView::m_vResultVec;

CAnimView::CAnimView()
	: m_vSelectedSprites{}
{
}

CAnimView::~CAnimView()
{
}

void CAnimView::Render()
{
	ID2D1HwndRenderTarget* pRT = GetRT();
	ID2D1Bitmap* pD2DBitmap = *(GetMyBitmap()->GetD2DBitmap());
	//ID2D1HwndRenderTarget* p_Back_RT = GetRT();

	pRT->BeginDraw();
	D2D1_SIZE_F rtSize = pRT->GetSize();

	// clear
	pRT->Clear(D2D1::ColorF(0x7777ff)); // violet(변형)
	//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	pRT->DrawBitmap(
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
	pRT->EndDraw();


	// ===== SpriteView로 부터 받아온 Vector 출력 =====
	m_vSelectedSprites = CSpriteView::GetSelectedSprites();

	/*for (auto it = vTemp.begin(); it != vTemp.end(); ++it)
	{
		m_vSelectedSprites.push_back(*it);
	}*/

	// ===== SpriteView의 D2D1Bitmap을 가져와서 교체한다. ======
	/*CSpriteView spriteView
	ID2D1Bitmap* pD2DBitmap = *(GetMyBitmap()->GetD2DBitmap());*/

	//for (auto it = m_vSelectedSprites.begin(); it != m_vSelectedSprites.end(); ++it)
	//{
	//	float fAlpha = 1.f;
	//	/*if (it->m_iSelected == SELECTED)
	//		fAlpha = 0.1f;*/

	//	pRT->DrawBitmap(
	//		pD2DBitmap,
	//		D2D1::RectF(
	//			it->m_ptStartPos.x,
	//			it->m_ptStartPos.y,
	//			it->m_ptStartPos.x + it->m_iWidth,
	//			it->m_ptStartPos.y + it->m_iHeight
	//		),
	//		fAlpha, // Alpha 값
	//		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
	//		D2D1::RectF(
	//			it->m_d2dRect.left,
	//			it->m_d2dRect.top,
	//			it->m_d2dRect.right,
	//			it->m_d2dRect.bottom)
	//	);
	//}

	HDC hdc = GetDC(m_hWnd);

	// ...

	ReleaseDC(m_hWnd, hdc);
}

LRESULT CAnimView::WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
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
	{
		hdc = BeginPaint(_hWnd, &ps);

		Update();
		Render();

		EndPaint(_hWnd, &ps);
	}
	break;

	case WM_LBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
	{
		InvalidateRect(m_hWnd, NULL, true);
	}
	break;

	case WM_MOUSEMOVE:
	{
		WORD x = HIWORD(_lParam);
		WORD y = LOWORD(_lParam);

		hdc = GetDC(_hWnd);

		COLORREF color = GetPixel(hdc, x, y);
		wchar_t str[100];
		
		UINT iWndHeight = (UINT)GetRT()->GetSize().height;

		wsprintf(str, L"Mouse Pos : (%.3d, %.3d)", x, y);
		TextOut(hdc, 10, iWndHeight - 50, str, wcslen(str));
		wsprintf(str, L"RGB Value : %.8x", color);
		TextOut(hdc, 10, iWndHeight - 30, str, wcslen(str));

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