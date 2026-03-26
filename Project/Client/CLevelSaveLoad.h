#pragma once

class CLevel;
class CGameObject;
class CScript;

class CLevelSaveLoad
{
public:
	static int SaveLevel(const wstring& _FilePath, CLevel* _Level);
	static int SaveGameObject(CGameObject* _Object, FILE* _File);

	static CLevel* LoadLevel(const wstring& _FilePath);
	static CGameObject* LoadGameObject(FILE* _File);

	static vector<CGameObject*> LoadGameObjectFile(const wstring& _FilePath);

private:
	static class CComponent* CreateComponent(COMPONENT_TYPE _Type);
};

