#include <Windows.h>

#include "Bitmap.h"
#include "Wnd.h"


class CApp
{
private:
	HINSTANCE				m_hInstance;

	//CWnd* m_Wnd[2];

	CWnd*				m_pMainWindow;
	CWnd*				m_pToolWindow;

public:
	CApp();
	~CApp();

public:
	HRESULT Init(HINSTANCE _hInstance, int _nCmdShow);
	HRESULT CreateWindows(HINSTANCE _hInstance, int _nCmdShow);

public:
	int Run();
	void Release();
};