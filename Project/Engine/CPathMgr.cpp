#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr()
{

}
CPathMgr::~CPathMgr()
{

}

void CPathMgr::Init()
{
	// 현재 작업 디렉터리를 알아낸다. (실행시킨 *.exe 파일 위치 OR 프로젝트 작업디렉터리 위치)
	
	// 실행 파일 위치와 프로젝트 작업 디렉터리 통일시키는 법
	// => 프로젝트 속성 - (일반 속성 - 출력 디렉터리)와 (디버깅 속성 - 작업 디렉터리) 일치시키기
	wchar_t szCurDir[256];
	GetCurrentDirectory(256, szCurDir);

	int Len = wcslen(szCurDir);
	for (int i = Len - 1; 0 < i; --i)
	{
		if (szCurDir[i] == L'\\')
		{
			szCurDir[i + 1] = L'\0';
			break;
		}
	}


	m_BinPath = szCurDir;
	m_ContentPath = m_BinPath + L"content\\";
	m_BinPath += L"bin\\";
}
