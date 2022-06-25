#include "ToolView.h"

#include "D2DCore.h"

CToolView::CToolView()
	: m_alphaZeroColor(0x0)
	, m_bSetAlphaZeroState(false)
	, m_bDragSliceState(false)

	, m_bIsLButtonDown(false)
{
}

CToolView::~CToolView()
{
}

LRESULT CToolView::WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (_message)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		hdc = BeginPaint(_hWnd, &ps);
		EndPaint(_hWnd, &ps);
		break;

	case WM_MOUSEMOVE:
	{
		// D2D1
		Update();
		Render();

		// =====================================================================

		WORD x = LOWORD(_lParam);
		WORD y = HIWORD(_lParam);
		wchar_t str[100];

		hdc = GetDC(_hWnd);

		// 마우스 위치의 pixel값 스포이드
		//COLORREF color = GetPixel(hdc, x, y);
		ID2D1Bitmap* d2d1_temp = *(m_pMyBitmap->GetD2DBitmap());
		UINT width = d2d1_temp->GetSize().width;
		UINT height = d2d1_temp->GetSize().height;

		DWORD hex_color = m_pMyWICBitmap->GetPixelColor(x, y, width, height);


		// 멤버변수에 보관
		m_alphaZeroColor = hex_color;


		BYTE a_value = (hex_color & 0xff000000) >> (8 * 3);
		BYTE r_value = (hex_color & 0x00ff0000) >> (8 * 2);
		BYTE g_value = (hex_color & 0x0000ff00) >> (8 * 1);
		BYTE b_value = (hex_color & 0x000000ff) >> (8 * 0);
		hex_color = (hex_color | b_value) << (8);
		hex_color = (hex_color | g_value) << (8);
		hex_color = (hex_color | r_value) << (8);
		hex_color = (hex_color | a_value);

		wsprintf(str, L"(%.3d, %.3d) COLOR : %.8x", x, y, hex_color);
		TextOut(hdc, 10, 10, str, wcslen(str));

		HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(0xff, 0xff, 0xff));
		HPEN hOldPen = (HPEN)SelectObject(hdc, hNewPen);

		HBRUSH hNewBrush = CreateSolidBrush(RGB(r_value, g_value, b_value));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);

		POINT rtSize = { 50, 50 };
		Rectangle(hdc, 900, 20, 900 + rtSize.x, 20 + rtSize.y);

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);

		DeleteObject(hNewPen);
		DeleteObject(hNewBrush);
		
		// ==========================================================
		HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(0xff, 0x00, 0x00));
		hOldPen = (HPEN)SelectObject(hdc, hRedPen);

		// 시스템 함수 (delete 안해줘도 됨)
		SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

		/*HBRUSH hRedBrush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
		hOldBrush = (HBRUSH)SelectObject(hdc, hRedBrush);*/
		
		if(m_bDragSliceState && m_bIsLButtonDown)
		{
			Rectangle(hdc,
				ptTempLT.x,
				ptTempLT.y,
				x,
				y);
		}

		for (std::vector<SLICE_RECT_POS>::iterator it = m_vSlicePos.begin(); it != m_vSlicePos.end(); ++it)
		{
			Rectangle(hdc, 
				it->_ptDragLeftTop.x,
				it->_ptDragLeftTop.y,
				it->_ptDragRightBottom.x,
				it->_ptDragRightBottom.y);
		}

		/*SelectObject(hdc, hOldBrush);
		DeleteObject(hNewPen);*/
		SelectObject(hdc, hOldPen);
		DeleteObject(hRedPen);
		// ==========================================================

		ReleaseDC(_hWnd, hdc);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		m_bIsLButtonDown = true;

		WORD x = LOWORD(_lParam);
		WORD y = HIWORD(_lParam);

		if (m_bSetAlphaZeroState)
		{
			m_pMyWICBitmap->SetAlphaZero(m_alphaZeroColor);

			CD2DCore::GetInst()->CreateD2D1BitampFromWICBitmap(
				m_pRenderTarget,
				m_pMyWICBitmap->GetWICBitmap(),
				m_pMyBitmap->GetD2DBitmap());

			m_bSetAlphaZeroState = false;
		}

		if (m_bDragSliceState)
		{
			ptTempLT = { x, y };
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		m_bIsLButtonDown = false;

		ID2D1Bitmap* d2d1_temp = *(m_pMyBitmap->GetD2DBitmap());
		UINT width = d2d1_temp->GetSize().width;
		UINT height = d2d1_temp->GetSize().height;

		WORD pos_X = LOWORD(_lParam);
		WORD pos_Y = HIWORD(_lParam);
		ptTempRB = { pos_X, pos_Y };

		SLICE_RECT_POS srp = {};

		if (m_bDragSliceState)
		{
			srp._ptDragLeftTop = { ptTempLT.x, ptTempLT.y };
			srp._ptDragRightBottom = { ptTempRB.x, ptTempRB.y };
			//m_vSlicePos.push_back(srp);
			m_vSlicePos.push_back(srp);
		}

		//if (m_bDragSliceState)
		//{
		//	UINT max_X = 0;
		//	UINT max_Y = 0;
		//	UINT min_X = INFINITE;
		//	UINT min_Y = INFINITE;

		//	for (UINT y = ptTempLT.y; y < ptTempRB.y; ++y)
		//	{
		//		for (UINT x = ptTempLT.x; x < ptTempRB.x; ++x)
		//		{
		//			DWORD hex_color = m_pMyWICBitmap->GetPixelColor(x, y, width, height);
		//			BYTE alpha_value = (hex_color & 0xff000000) >> (8 * 3);

		//			if (alpha_value != 0xff)
		//				break;

		//			if (x < 0 || y < 0) //  || x > width || y > height
		//				break;

		//			if (x >= ptTempLT.x && y >= ptTempLT.y && x <= ptTempRB.x && y <= ptTempRB.y)
		//			{
		//				if (x > max_X)
		//					max_X = x;

		//				if (y > max_Y)
		//					max_Y = y;

		//				if (x < min_X)
		//					min_X = x;

		//				if (y < min_Y)
		//					min_Y = y;
		//			}
		//		}
		//	}

		//	ptTempLT.x = min_X;
		//	ptTempLT.y = min_Y;

		//	ptTempRB.x = max_X;
		//	ptTempRB.y = max_Y;

		//	srp._ptDragLeftTop = { ptTempLT.x, ptTempLT.y };
		//	srp._ptDragRightBottom = { ptTempRB.x, ptTempRB.y };
		//	m_vSlicePos.push_back(srp);
		//}

		/*ptTempLT = { 0, 0 };
		ptTempRB = { 0, 0 };*/
	}
	break;

	case WM_SIZE:
	{
		UINT width = LOWORD(_lParam);
		UINT height = HIWORD(_lParam);

		if (m_pRenderTarget)
			m_pRenderTarget->Resize({ (UINT32)width, (UINT32)height });
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(_wParam))
		{
		case ID_FILE_LOAD:
		{
			WCHAR wsFileName[32] = {};
			OPENFILENAME ofn; // = {};
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFilter = L"All Files(*.*)\0*.*\0";
			ofn.lpstrFile = wsFileName;
			ofn.nMaxFile = sizeof(wsFileName);;
			GetOpenFileName(&ofn);

			if ('\0' != wsFileName[0])
				MessageBox(m_hWnd, L"Successfully Load File", L"Load File", MB_OK);
		}
		break;

		case ID_FILE_SAVE:
		{
			MessageBox(m_hWnd, L"File Save", L"File Save Event", MB_OKCANCEL);
		}
		break;
		/*case ID_FILE_MENU2:
			MessageBox(m_hWnd, L"MENU1", L"TEST MENU1", MB_OK);
			break;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		*/
		case ID_SPRITE_DRAGSLICE:
		{
			m_bDragSliceState = true;
		}
		break;

		case ID_COLOR_SETALPHAZERO:
		{
			m_bSetAlphaZeroState = true;
		}
		break;
		}
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