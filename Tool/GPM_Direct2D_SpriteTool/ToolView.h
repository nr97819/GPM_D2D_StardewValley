#pragma once

#include "Wnd.h"

#include <vector>

struct SLICE_RECT_POS
{
	POINT _ptDragLeftTop = {};
	POINT _ptDragRightBottom = {};
};

class CToolView : public CWnd
{
private:
	bool m_bSetAlphaZeroState;
	bool m_bDragSliceState;

	bool m_bIsLButtonDown;

	POINT ptTempLT = {};
	POINT ptTempRB = {};

	std::vector<SLICE_RECT_POS> m_vSlicePos{};

private:
	DWORD m_alphaZeroColor;

public:
	CToolView();
	~CToolView();

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

};