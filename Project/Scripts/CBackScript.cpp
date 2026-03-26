#include "pch.h"
#include "CBackScript.h"

CBackScript::CBackScript()
	: CScript((UINT)SCRIPT_TYPE::BACKSCRIPT)
{
}

CBackScript::CBackScript(const CBackScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::BACKSCRIPT)
{
}

CBackScript::~CBackScript()
{
}
