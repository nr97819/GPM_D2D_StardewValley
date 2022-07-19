#include "AnimView.h"

#include "TimeMgr.h"

//std::vector<SPRITE_INFO> CAnimView::m_vResultVec;

CAnimView::CAnimView()
	: m_vSelectedSprites{}
	, m_vAnimVec{}
	, m_pSprieViewBitmap(nullptr)
	, m_spriteViewD2DBitmap(nullptr)
{
}

CAnimView::~CAnimView()
{
	m_pSprieViewBitmap->Release();
}

void CAnimView::Init()
{
	CTimeMgr::GetInst()->Init();
}

void CAnimView::Update()
{
	CTimeMgr::GetInst()->Update();
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

	// ===== SpriteView로 부터 받아온 Vector 출력 =====

	m_vSelectedSprites = CSpriteView::GetSelectedSprites();

	//wstring wsImagePath = CSpriteView::GetImagePath();

	wstring wsImagePath = L"images\\woman.png";
	m_pSprieViewBitmap = new CBitmap();
	m_pSprieViewBitmap->Create(wsImagePath, GetRT());
	m_spriteViewD2DBitmap = *(m_pSprieViewBitmap->GetD2DBitmap());


	// ===== SpriteView로 부터 받아온 Vector 출력 =====
	float fAlpha = 1.f;
	//static auto it = m_vSelectedSprites.begin();

	for (auto it = m_vSelectedSprites.begin(); it != m_vSelectedSprites.end(); ++it)
	{
		pRT->DrawBitmap(
			m_spriteViewD2DBitmap,
			D2D1::RectF(
				50.f, // 대충, 배경이랑 어울리는 위치 잡아만 놓은 것..
				300.f,
				50.f + it->m_iWidth,
				300.f + it->m_iHeight
			),
			fAlpha, // Alpha 값
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				it->m_d2dRect.left,
				it->m_d2dRect.top,
				it->m_d2dRect.right,
				it->m_d2dRect.bottom)
		);
	}

	/*if (CTimeMgr::GetInst()->GetfDT() >= 0.1f)
	{
		if (it != m_vSelectedSprites.end())
		{
			++it;
		}
		else
		{
			it = m_vSelectedSprites.begin();
		}
	}*/



	pRT->EndDraw();

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

void CAnimView::OnMouseUp()
{
}

LRESULT CAnimView::WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	int width = 0;
	int height = 0;

	// TimeMgr : 수시로 새로고침
	static double dAcc = 0.;
	Update();
	dAcc += fDT;
	if (dAcc >= 0.1)
	{
		InvalidateRect(m_hWnd, NULL, true);
		dAcc = 0.;
	}
	// ...

	// =====================================

	switch (_message)
	{
	case WM_CREATE:
	{
		Init();
	}
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(_hWnd, &ps);

		Render();

		EndPaint(_hWnd, &ps);
	}
	break;

	case WM_LBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
	{
		// MouseUp 될 때마다, bitmap 새로고침
		OnMouseUp();

		InvalidateRect(m_hWnd, NULL, false);
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