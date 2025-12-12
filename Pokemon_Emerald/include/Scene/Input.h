#pragma once

#include "../Gameinfo.h"

#define DIRECTINPUT_VERSION 0x0800

// 16진수
// 2진수 -> 10진수 -> 16진수 
#define DIK_MOUSELBUTTON 0xfc
#define DIK_MOUSERBUTTON 0xfd
#define DIK_MOUSEWHEEL 0xfe

//다이렉트 전용 입력 헤더를 가져올것이다. 
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

enum class EInputSystem_Type
{
	DInput,
	Window
};

namespace EMouseButtonType
{
	enum Type
	{
		LButton,	//왼쪽
		RButton,	//오른쪽
		MButton,	// 휠
		End
	};

}

//어떤키인지 
struct FKeyState
{
	unsigned char Key = 0;
	bool Down = false;
	bool Hold = false;
	bool Up = false;
};

namespace EInputType
{
	enum Type
	{
		Down,
		Hold,
		Up,
		End
	};
}

struct FBindFunction
{
	//객체 주소 
	void* Obj = nullptr;
	//함수 
	function<void(float)> Func;
	//			 반환값(매개변수타입)
};

struct FBindKey
{
	FKeyState* Key = nullptr;
	bool Ctrl = false;
	bool Alt = false;
	bool Shift = false;
	// 20251028 추가 
	bool KeyHold = false;
	//함수 포인터 
	vector<FBindFunction> FunctionList[EInputType::End];
};


class CInput
{
	friend class CScene;

private:
	CInput();
	~CInput();

private:
	HINSTANCE mhInst;
	HWND mhWnd;
	class CScene* mScene = nullptr;;

	//키보드 상태를 만들어준다. 
	unsigned char mKeyState[256] = {};
	DIMOUSESTATE mMouseState = {};

	// 원한 키, 이벤트 이름을 반인드 
	// 모든 키보드 마우스 상태값을 검사하는게아닌
	// 내가 추가한 키랑 이벤트만 상태가 들어왔는지 검사할것이고 
	// 이벤트 이름이랑 키상태랑 함수포인터 바인드 할것이다. 
	// 상태가 들어온 키드만 해당 이벤트 이름을 호출해줄것이다. 

	//해당 키가 눌렸는지 확인하고 
	unordered_map<unsigned char, FKeyState*> mKeyStateMap;
	//해당키가 눌린 상태면 바인드키를 호출한다. 
	unordered_map<string, FBindKey*> mBindKeyMap;


	// 키입력 
	bool mCtrl[EInputType::End] = {};
	bool mAlt[EInputType::End] = {};
	bool mShift[EInputType::End] = {};

	//마우스 버튼 타입 저장 
	bool mMouseDown[EMouseButtonType::End] = {};	//마우스 눌림
	bool mMouseHold[EMouseButtonType::End] = {};	//누르고 있음 
	bool mMouseUp[EMouseButtonType::End] = {};		// 마우스 땟음


	//마우스 정보
	FVector2D mMousePos;
	FVector2D mMouseWorldPos2D;
	FVector2D mMouseMove;

	// 첫번째 프레임 확인하기 위해서 
	bool mMouseCompute = false;





	//디바이스 입력 
	//디바이스 같은 녀석인데 디바이스는 아니다.
	// IDirectInput8 객체로 부터 내가 원하는 장치의 디바이스를 얻어온다. 
	// 키보드 디바이스, 마우스 디바이스, 게임패드 디바이스 등등 
	IDirectInput8* mInput = nullptr;
	IDirectInputDevice8* mKeyboard = nullptr;	//키보드 디바이스 
	IDirectInputDevice8* mMouse = nullptr;		//마우스 디바이스 
	EInputSystem_Type mInputType = EInputSystem_Type::DInput;



public:
	void AddBindKey(const string& Name, unsigned char Key);
	void ChangeKeyCtrl(const string& Name, bool Ctrl);
	void ChangeKeyAlt(const string& Name, bool Alt);
	void ChangeKeyShift(const string& Name, bool Shift);


private:
	bool InitInput();
	bool UpdateKeyboard();
	bool UpdateMouse();

	FKeyState* FindKeyState(unsigned char Key);
	FBindKey* FindBindKey(const string& Name);

public:
	bool Init();
	void Update(float DeltaTime);

private:
	void UpdateMousePos(float DeltaTime);
	void UpdateInput(float DeltaTime);
	void UpdateBind(float DeltaTime);

public:
	//마우스 입력 상태값
	bool GetMouseDown(EMouseButtonType::Type Type)
	{
		return mMouseDown[Type];
	}

	bool GetMouseHold(EMouseButtonType::Type Type)
	{
		return mMouseHold[Type];
	}

	bool GetMouseUp(EMouseButtonType::Type Type)
	{
		return mMouseUp[Type];
	}

	//마우스 좌표 함수
	const FVector2D& GetMousePos() const
	{
		return mMousePos;
	}

	//마우스 월드 포즈
	const FVector2D& GetMouseWorldPos() const
	{
		return mMouseWorldPos2D;
	}

	const FVector2D& GetMouseMoveStep() const
	{
		return mMouseMove;
	}




public:
	template<typename T>
	void AddBindFunction(const string& KeyName, EInputType::Type Type, T* Object , void (T::* Func)(float))
	{
		FBindKey* BindKey = FindBindKey(KeyName);

		if (!BindKey)
			return;

		FBindFunction BindFunc;

		BindFunc.Obj = Object;
		// C++ 문법 : 멤버함수 바인드 문법
		BindFunc.Func = bind(Func, Object, placeholders::_1);

		BindKey->FunctionList[Type].emplace_back(BindFunc);
	}

private:
	unsigned char ConvertKey(unsigned char Key);

};

