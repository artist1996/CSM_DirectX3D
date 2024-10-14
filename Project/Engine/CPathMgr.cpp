#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"

CPathMgr::CPathMgr()	
{
}

CPathMgr::~CPathMgr()
{
}

void CPathMgr::Init()
{
	// 실행경로를 얻어낸다
	wchar_t szBuffer[256] = {};
	GetCurrentDirectory(256, szBuffer);

	// bin 폴더의 상위폴더로 접근한다.
	GetParentPath(szBuffer);

	// \\Content\\ 를 붙여둔다
	m_Content = szBuffer;
	m_Content += L"\\content\\";	
}

void CPathMgr::render()
{	
}

void CPathMgr::GetParentPath(_Inout_ wchar_t* _Buffer)
{
	size_t len = wcslen(_Buffer);
	
	for (size_t i = len - 1; 0 < i; --i)
	{
		if (L'\\' == _Buffer[i])
		{
			_Buffer[i] = L'\0';
			break;
		}
	}
}

wstring CPathMgr::GetRelativePath(const wstring& _FilePath)
{
	// Content Path 의 Pos 를 찾아온다.
	size_t FindPos = _FilePath.find(m_Content);

	if (FindPos == wstring::npos)
		return L"";

	// 찾아온 FindPos 의 Pos가 맨 뒤의 Pos를 반환 하는게 아닌 맨 앞의 Pos를
	// 반환하기 때문에 Content Path와 더 해준 후 _FilePath의 length 를 넣어준다.
	return _FilePath.substr(FindPos + m_Content.length(), _FilePath.length());
}
