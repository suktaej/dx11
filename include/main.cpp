#include "GameManager.h"

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	/*
	CGameManager gameManager;

	if(!gameManager.init(hInstance))
		return 0;

	return gameManager.run();
	*/
	
	if (!CGameManager::getInst()->init(hInstance))
	{
		CGameManager::destroyInst();
		return 0;
	}

	int ret = CGameManager::getInst()->run();
	CGameManager::destroyInst();

	return ret;
}