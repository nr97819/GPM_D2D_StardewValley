#include "ResMgr.h"

CResMgr::CResMgr()
	: m_resMap{}
{

}

CResMgr::~CResMgr()
{

}

//void CResMgr::Init()
//{}
//
//void CResMgr::Update()
//{}

CRes* CResMgr::FindByKey(const wstring& _keyValue)
{
	map<wstring, CRes>::iterator it = m_resMap.find(_keyValue);

	if (it == m_resMap.end())
		return nullptr;
	
	return &(it->second);
}