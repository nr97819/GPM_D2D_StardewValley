#include "App.h"

#include "D2DCore.h"

#include "ToolView.h"
#include "ImgView.h"
#include "SpriteView.h"

CApp::CApp()
	: m_hInstance(nullptr)

	, m_arrWnds{}
{
}

CApp::~CApp()
{
}

HRESULT CApp::Init(HINSTANCE _hInstance, int _nCmdShow)
{


	CreateWindows(_hInstance, _nCmdShow);
	
	

	//// Creaate New Window (런타임에 새 윈도우 창 생성)
	//HWND tempHWnd = NULL;
	//wstring wsFilePath = L"images\\";

	//m_arrWnds[(UINT)WND_TYPE::Sprite] = new CSpriteView();



	//CBitmap* pMyBitmap_temp = m_arrWnds[(UINT)WND_TYPE::Tool]->GetMyBitmap();
	//CToolView* pToolView_temp = (CToolView*)(m_arrWnds[(UINT)WND_TYPE::Tool]);
	//std::vector<SLICE_RECT_POS>* pVecSlicedPos_temp = pToolView_temp->GetSlicedPos();
	//static_cast<CSpriteView*>(m_arrWnds[(UINT)WND_TYPE::Sprite])->InitResource(pMyBitmap_temp, pVecSlicedPos_temp);



	//tempHWnd = m_arrWnds[(UINT)WND_TYPE::Sprite]->Create(_hInstance, _nCmdShow, L"SpriteApp", L"Sprite Window App", CWnd::WndProc, true); // Show Window 분리 필요
	//if (NULL == tempHWnd)
	//	return E_FAIL;
	//m_arrWnds[(UINT)WND_TYPE::Sprite]->_ShowWindow();

	//m_arrWnds[(UINT)WND_TYPE::Sprite]->InitTarget();
	//m_arrWnds[(UINT)WND_TYPE::Sprite]->InitBitmap(wsFilePath + L"cookie.png");


	//m_arrWnds[(UINT)WND_TYPE::Sprite]->Adjust(
	//	POINT{ 600, 300 },
	//	POINT{ 200, 100 }
	//);


	// Create WIC Bitmap


	return S_OK;
}

HRESULT CApp::CreateWindows(HINSTANCE _hInstance, int _nCmdShow)
{
	HWND tempHWnd = NULL;
	wstring wsFilePath = L"images\\";

	// [ Main ]
	m_arrWnds[(UINT)WND_TYPE::Tool] = new CToolView();
	tempHWnd = m_arrWnds[(UINT)WND_TYPE::Tool]->Create(_hInstance, _nCmdShow, L"MainApp", L"Main Window App", CWnd::WndProc, true); // Show Window 분리 필요
	if (NULL == tempHWnd)
		return E_FAIL;
	m_arrWnds[(UINT)WND_TYPE::Tool]->_ShowWindow();
	m_arrWnds[(UINT)WND_TYPE::Tool]->InitTarget();
	m_arrWnds[(UINT)WND_TYPE::Tool]->InitBitmap(wsFilePath + L"woman.png");
	m_arrWnds[(UINT)WND_TYPE::Tool]->Adjust(
		POINT{ 1000, 700 },
		POINT{ 100, 100 }
	);

	// [ Tool ]
	m_arrWnds[(UINT)WND_TYPE::Img] = new CImgView();
	tempHWnd = m_arrWnds[(UINT)WND_TYPE::Img]->Create(_hInstance, _nCmdShow, L"ToolApp", L"Tool Window App", CWnd::WndProc, false);
	if (NULL == tempHWnd)
		return E_FAIL;
	m_arrWnds[(UINT)WND_TYPE::Img]->_ShowWindow();
	m_arrWnds[(UINT)WND_TYPE::Img]->InitTarget();
	m_arrWnds[(UINT)WND_TYPE::Img]->InitBitmap(wsFilePath + L"woman.png");
	m_arrWnds[(UINT)WND_TYPE::Img]->Adjust(
		POINT{ 300, 700 }, 
		POINT{ ((int)m_arrWnds[(UINT)WND_TYPE::Tool]->GetRT()->GetSize().width + 100), 100 } // 옆에 고정
	);

	// Tool Window 크기 조정하면 Main Window도 영향을 받아서, 되돌리는 코드
	//m_pMainWindow->GetRT()->Resize({ (UINT32)m_pMainWindow->GetRT()->GetSize().width, (UINT32)700 });

	return S_OK;
}

int CApp::Run()
{
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

void CApp::Release()
{
	for (UINT i = 0; i < (UINT)WND_TYPE::LAST_INDEX; ++i)
	{
		if (m_arrWnds[i])
		{
			m_arrWnds[i]->Release();
			delete m_arrWnds[i];
			m_arrWnds[i] = nullptr;
		}
	}
}
