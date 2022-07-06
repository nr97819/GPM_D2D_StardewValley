#include "MainView.h"

#include "D2DCore.h"

std::vector<POS> CMainView::m_vecPixelPos = {};

CMainView::CMainView()
	: m_alphaZeroColor(0x0)
	, m_bSetAlphaZeroState(false)
	
	, m_bDragSliceState(false)
	, m_bAutoSliceState(false)

	, m_bIsLButtonDown(false)

	, m_bIsInvalidSlicedRect(false)
{
}

CMainView::~CMainView()
{
}

void CMainView::OnCreate()
{
	//this->InitRedBursh();
}

void CMainView::OnMouseMove(LPARAM _lParam)
{
	HDC hdc = GetDC(m_hWnd);

	WORD x = LOWORD(_lParam);
	WORD y = HIWORD(_lParam);

	wchar_t str[100];
	ptCurMousePos = { x, y };

	// 마우스 위치의 pixel값 스포이드
	//COLORREF color = GetPixel(hdc, x, y);
	ID2D1Bitmap* d2d1_temp = *(m_pMyBitmap->GetD2DBitmap());
	UINT width = (UINT)d2d1_temp->GetSize().width;
	UINT height = (UINT)d2d1_temp->GetSize().height;

	DWORD hex_color = m_pMyWICBitmap->GetPixelColor(ptCurMousePos.x, ptCurMousePos.y, width, height);

	// 멤버변수에 보관
	m_alphaZeroColor = hex_color;

	BYTE a_value = static_cast<BYTE>((hex_color & 0xff000000) >> (8 * 3));
	BYTE r_value = static_cast<BYTE>((hex_color & 0x00ff0000) >> (8 * 2));
	BYTE g_value = static_cast<BYTE>((hex_color & 0x0000ff00) >> (8 * 1));
	BYTE b_value = static_cast<BYTE>((hex_color & 0x000000ff) >> (8 * 0));
	hex_color = (hex_color | b_value) << (8);
	hex_color = (hex_color | g_value) << (8);
	hex_color = (hex_color | r_value) << (8);
	hex_color = (hex_color | a_value);

	//UINT iWndWidth = (UINT)GetRT()->GetSize().width;
	UINT iWndHeight = (UINT)GetRT()->GetSize().height;

	wsprintf(str, L"Mouse Pos : (%.3d, %.3d)", ptCurMousePos.x, ptCurMousePos.y);
	TextOut(hdc, 10, iWndHeight - 50, str, wcslen(str));
	wsprintf(str, L"RGB Value : %.8x", hex_color);
	TextOut(hdc, 10, iWndHeight - 30, str, wcslen(str));

	HPEN hWhitePen = CreatePen(PS_SOLID, 1, RGB(0xff, 0xff, 0xff));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hWhitePen);

	HBRUSH hAlphaBrush = CreateSolidBrush(RGB(r_value, g_value, b_value));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hAlphaBrush);

	// rectSize
	Rectangle(hdc, 900, 20, 900 + 50, 20 + 50);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hWhitePen);
	DeleteObject(hAlphaBrush);

	ReleaseDC(m_hWnd, hdc);
}

void CMainView::OnMouseDown(LPARAM _lParam)
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

