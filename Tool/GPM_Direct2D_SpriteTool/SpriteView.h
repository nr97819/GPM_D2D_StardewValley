#pragma once

#include "Wnd.h"

#include "Bitmap.h"
#include <vector>

class CSpriteView : public CWnd
{
private:

private:
	CBitmap*						m_pMyBitmap;
	std::vector<SLICE_RECT_POS>*	m_slicedSpritesVec;

public:
	CSpriteView();
	~CSpriteView();

public:
	//void InitResource(CBitmap* _pMyBitmap, std::vector<SLICE_RECT_POS>* _slicedSpritesVec);
	virtual void Update() override;
	virtual void Render() override;

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

};
