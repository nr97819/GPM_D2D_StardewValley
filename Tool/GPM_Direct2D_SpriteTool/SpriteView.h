#pragma once

#include "Wnd.h"

#include "Bitmap.h"
#include <vector>

enum SELECT_TYPE
{
	UNSELECTED = 0,
	SELECTED = 1
};

struct SPRITE_INFO
{
	POINT		m_ptStartPos;
	D2D1_RECT_F	m_d2dRect;
	UINT		m_iWidth;
	UINT		m_iHeight;
	UINT		m_iSelected;

	SPRITE_INFO()
	{
		m_ptStartPos = {};
		m_d2dRect = {};
		m_iWidth = 0;
		m_iHeight = 0;
		m_iSelected = UNSELECTED;
	}

	SPRITE_INFO(POINT _ptStartPos, D2D1_RECT_F _dRect, UINT _iWidth, UINT _iHeight, UINT _iSelected)
	{
		m_ptStartPos = _ptStartPos;
		m_d2dRect = _dRect;
		m_iWidth = _iWidth;
		m_iHeight = _iHeight;
		m_iSelected = UNSELECTED;
	}

	SPRITE_INFO& operator=(const SPRITE_INFO& _other)
	{
		m_ptStartPos = _other.m_ptStartPos;
		m_d2dRect = _other.m_d2dRect;
		m_iWidth = _other.m_iWidth;
		m_iHeight = _other.m_iHeight;
		m_iSelected = _other.m_iSelected;

		return *this;
	}

	bool operator==(const SPRITE_INFO& _other)
	{
		bool bResult = true;

		bResult &= (m_ptStartPos.x == _other.m_ptStartPos.x && 
					m_ptStartPos.y == _other.m_ptStartPos.y);
		bResult &= (m_d2dRect.top == _other.m_d2dRect.top &&
					m_d2dRect.bottom == _other.m_d2dRect.bottom &&
					m_d2dRect.left == _other.m_d2dRect.left &&
					m_d2dRect.right == _other.m_d2dRect.right);
		bResult &= (m_iWidth == _other.m_iWidth);
		bResult &= (m_iHeight == _other.m_iHeight);

		return bResult;
	}
};

class CSpriteView : public CWnd
{
private:
	CBitmap*							m_pMyBitmap;
	std::vector<SPRITE_INFO>			m_slicedSpritesVec;

	// AnimView로 보내기 위한 (선택된) 스프라이트 벡터
	static std::vector<SPRITE_INFO>		m_selectedSpritesVec;

public:
	CSpriteView();
	~CSpriteView();

public:
	//void InitResource(CBitmap* _pMyBitmap, std::vector<SLICE_RECT_POS>* _slicedSpritesVec);
	virtual void Update() override;
	virtual void Render() override;

public:
	virtual LRESULT WndMsgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

public:
	void OnMouseUp(LPARAM _lParam);
	
public:
	static std::vector<SPRITE_INFO>& GetSelectedSprites() { return m_selectedSpritesVec; }

	// 테스트용
	//CBitmap* GetMyBitmap() { return m_pMyBitmap; }
};