void CMainView::DrawSlicedSprite()
{
	HDC hdc = GetDC(m_hWnd);

	HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(0xff, 0x00, 0x00));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hRedPen);

	//HBRUSH hRedBrush = CreateSolidBrush(RGB(0xff, 0x00, 0x00)); // Red 브러시
	//hOldBrush = (HBRUSH)SelectObject(hdc, hRedBrush);

	// 시스템 함수 (delete 안해줘도 됨)
	SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

	if (m_bDragSliceState)
	{
		if (m_bIsLButtonDown)
		{
			Rectangle(hdc,
				ptTempLT.x,
				ptTempLT.y,
				ptCurMousePos.x,
				ptCurMousePos.y);
		}

		for (std::vector<SLICE_RECT_POS>::iterator it = m_vSlicedPos.begin(); it != m_vSlicedPos.end(); ++it)
		{
			Rectangle(hdc,
				it->_ptDragLeftTop.x,
				it->_ptDragLeftTop.y,
				it->_ptDragRightBottom.x,
				it->_ptDragRightBottom.y);
		}
	}
	else if (m_bAutoSliceState)
	{
		// 중복 처리 안하도록 추후 수정 (임시 함수)
		AutoSlice();

		for (std::vector<POS>::iterator it = m_vecPixelPos.begin(); it != m_vecPixelPos.end(); ++it)
		{
			// test용
			
			m_pMyWICBitmap->TestWICRed(it->m_x, it->m_y);

			/*Rectangle(hdc,
				it->m_x,
				it->m_y,
				it->m_x + 50,
				it->m_y + 50
			);*/
		}

		CD2DCore::GetInst()->CreateD2D1BitampFromWICBitmap(
			m_pRenderTarget,
			m_pMyWICBitmap->GetWICBitmap(),
			m_pMyBitmap->GetD2DBitmap());
	}
	else
	{
		// ...
	}

	/*SelectObject(hdc, hOldBrush);*/
	SelectObject(hdc, hOldPen);
	DeleteObject(hRedPen);

	ReleaseDC(m_hWnd, hdc);
}

void CMainView::AutoSlice()
{
	ID2D1Bitmap* d2d1_temp = *(m_pMyBitmap->GetD2DBitmap());

	UINT width = (UINT)d2d1_temp->GetSize().width;
	UINT height = (UINT)d2d1_temp->GetSize().height;


	POS prevPos{};
	m_vecPixelPos.clear();

	// step 1 : 행을 순차적으로 읽다가, alpha가 0x0보다 큰 좌표를 찾으면 탈출

	bool bEscapeLoop = false;
	
	for (UINT posY = 0; posY < height; ++posY)
	{
		for (UINT posX = 0; posX < width; ++posX)
		{
			DWORD hex_color 
				= m_pMyWICBitmap->GetPixelColor(posX, posY, width, height);

			BYTE a_value = static_cast<BYTE>((hex_color & 0xff000000) >> (8 * 3));
			
			// 조금이라도 Alpha 값이 있는 pixel이라면,
			if (a_value > 0x0)
			{
				m_vecPixelPos.push_back(POS(posX, posY));
				prevPos = POS(posX, posY);


				bEscapeLoop = true;
				break;
			}
		}

		if (bEscapeLoop)
			break;
	}

	// step 2 : 테두리 읽기 시작

	UINT iDir = (UINT)DIR::RIGHT; // 처음에는 특수하게 RIGHT로 시작하면 됨
	
	//test
	static int cnt = 0;


	while (true)
	{
		/*switch (iDir)
		{
		case (UINT)DIR::UP:
		{
			newPos = GET_DIR(iDir);
		}
		break;

		case (UINT)DIR::RIGHT:
		{
			newPos = GET_DIR(iDir);
		}
		break;

		case (UINT)DIR::DOWN:
		{

		}
		break;

		case (UINT)DIR::LEFT:
		{

		}
		break;
		}*/


		POS newPos = {};
		newPos = GET_DIR(iDir);


		DWORD hex_color
			= m_pMyWICBitmap->GetPixelColor(newPos.m_x, newPos.m_y, width, height);

		BYTE a_value = static_cast<BYTE>((hex_color & 0xff000000) >> (8 * 3));

		std::vector<POS>::iterator it = m_vecPixelPos.end();
		if (newPos == *(it - 1))
		{
			break; // 종료한다. (아직 구현 못 함)
		}
		
		
		// ===== 임시 중단 코드 ======
		if (cnt > 100000)
		{
			break;; // 종료한다. (아직 구현 못 함)
		}


		if (a_value > 0x0)
		{
			m_vecPixelPos.push_back(POS(newPos.m_x, newPos.m_y));
			prevPos = POS(newPos.m_x, newPos.m_y);

			iDir = REVERSE_DIR(iDir) + 1; // 시작할 방향 : 반대 + 1
		}
		else
		{
			iDir = iDir + 1;
		}

		cnt++;
	}
}

