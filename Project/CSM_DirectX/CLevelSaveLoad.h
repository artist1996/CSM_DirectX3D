#pragma once

class CLevelSaveLoad
{
public:
	static void SaveLevel(const wstring& _FilePath, CLevel* _pLevel);
	static void SaveGameObject(FILE* _pFile, CGameObject* _Object);

	static class CLevel* LoadLevel(const wstring& _FilePath);
	static class CGameObject* LoadGameObject(FILE* _pFile);
	static class CComponent* GetComponent(COMPONENT_TYPE _Type);
};

