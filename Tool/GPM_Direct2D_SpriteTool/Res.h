#pragma once

#include <string>
using std::wstring;

class CRes
{
private:
	wstring		m_strKey;
	wstring		m_strRelativePath;

public:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelaivePath(const wstring& _strpath) { m_strRelativePath = _strpath; }

	const wstring& GetKey() { return m_strKey; }
	const wstring& GetRelativePath() { return m_strRelativePath; }

public:
	CRes();
	~CRes();
};