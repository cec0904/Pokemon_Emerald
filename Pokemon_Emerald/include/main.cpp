
#include "GameManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(749);
    
    //int Size = sizeof(MyTestUnion);
    //MyTestUnion unionTest;
    //unionTest.Number = 100;
    //float fv = unionTest.fNum;
    //unionTest.fNum = 1.3f;
    

    if (!CGameManager::GetInst()->Init(hInstance))
    {
        return 0;
    }

    CGameManager::GetInst()->Run();


    return 0;
}