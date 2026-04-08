#include "GameManager.h"

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	/*
	CGameManager gameManager;

	if(!gameManager.init(hInstance))
		return 0;

	return gameManager.run();
	*/
	// CRT 디버그 플래그 설정
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc();

	if (!CGameManager::getInst()->init(hInstance))
	{
		CGameManager::destroyInst();
		return 0;
	}

	int ret = CGameManager::getInst()->run();
	CGameManager::destroyInst();

	return ret;
}
