#pragma once

#include "Wnd.h"

#include "SpriteView.h"
#include <vector>


class CAnimView : public CWnd
{
private:
	std::vector<SPRITE_INFO> m_vSelectedSprites;

	std::vector<SPRITE_INFO> m_vAnimVec;

	CBitmap* m_pSprieViewBitmap;
	ID2D1Bitmap* m_spriteViewD2DBitmap;

public:
	CAnimView();
	~CAnimView();

public:
	void Init();
	virtual void Update() override;
	virtual void Render() override;

public:
	void OnMouseUp();

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

};

