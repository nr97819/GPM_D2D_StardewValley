#pragma once

#include "Wnd.h"

enum class SPRITE_MODE
{
	NONE = 0,
	SELECT = 1,
	UNSELECT = 2
};

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

	// ============ TEST ============
	static std::vector<D2D1_RECT_F> m_vSlicedRects;

	// Sprite Mode
	UINT m_spriteMode = (UINT)SPRITE_MODE::NONE;

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
	static std::vector<D2D1_RECT_F>& GetSlicedRects() { return m_vSlicedRects; }

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;
};