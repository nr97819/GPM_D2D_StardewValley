#pragma once

#include <wtypes.h>
#include <string>
using std::wstring;
#include <map>
using std::map;

#include "define.h"

#include "Res.h"

/*
#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
					private:\
						type();\
						~type();
*/

class CResMgr
{
	SINGLE(CResMgr);
private:
	map<wstring, CRes>	m_resMap;

public:
	/*void Init();
	void Update();*/

public:
	CRes* FindByKey(const wstring& _keyValue);

	void Insert(const wstring& _keyValue, CRes& _res) 
	{ 
		m_resMap.insert(std::make_pair(_keyValue, _res)); 
	}
	void Delete(wstring& _keyValue)
	{ 
		FindByKey(_keyValue);
	}
};