#pragma once


#include <Engine/Ptr.h>
#include <Engine/assets.h>
#include <Engine/CComponent.h>
#include <Engine/CScript.h>

#include "EditorUI.h"

class ParamUI
{
private:
	static UINT g_ParamID;

public:
	static bool Param_Bool(const string& _Desc, bool* _Data);
	static bool Param_Int(const string& _Desc, int* _Data, bool _Drag);
	static bool Param_Float(const string& _Desc, float* _Data, bool _Drag);
	static bool Param_Vec2(const string& _Desc, Vector2* _Data, bool _Drag);
	static bool Param_Vec3(const string& _Desc, Vector3* _Data, bool _Drag);
	static bool Param_Vec4(const string& _Desc, Vector4* _Data, bool _Drag);

	static bool Param_Tex(const string& _Desc, Ptr<CTexture>& _Tex
		, EditorUI* _Inst = nullptr, EUI_DELEGATE_2 _MemFunc = nullptr);

	static bool Param_Prefab(const string& _Desc, Ptr<CPrefab>& _Prefab
		, EditorUI* _Inst = nullptr, EUI_DELEGATE_2 _MemFunc = nullptr);

	static bool Param_GameObject(const string& _Desc, CGameObject*& _Obj);

	static bool Param_Mtrl(const string& _Desc, Ptr<CMaterial>& _Mtrl);

	static bool Param_String(const string& _Desc, wstring* _Str);

	static bool Param_Func(const string& _Desc, CScript* _Inst = nullptr, SCRIPT_DELEGATE _MemFunc = nullptr);

	static bool Param_Bool32(const string& _Desc, UINT* _State);


	friend class CImGuiMgr;
};

