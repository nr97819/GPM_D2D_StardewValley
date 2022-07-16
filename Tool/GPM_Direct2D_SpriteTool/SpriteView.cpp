#include "SpriteView.h"

#include "App.h"

#include "MainView.h"

CSpriteView::CSpriteView()
	: m_pMyBitmap(nullptr)
	, m_slicedSpritesVec(nullptr)
{
}

CSpriteView::~CSpriteView()
{
}

//void CSpriteView::InitResource(CBitmap* _pMyBitmap, std::vector<SLICE_RECT_POS>* _slicedSpritesVec)
//{
//	// ���⿡ Slice�� Sprite�� ���� Resources�� ������ �ڵ� �ۼ�
//	// ...
//
//	m_pMyBitmap = _pMyBitmap;
//	m_slicedSpritesVec = _slicedSpritesVec;
//	
//}

void CSpriteView::Update()
{

}

void CSpriteView::Render()
{
	ID2D1HwndRenderTarget* pRT = GetRT();
	ID2D1Bitmap* pD2DBitmap = *(GetMyBitmap()->GetD2DBitmap());
	//ID2D1HwndRenderTarget* p_Back_RT = GetRT();

	pRT->BeginDraw();
	//D2D1_SIZE_F rtSize = pRT->GetSize();

	// clear
	pRT->Clear(D2D1::ColorF(0x7777ff)); // violet(����)
	//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// ��Ȱ�� �ؼ� ���� �� ...
	POINT ptStartDrawPos = { 0, 0 };
	UINT iMaxWidth = 600;
	UINT iMaxHeight = 0;

	for (std::vector<SLICE_RECT_POS>::iterator it = m_vSlicedPos.begin(); it != m_vSlicedPos.end(); ++it)
	{
		UINT width = it->_ptDragRightBottom.x - it->_ptDragLeftTop.x;
		UINT height = it->_ptDragRightBottom.y - it->_ptDragLeftTop.y;

		// ũ�� Ȯ��
		/*width *= 1.5;
		height *= 1.5;*/

		// =========== StartPosY�� ���� Exception ===========
		if (iMaxHeight < height)
		{
			iMaxHeight = height;
		}

		// =========== StartPosY�� ���� Exception ===========
		if (ptStartDrawPos.x > iMaxWidth)
		{
			ptStartDrawPos.x = 0;
			ptStartDrawPos.y += iMaxHeight + 10;

			iMaxHeight = -1;
		}

		pRT->DrawBitmap(
			pD2DBitmap,
			D2D1::RectF(
				ptStartDrawPos.x,
				ptStartDrawPos.y,
				ptStartDrawPos.x + width, // �ӽ� ũ�� Ȯ��
				ptStartDrawPos.y + height // �ӽ� ũ�� Ȯ��
			),
			1.0f, // Alpha ��
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				it->_ptDragLeftTop.x,
				it->_ptDragLeftTop.y,
				it->_ptDragRightBottom.x,
				it->_ptDragRightBottom.y
			)
		);

		//ptStartDrawPos.y += it->_ptDragRightBottom.y;

		D2D1_RECT_F d2d_rectangle = D2D1::RectF(
			ptStartDrawPos.x,
			ptStartDrawPos.y,
			ptStartDrawPos.x + width, // �ӽ� ũ�� Ȯ��
			ptStartDrawPos.y + height // �ӽ� ũ�� Ȯ��
		);
		pRT->DrawRectangle(d2d_rectangle, m_pD2D1RedBrush);

		UINT padding = 0; // ���ʿ�
		ptStartDrawPos.x += width + padding;
	}

	// ======================================================
	// --------------- Auto Slice ���� ��� ------------------
	// ======================================================
	std::vector<D2D1_RECT_F> rects = CMainView::GetSlicedRects();

	for (std::vector<D2D1_RECT_F>::iterator it = rects.begin(); it != rects.end(); ++it)
	{
		UINT width = it->right - it->left;
		UINT height = it->top - it->bottom;

		// ũ�� Ȯ��
		/*width *= 1.5;
		height *= 1.5;*/

		// =========== StartPosY�� ���� Exception ===========
		if (iMaxHeight < height)
		{
			iMaxHeight = height;
		}

		// =========== StartPosY�� ���� Exception ===========
		if (ptStartDrawPos.x > iMaxWidth)
		{
			ptStartDrawPos.x = 0;
			ptStartDrawPos.y += iMaxHeight;

			iMaxHeight = 0;
		}

		pRT->DrawBitmap(
			pD2DBitmap,
			D2D1::RectF(
				ptStartDrawPos.x,
				ptStartDrawPos.y,
				ptStartDrawPos.x + width, // �ӽ� ũ�� Ȯ��
				ptStartDrawPos.y + height // �ӽ� ũ�� Ȯ��
			),
			1.0f, // Alpha ��
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				it->left,
				it->top,
				it->right,
				it->bottom
			)
		);

		//ptStartDrawPos.y += it->_ptDragRightBottom.y;

		D2D1_RECT_F d2d_rectangle = D2D1::RectF(
			ptStartDrawPos.x,
			ptStartDrawPos.y,
			ptStartDrawPos.x + width, // �ӽ� ũ�� Ȯ��
			ptStartDrawPos.y + height // �ӽ� ũ�� Ȯ��
		);
		pRT->DrawRectangle(d2d_rectangle, m_pD2D1RedBrush);

		UINT padding = 0; // ���ʿ�
		ptStartDrawPos.x += width + padding;
	}

	pRT->EndDraw();

	// =========================================================

	HDC hdc = GetDC(m_hWnd);

	ReleaseDC(m_hWnd, hdc);
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
	{
		hdc = BeginPaint(_hWnd, &ps);

		Update(); // Main View�� Sprite�� ����ȭ
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
