#pragma once

#include "Wnd.h"

#include "SpriteView.h"
#include <vector>


class CAnimView : public CWnd
{
private:
	std::vector<SPRITE_INFO> m_vSelectedSprites;

	//static std::vector<SPRITE_INFO> m_vResultVec;

public:
	CAnimView();
	~CAnimView();

public:
	virtual void Render() override;

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

};

