#pragma once

#include "Wnd.h"

#include <vector>

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

class CToolView : public CWnd
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

	std::vector<SLICE_RECT_POS> m_vSlicePos{};

private:
	DWORD m_alphaZeroColor;

public:
	CToolView();
	~CToolView();

public:
	std::vector<SLICE_RECT_POS>* GetSlicedPos() { return &m_vSlicePos; }

public:
	virtual void Render() override;

public:
	// 급하게 추가함
	void OnCreate();

	void OnMouseMove(LPARAM _lParam);
	void OnMouseUp(LPARAM _lParam);
	void OnMouseDown(LPARAM _lParam);

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;
};