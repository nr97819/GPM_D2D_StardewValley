#pragma once

#include "Wnd.h"

class CImgView : public CWnd
{
public:
	CImgView();
	~CImgView();

private:

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

};

