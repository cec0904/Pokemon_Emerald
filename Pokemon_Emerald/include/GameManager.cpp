#include "GameManager.h"

#include "resource.h"

#include "Share/Timer.h"
#include "Share/Log.h"
#include "Device.h"
#include "Asset/AssetManager.h"
#include "Shader/ShaderManager.h"

#include "Asset/Mesh/MeshManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/Shader.h"

#include "Shader/TransformCBuffer.h"
#include "Scene/SceneManager.h"

#include "ProfileManager.h"

#include "Render/RenderManager.h"
#include "Render/RenderState.h"
#include "Render/RenderStateManager.h"

#include "Asset/Sound/SoundManager.h"
#include "UI/Widget.h"

#include "Pokemon/Pokemon/Pokemon.h"
#include "Pokemon/Pokemon/PokemonManager.h"

//Gameinfo에 extern을 선언해준다.
//그럼 Gameinfo를 받아간 파일들은 모두 gRootPath를 사용할수있다.
TCHAR gRootPath[MAX_PATH];
//멀티바이트 용
char gRootPathMultibyte[MAX_PATH];

bool CGameManager::mLoop = true;

CGameManager::CGameManager()
{
	//생성자
}

CGameManager::~CGameManager()
{
	//소멸자
	
	CLog::Destroy();

	//DC반납
	ReleaseDC(mhWnd, mhDC);
}

bool CGameManager::Init(HINSTANCE hInst)
{
	mhInst = hInst;

	lstrcpy(mClassName, TEXT("Pokemon_Emerald"));
	lstrcpy(mTitleName, TEXT("Pokemon_Emerald"));
	
	//레지스터 클래스 등록 
	RegisterWindowClass();

	if (!Create())
	{
		return false;
	}

	if (!CLog::Init())
	{
		return false;
	}
	


	//디바이스 컨텍스트 얻어오기
	//인자로 들어간 윈도우에 출력할 수 있는 DC가 만들어진다. 
	mhDC = GetDC(mhWnd);

	//디바이스 초기화 
	if (!CDevice::GetInst()->Init(mhWnd, 1280, 720, true))
	{
		return false;
	}

    //Shader 관리자 초기화 
    if (!CShaderManager::GetInst()->Init())
    {
        return false;
    }

	//Render 매니져
	if (!CRenderManager::GetInst()->Init())
	{
		return false;
	}

	//에셋 매니져 초기화
	if (!CAssetManager::GetInst()->Init())
	{
		return false;
	}

	//Collision Profile 매니져 초기화
	if (!CProfileManager::GetInst()->Init())
	{
		return false;
	}

	//타이머 초기화 
	CTimer::Init();

	//장면관리자 
	if (!CSceneManager::GetInst()->Init())
	{
		return false;
	}

	//이때 UI전용 Proj 행렬 초기화
	FResolution RS = CDevice::GetInst()->GetResolution();
	CWidget::CreateUIProjection(static_cast<float>(RS.Width), static_cast<float>(RS.Height), 1000.f);
	
	
	return true;
}

