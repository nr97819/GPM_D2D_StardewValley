#pragma once

#include "Wnd.h"

class CAnimView : public CWnd
{
private:
	// ��� ����

public:
	CAnimView();
	~CAnimView();

public:
	virtual void Render() override;

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

};