void CMainView::OnMouseUp(LPARAM _lParam)
{
	m_bIsInvalidSlicedRect = false;
	m_bIsLButtonDown = false;

	ID2D1Bitmap* d2d1_temp = *(m_pMyBitmap->GetD2DBitmap());
	UINT width = (UINT)d2d1_temp->GetSize().width;
	UINT height = (UINT)d2d1_temp->GetSize().height;

	WORD pos_X = LOWORD(_lParam);
	WORD pos_Y = HIWORD(_lParam);
	ptTempRB = { pos_X, pos_Y };

	SLICE_RECT_POS srp = {};

	if (m_bDragSliceState)
	{
		srp._ptDragLeftTop = { ptTempLT.x, ptTempLT.y };
		srp._ptDragRightBottom = { ptTempRB.x, ptTempRB.y };

		LONG max_X = 0L;			// LeftTop.x
		LONG max_Y = 0L;			// LeftTop.y
		LONG min_X = LONG_MAX;		// RightBottom.x
		LONG min_Y = LONG_MAX;		// RightBottom.y
		bool bPassed = false;

		// 220627 sprite 중간에 사각형 쳐지는 문제 해결
		for (LONG y = ptTempLT.y; y <= ptTempRB.y; ++y)
		{
			LONG x = ptTempLT.x;

			DWORD hex_color = m_pMyWICBitmap->GetPixelColor(x, y, width, height);
			BYTE alpha_value = (hex_color & 0xff000000) >> (8 * 3);

			if (alpha_value == 0xff)
				break;
		}

		for (LONG y = ptTempLT.y; y <= ptTempRB.y; ++y)
		{
			LONG x = ptTempRB.x;

			DWORD hex_color = m_pMyWICBitmap->GetPixelColor(x, y, width, height);
			BYTE alpha_value = (hex_color & 0xff000000) >> (8 * 3);

			if (alpha_value == 0xff)
			{
				m_bIsInvalidSlicedRect = true;
				break;
			}
		}

		for (LONG x = ptTempLT.x; x <= ptTempRB.x; ++x)
		{
			LONG y = ptTempLT.y;

			DWORD hex_color = m_pMyWICBitmap->GetPixelColor(x, y, width, height);
			BYTE alpha_value = (hex_color & 0xff000000) >> (8 * 3);

			if (alpha_value == 0xff)
			{
				m_bIsInvalidSlicedRect = true;
				break;
			}
		}

		for (LONG x = ptTempLT.x; x <= ptTempRB.x; ++x)
		{
			LONG y = ptTempRB.y;

			DWORD hex_color = m_pMyWICBitmap->GetPixelColor(x, y, width, height);
			BYTE alpha_value = (hex_color & 0xff000000) >> (8 * 3);

			if (alpha_value == 0xff)
			{
				m_bIsInvalidSlicedRect = true;
				break;
			}
		}

		// Drag Slice 버그 방지용
		if (m_bIsInvalidSlicedRect) // Drag Slice 상태가 해제되지 않는다. (밑에서 false 되기 전 탈출)
			return;

		for (LONG y = ptTempLT.y + 1; y < ptTempRB.y; ++y)
		{
			for (LONG x = ptTempLT.x + 1; x < ptTempRB.x; ++x)
			{
				DWORD hex_color = m_pMyWICBitmap->GetPixelColor(x, y, width, height);
				BYTE alpha_value = (hex_color & 0xff000000) >> (8 * 3);

				if (alpha_value != 0xff)
					continue;

				if (x < 0 || y < 0) //  || x > width || y > height
					continue;

				if (x > max_X && x < ptTempRB.x)
					max_X = x;

				if (y > max_Y && y < ptTempRB.y)
					max_Y = y;

				if (x < min_X && x > ptTempLT.x)
					min_X = x;

				if (y < min_Y && y > ptTempLT.y)
					min_Y = y;

				bPassed = true;
			}
		}

		if (bPassed)
		{
			srp._ptDragLeftTop = { min_X - 1, min_Y - 1 };
			srp._ptDragRightBottom = { max_X + 3, max_Y + 3 };

			m_vSlicedPos.push_back(srp);
		}
	}

	//m_bDragSliceState = false;
	// -> 일단 연속 사용 가능하도록..!
	// 추후 수정 필요!
}

