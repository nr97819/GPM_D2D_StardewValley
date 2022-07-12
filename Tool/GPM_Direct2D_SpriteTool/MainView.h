#pragma once

#include "Wnd.h"

class CMainView : public CWnd
{
private:
	bool m_bSetAlphaZeroState;
	bool m_bDragSliceState;
	bool m_bAutoSliceState;

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
	void DrawSlicedSprite();

	void AutoSlice();

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;
};