int CGameManager::Run()
{
	MSG msg;

	while (mLoop)
	{
		//아래는 무시해도 되는 친구들이다. 
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		//윈도우 데드타임일 경우 동작한다. 메시지가 없을경우 들어온다. 
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CGameManager::Logic()
{
	float DeltaTime = CTimer::Update();

	Input(DeltaTime);
	Update(DeltaTime);
	Collision(DeltaTime);
	Render(DeltaTime);

	//오디오 시스템은 매프레임 갱신 해줘야한다. 
	CAssetManager::GetInst()->GetSoundManager()->Update();

}

void CGameManager::Input(float DeltaTime)
{
	CSceneManager::GetInst()->Input(DeltaTime);
}

void CGameManager::Update(float DeltaTime)
{
	CSceneManager::GetInst()->Update(DeltaTime);

	static bool push = false;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		push = true;
	}
	else if (push)
	{
		push = false;
		CLog::SaveLog();
	}
}


void CGameManager::Collision(float DeltaTime)
{
	CSceneManager::GetInst()->Collision(DeltaTime);
}


void CGameManager::Render(float DeltaTime)
{
	CDevice::GetInst()->ClearBackBuffer(mClearColor);
	CDevice::GetInst()->ClearDepthStencill(1.f, 0);
	CDevice::GetInst()->SetTarget();


	CSceneManager::GetInst()->Render();

	CRenderManager::GetInst()->Render();

	CSceneManager::GetInst()->EndFrame();

	CDevice::GetInst()->Render();
}

void CGameManager::RegisterWindowClass()
{
	//윈도우 클래스 구조체  
	//윈도우 클래스 구조체는 윈도우의 속성들을 지정하는 구조체이다.
	WNDCLASSEXW wcex;

	// 윈도우 클래스 구조체의 크기를 나타낸다. 반드시 지정되어야 한다. 
	wcex.cbSize = sizeof(WNDCLASSEX);

	// 윈도우 클래스의 스타일을 지정한다.
	// CS_HREDRAW : 윈도우의 가로 크기가 변경되었을 때, 전체 윈도우를 다시 그린다.
	// CS_VREDRAW : 윈도우의 세로 크기가 변경되었을 때, 전체 윈도우를 다시 그린다.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// WndProc는 창의 메시지를 처리하는 함수의 포인터이다.
	// 메시지큐에서 얻어온 메시지를 인자로 넣어서 호출해줄 함수를 주소를 넘겨준다. 
	wcex.lpfnWndProc = WndProc;

	//필요가 없다. 
	// 윈도우 클래스에 대한 추가 메모리를 지정한다.
	wcex.cbClsExtra = 0;
	// 윈도우 인스턴스에 대한 추가 메모리를 지정한다.
	wcex.cbWndExtra = 0;

	// 운ㅇㅕㅇ체제에서 부여해준 HINSTANCE를 지정한다.
	wcex.hInstance = mhInst;

	//실행파일 아이콘을 지정한다. 
	//바탕화면 아이콘 
	//아이콘 만들기 -> 리소스 -> 추가 -> 리소스 -> 아이콘
	//#include "resource.h" 추가 
	wcex.hIcon = LoadIcon(mhInst, MAKEINTRESOURCE(IDI_ICON1));

	//커서모양
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// 클라이언트 영역의 색상을 지정한다. 
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	// 윈도우 메뉴를 지정한다. 
	//wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINOWSSAMPLE);
	wcex.lpszMenuName = 0;  // 0을 넣으면 메뉴가 없는 윈도우가 된다.

	// 윈도우 클래스의 이름을 지정한다.
	wcex.lpszClassName = mClassName;

	// 윈도우창 왼쪽위 아이콘을 말한다. 
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	// 위에서 설정한 윈도우 클래스를 등록한다. 
	RegisterClassExW(&wcex);
}

bool CGameManager::Create()
{
	// CreateWindowW : 윈도우 창을 생성해주는 함수이다. 
	// WinAPI에서 함수명 뒤에 W가 붙으면 유니코드, A가 붙으면 멀티바이트이다. 
	// API란? Application Programming Interface의 약자이다
	// 1번 인자 : 윈도우 클래스 이름을 지정한다. 
	// 2번 인자 : 윈도우 타이틀바에 출력할 이름을 지정한다. 
	// 3번 인자 : 윈도우 창의 모양을 결정한다. 
	// 4,5번 인자 : 화면에서 윈도우 창의 x,y 좌표를 지정한다.
	// 6,7번 인자 : 윈도우 창의 가로, 세로 크기를 지정한다.
	// 8번 인자 : 부모 윈도우의 핸들을 지정한다. 최상위 윈도우이면 nullptr을 넣어준다.
	// 9번 인자 : 메뉴 핸들을 지정한다. 메뉴가 없으면 nullptr을 넣어준다.
	// 10번 인자 : 윈도우를 생성한 프로그램의 인스턴스 핸들을 지정한다.
	// 11번 인자 : 창 생성 데이터를 지정한다. WM_CREATE 메시지의 lParam에 전달된다.
	// 이 메시지가 발생하면 WndProc 함수의 lParam에 전달된다. 거의 사용은 안한다. 
	// 잘못된 생성일 경우 0을 반환한다. 
	mhWnd = CreateWindowW(mClassName, mTitleName, WS_OVERLAPPEDWINDOW,
		100, 200, 1280, 720, nullptr, nullptr, mhInst, nullptr);

	if (!mhWnd)
	{
		return FALSE;
	}
	//여기까지는 창을 만들기만 한것이다. 

	//윈도우 클라이언트 영역의 크기를 원하는 크기로 지정한다. 
	//위에서 지정한 윈도우 크기는 타이틀바 등의 크기가 모두 합쳐진 크기로 지정된다. 
	RECT WindowRC = { 0, 0, 1280, 720 };
	//클라이언트 영역이 1280*720이 되도록 윈도우 크기를 재조정한다.
	AdjustWindowRect(&WindowRC, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(mhWnd, HWND_TOPMOST, 100, 100, WindowRC.right - WindowRC.left, WindowRC.bottom - WindowRC.top, SWP_NOMOVE | SWP_NOZORDER);


	//위에서 윈도우 창을 만들었다면 이제 ShowWinodw 함수를 이용해서 창을 보여줄지 숨길지를 결정한다. 
	//ShowWindow(hWnd, nCmdShow);
	ShowWindow(mhWnd, SW_SHOW);
	// 클라이언트 영역을 강제로 다시 그리게 요청해주는 함수이다. 
	//처음 생성시나 특정상황에 창을 다시 그려야 할때 호출한다.
	UpdateWindow(mhWnd);

	return TRUE;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	//윈도우 창에 그려야 하는 경우 동작한다. 
	//case WM_PAINT:
	//{
	//	PAINTSTRUCT ps;
	//	HDC hdc = BeginPaint(hWnd, &ps);
	//	// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
	//	//위가 Y- 아래가 Y+

	//	//TextOut(hdc, 10, 10, L"Output", 6);
	//	//TextOut(hdc, 100, 100, L"Output", 6);

	//	//Rectangle(hdc, 100, 200, 200, 300);
	//	//Ellipse(hdc, 100, 200, 200, 300);   //내적원 그리기 

	//	////선으ㅣ 시작점을 지정한다.  
	//	//MoveToEx(hdc, 200, 100, nullptr); //시작점
	//	////선의 끝점을 나타낸다.
	//	//LineTo(hdc, 300, 200);
	//	//LineTo(hdc, 400, 100);

	//	//MoveToEx(hdc, 300, 300, nullptr);
	//	//LineTo(hdc, 400, 300);


	//	EndPaint(hWnd, &ps);
	//}
	//break;
	//윈도우창 종료 메시지


	case WM_SYSKEYDOWN:
		if (wParam == VK_F4)
		{
			mLoop = false;
			DestroyWindow(hWnd);
		}
		break;
	//case WM_KEYDOWN:
	//	break;
	case WM_DESTROY:
		mLoop = false;
		PostQuitMessage(0);
		break;
	default:    // 위에서 지정한 메시지 외의 다른 메시지가 발생할 경우 윈도우의 기본 동작으로 처리가 되게 만들어 준다. 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
