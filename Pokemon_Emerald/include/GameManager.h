#pragma once

#include "Gameinfo.h"

class CGameManager
{
	DECLARE_SINGLETON(CGameManager)

private:
	static bool mLoop;
	HINSTANCE mhInst = 0;
	HWND mhWnd = 0;
	HDC mhDC = 0;
	TCHAR mClassName[256] = {};
	TCHAR mTitleName[256] = {};

	//사각형 그리던거 삭제 

	//배경색
	//처음할때는 검정색 비추한다. 
	float mClearColor[4] = { 0.f, 0.f, 0.f, 1.f };

public:
	HINSTANCE GetWindowInstance()	const
	{
		return mhInst;
	}

	HWND GetWindowHandle()	const
	{
		return mhWnd;
	}

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Input(float DeltaTime);
	void Update(float DeltaTime);
	void Collision(float DeltaTime);
	void Render(float DeltaTime);
	


private:
	void RegisterWindowClass();
	bool Create();

	//전역함수로 넣어야 하기때문에 
	//static을 추가해준다. 
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

