#include <Windows.h>

#include "Bitmap.h"
#include "Wnd.h"

#include <vector>

enum class WND_TYPE
{
	Main,
	Sprite,
	Image,

	LAST_INDEX
};

class CApp
{
private:
	HINSTANCE				m_hInstance;

	CWnd*					m_arrWnds[(UINT)WND_TYPE::LAST_INDEX];

public:
	CApp();
	~CApp();

public:
	HRESULT Init(HINSTANCE _hInstance, int _nCmdShow);
	HRESULT CreateWindows(HINSTANCE _hInstance, int _nCmdShow);

	// CreateNewWindow()

public:
	int Run();
	void Release();

public:
	CWnd* GetMainViewHWnd() { return m_arrWnds[(UINT)WND_TYPE::Main]; }
};