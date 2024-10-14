#pragma once
class CTestLevel
{
public:
	static void CreateTestLevel();
	static void CreatePrefab();


private:
	static void CreateTestObject(CLevel* pLevel, Ptr<CMaterial> pMtrl);
};

