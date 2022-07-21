#include "Bitmap.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib") // WIC Lib

#include <wchar.h>
#include "D2DCore.h"

#include "ResMgr.h"

CBitmap::CBitmap()
	: m_pD2DBitmap(nullptr)
{}

CBitmap::~CBitmap()
{}

HRESULT CBitmap::Create(const wstring& _wsFileName, ID2D1HwndRenderTarget* _pRenderTarget)
{
	HRESULT hr = S_OK;

	CD2DCore::GetInst()->CreateD2D1Bitmap(_wsFileName, _pRenderTarget, &m_pD2DBitmap);

	// Resource 상속 관련
	CResMgr::GetInst()->Insert(_wsFileName, *((CRes*)this));
	
	// 임시로 그냥 경로로 설정
	this->SetKey(_wsFileName);
	this->SetRelaivePath(_wsFileName);

	return S_OK;
}

void CBitmap::Release()
{
	wstring key = this->GetKey();
	CResMgr::GetInst()->Delete(key);

	if (m_pD2DBitmap) 
	{ 
		m_pD2DBitmap->Release(); 
		m_pD2DBitmap = nullptr; 
	}
}
