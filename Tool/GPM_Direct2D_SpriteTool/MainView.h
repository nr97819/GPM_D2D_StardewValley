#pragma once

#include "Wnd.h"

#define REVERSE_DIR(value)	((UINT)value + 4) % 8
#define GET_DIR(value)		POS(\
								(m_vecPixelPos.back().m_x + DIR_X[(UINT)value]),\
								(m_vecPixelPos.back().m_y + DIR_Y[(UINT)value])\
							)\

enum class DIR
{
	UP,			// 0
	RIGHT_UP,	// 1
	RIGHT,		// 2
	RIGHT_DOWN,	// 3
	DOWN,		// 4
	LEFT_DOWN,	// 5
	LEFT,		// 6
	LEFT_UP		// 7

	// reverse : (x+4) % 8
};

static UINT DIR_X[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
static UINT DIR_Y[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

struct POS
{
	UINT m_x;
	UINT m_y;

	POS()
	{
		m_x = 0;
		m_y = 0;
	}

	POS(UINT _x, UINT _y)
	{
		m_x = _x;
		m_y = _y;
	}

	bool operator==(POS& _other)
	{
		if (_other.m_x != this->m_x)
			return false;

		if (_other.m_y != this->m_y)
			return false;

		return true;
	}
};

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
	static std::vector<POS> m_vecPixelPos;

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