void CMainView::Render()
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


	//...

	DrawSlicedSprite(); // 신규 분리 함수
}

LRESULT CMainView::WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	HMENU hMenu;

	switch (_message)
	{
	case WM_CREATE:
	{
		hMenu = GetMenu(m_hWnd);
		CheckMenuRadioItem(hMenu,
			ID_RADIOBUTTON_RADIOTEST1, ID_RADIOBUTTON_RADIOTEST3, ID_RADIOBUTTON_RADIOTEST2,
			MF_BYCOMMAND);

		OnCreate();
	}
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(_hWnd, &ps);

		// D2D1
		Update();
		Render();

		EndPaint(_hWnd, &ps);
	}
		break;

	case WM_MOUSEMOVE:
	{
		OnMouseMove(_lParam);

		InvalidateRect(m_hWnd, NULL, false); // false인 것에 주의 (더블버퍼링 X)
	}
	break;

	case WM_LBUTTONDOWN:
	{
		OnMouseDown(_lParam);
	}
	break;

	case WM_LBUTTONUP:
	{
		OnMouseUp(_lParam);

		InvalidateRect(m_hWnd, NULL, true); // true
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

		case ID_SPRITE_AUTOSLICE:
		{
			if (m_bDragSliceState)
				m_bDragSliceState = false;
			if (m_bSetAlphaZeroState)
				m_bSetAlphaZeroState = false;

			m_bAutoSliceState = true;
		}
		break;

		case ID_SPRITE_DRAGSLICE:
		{
			if (m_bDragSliceState)
				m_bDragSliceState = false;
			if (m_bSetAlphaZeroState)
				m_bSetAlphaZeroState = false;

			m_bDragSliceState = true;
		}
		break;

		case ID_COLOR_SETALPHAZERO:
		{
			if (m_bDragSliceState)
				m_bDragSliceState = false;
			if (m_bDragSliceState)
				m_bDragSliceState = false;

			m_bSetAlphaZeroState = true;
		}
		break;

		//============== [Menu] ==================
		case ID_RADIOBUTTON_RADIOTEST1:
			hMenu = GetMenu(m_hWnd);
			CheckMenuRadioItem(hMenu,
				ID_RADIOBUTTON_RADIOTEST1, ID_RADIOBUTTON_RADIOTEST3, ID_RADIOBUTTON_RADIOTEST1,
				MF_BYCOMMAND);
			break;

		case ID_RADIOBUTTON_RADIOTEST2:
			hMenu = GetMenu(m_hWnd);
			CheckMenuRadioItem(hMenu,
				ID_RADIOBUTTON_RADIOTEST1, ID_RADIOBUTTON_RADIOTEST3, ID_RADIOBUTTON_RADIOTEST2,
				MF_BYCOMMAND);
			break;

		case ID_RADIOBUTTON_RADIOTEST3:
			hMenu = GetMenu(m_hWnd);
			CheckMenuRadioItem(hMenu,
				ID_RADIOBUTTON_RADIOTEST1, ID_RADIOBUTTON_RADIOTEST3, ID_RADIOBUTTON_RADIOTEST3,
				MF_BYCOMMAND);
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