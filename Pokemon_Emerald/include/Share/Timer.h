#pragma once
//GameInfo
#include "../Gameinfo.h"

/*
	텔타 타입이란? 
	프레임 간의 시간차이를 의미한다. 
	FPS 는 1초에 몇 번 화면이 갱신 되었는가를 의미한다.
	60FPS -> 1초에 화면이 60번 그려진거다 .
	60FPS 일때 1프레임은 몇초인가? 0.016666초 정도 된다. 
*/

class CTimer
{
	friend class CGameManager;

private:
	static LARGE_INTEGER mSecond;
	static LARGE_INTEGER mTime;
	static float mDeltaTime;		//델타타임을 나타낼것이다. 

private:
	static void Init();
	static float Update();

public:
	static float GetDeltaTime();

};

