#pragma once

#include "Wnd.h"

enum class MENU_RADIO_ITEM
{
	RADIOTEST1 = 40008,
	RADIOTEST2 = 40009,
	RADIOTEST3 = 40010,

	LAST

	/*
	#define ID_RADIOBUTTON_RADIOTEST1       40008
	#define ID_RADIOBUTTON_RADIOTEST2       40009
	#define ID_RADIOBUTTON_RADIOTEST3       40010
	*/
};

class CMainView : public CWnd
{
private:
	bool m_bSetAlphaZeroState;
	bool m_bDragSliceState;

	bool m_bIsLButtonDown;

	// Drag Slice 버그 방지용
	bool m_bIsInvalidSlicedRect;

	POINT ptTempLT = {};
	POINT ptTempRB = {};

	// ===================== NEW =======================
	POINT ptCurMousePos = {};

private:
	DWORD m_alphaZeroColor;

public:
	CMainView();
	~CMainView();

public:
	//std::vector<SLICE_RECT_POS>* GetSlicedSprites() { return &m_vSlicePos; }

public:
	virtual void Render() override;

public:
	// 급하게 추가함
	void OnCreate();
	void OnMouseMove(LPARAM _lParam);
	void OnMouseUp(LPARAM _lParam);
	void OnMouseDown(LPARAM _lParam);

public:
	void DrawDragedRect();

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;
};