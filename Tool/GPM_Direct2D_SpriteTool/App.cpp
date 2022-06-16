#include "App.h"

#include "D2DCore.h"

#include "ToolView.h"
#include "ImgView.h"

CApp::CApp()
	: m_hInstance(nullptr)
	, m_pMainWindow(nullptr)
	, m_pToolWindow(nullptr)
{
}

CApp::~CApp()
{
}

HRESULT CApp::Init(HINSTANCE _hInstance, int _nCmdShow)
{
	CreateWindows(_hInstance, _nCmdShow);
	
	return S_OK;
}

HRESULT CApp::CreateWindows(HINSTANCE _hInstance, int _nCmdShow)
{
	HWND m_hWnd = NULL;
	wstring wsFilePath = L"images\\";

	// [ Main ]
	m_pMainWindow = new CToolView();
	m_hWnd = m_pMainWindow->Create(_hInstance, _nCmdShow, L"MainApp", L"Main Window App", CWnd::WndProc, true); // Show Window 분리 필요
	if (NULL == m_hWnd)
		return E_FAIL;
	m_pMainWindow->_ShowWindow();
	m_pMainWindow->InitRenderTarget(wsFilePath + L"midnight.png");
	m_pMainWindow->Adjust(POINT{ 1000, 700 }, POINT{ 100, 100 });

	// [ Tool ]
	m_pToolWindow = new CImgView();
	m_hWnd = m_pToolWindow->Create(_hInstance, _nCmdShow, L"ToolApp", L"Tool Window App", CWnd::WndProc, false);
	if (NULL == m_hWnd)
		return E_FAIL;
	m_pToolWindow->_ShowWindow();
	m_pToolWindow->InitRenderTarget(wsFilePath + L"woman.png"); //전용 함수 필요
	m_pToolWindow->Adjust(POINT{ 300, 700 }, POINT{ ((int)m_pMainWindow->GetRT()->GetSize().width + 100), 100 });

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
	if (m_pMainWindow) 
	{ 
		m_pMainWindow->Release(); 
		delete m_pMainWindow; 
		m_pMainWindow = nullptr; 
	}

	if (m_pToolWindow) 
	{ 
		m_pToolWindow->Release(); 
		delete m_pToolWindow; 
		m_pToolWindow = nullptr; 
	}
}
