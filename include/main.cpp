#include "GameManager.h"

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	if (!CGameManager::getInst()->init(hInstance))
	{
		CGameManager::destroyInst();
		return 0;
	}

	int ret = CGameManager::getInst()->run();
	CGameManager::destroyInst();

	return ret;
}