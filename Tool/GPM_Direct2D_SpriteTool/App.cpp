#include "App.h"

#include "D2DCore.h"

#include "MainView.h"
#include "SpriteView.h"
#include "AnimView.h"

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
	
	//// Creaate New Window (��Ÿ�ӿ� �� ������ â ����)
	//HWND tempHWnd = NULL;
	//wstring wsFilePath = L"images\\";

	//m_arrWnds[(UINT)WND_TYPE::Sprite] = new CSpriteView();



	//CToolView* pToolView_temp = (CToolView*)(m_arrWnds[(UINT)WND_TYPE::Tool]);
	//CBitmap* pMyBitmap_temp = pToolView_temp->GetMyBitmap();
	//std::vector<SLICE_RECT_POS>* pVecSlicedPos_temp = pToolView_temp->GetSlicedPos();
	//static_cast<CSpriteView*>(m_arrWnds[(UINT)WND_TYPE::Sprite])->InitResource(pMyBitmap_temp, pVecSlicedPos_temp);



	//tempHWnd = m_arrWnds[(UINT)WND_TYPE::Sprite]->Create(_hInstance, _nCmdShow, L"SpriteApp", L"Sprite Window App", CWnd::WndProc, true); // Show Window �и� �ʿ�
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
	m_arrWnds[(UINT)WND_TYPE::Main] = new CMainView();
	tempHWnd = m_arrWnds[(UINT)WND_TYPE::Main]->Create(_hInstance, _nCmdShow, L"Main View", L"Main Window App", CWnd::WndProc, true); // Show Window �и� �ʿ�
	if (NULL == tempHWnd)
		return E_FAIL;
	m_arrWnds[(UINT)WND_TYPE::Main]->InitTarget();
	m_arrWnds[(UINT)WND_TYPE::Main]->InitRedBursh(); // ���ʿ�
	m_arrWnds[(UINT)WND_TYPE::Main]->InitBitmap(wsFilePath + L"woman.png");
	m_arrWnds[(UINT)WND_TYPE::Main]->_ShowWindow();
	m_arrWnds[(UINT)WND_TYPE::Main]->Adjust(
		POINT{ 1000, 800 },
		POINT{ 50, 50 }
	);

	// [Sprite]
	m_arrWnds[(UINT)WND_TYPE::Sprite] = new CSpriteView();
	tempHWnd = m_arrWnds[(UINT)WND_TYPE::Sprite]->Create(_hInstance, _nCmdShow, L"Sprite View", L"Sprite Window App", CWnd::WndProc, false);
	if (NULL == tempHWnd)
		return E_FAIL;
	m_arrWnds[(UINT)WND_TYPE::Sprite]->InitTarget();
	m_arrWnds[(UINT)WND_TYPE::Sprite]->InitRedBursh(); // ���ʿ�
	m_arrWnds[(UINT)WND_TYPE::Sprite]->InitBitmap(wsFilePath + L"woman.png");
	m_arrWnds[(UINT)WND_TYPE::Sprite]->_ShowWindow(); // ����, SpriteView �� Ȱ��ȭ ��, Show �ϵ��� ����
	m_arrWnds[(UINT)WND_TYPE::Sprite]->Adjust(
		POINT{ 600, ((int)m_arrWnds[(UINT)WND_TYPE::Main]->GetRT()->GetSize().height / 2) },		// size
		POINT{					// position
			((int)m_arrWnds[(UINT)WND_TYPE::Main]->GetRT()->GetSize().width + 50), // ���� ó�� ���� ����
			50
		} // ���� ����
	);

	// [ Image ]
	m_arrWnds[(UINT)WND_TYPE::Image] = new CAnimView();
	tempHWnd = m_arrWnds[(UINT)WND_TYPE::Image]->Create(_hInstance, _nCmdShow, L"Animation View", L"Animation Window App", CWnd::WndProc, false);
	if (NULL == tempHWnd)
		return E_FAIL;
	m_arrWnds[(UINT)WND_TYPE::Image]->InitTarget();
	m_arrWnds[(UINT)WND_TYPE::Image]->InitBitmap(wsFilePath + L"midnight.png");
	m_arrWnds[(UINT)WND_TYPE::Image]->_ShowWindow();
	m_arrWnds[(UINT)WND_TYPE::Image]->Adjust(
		POINT{ 600, ((int)m_arrWnds[(UINT)WND_TYPE::Main]->GetRT()->GetSize().height / 2) },		// size
		POINT{					// position
			((int)m_arrWnds[(UINT)WND_TYPE::Main]->GetRT()->GetSize().width + 50), // ���� ó�� ���� ����
			((int)m_arrWnds[(UINT)WND_TYPE::Main]->GetRT()->GetSize().height / 2) + 50
		} // ���� ����
	);

	// Tool Window ũ�� �����ϸ� Main Window�� ������ �޾Ƽ�, �ǵ����� �ڵ�
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
