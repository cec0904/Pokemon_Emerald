#include "Input.h"

#include "CameraManager.h"
#include "Scene.h"
#include "../GameManager.h"
#include "../Device.h"
#include "../Share/Log.h"

CInput::CInput()
{
}

CInput::~CInput()
{
	auto iter = mKeyStateMap.begin();
	auto iterEnd = mKeyStateMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	
	auto iter1 = mBindKeyMap.begin();
	auto iterEnd1 = mBindKeyMap.end();

	for (; iter1 != iterEnd1; ++iter1)
	{
		SAFE_DELETE(iter1->second);
	}

	if (mKeyboard)
	{
		mKeyboard->Unacquire();
		SAFE_RELEASE(mKeyboard);
	}

	if (mMouse)
	{
		mMouse->Unacquire();
		SAFE_RELEASE(mMouse);
	}


}

//AddBindKey("MoveUp", 'W');
void CInput::AddBindKey(const string& Name, unsigned char Key)
{
	if (FindBindKey(Name))
		return;

	FBindKey* NewKey = new FBindKey;

	FKeyState* State = FindKeyState(Key);

	if (!State)
	{
		State = new FKeyState;
		State->Key = ConvertKey(Key);
		mKeyStateMap.insert(make_pair(Key, State));
	}

	NewKey->Key = State;
	mBindKeyMap.insert(make_pair(Name, NewKey));
}

void CInput::ChangeKeyCtrl(const string& Name, bool Ctrl)
{
	FBindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Ctrl = Ctrl;
}

void CInput::ChangeKeyAlt(const string& Name, bool Alt)
{
	FBindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Alt = Alt;
}

void CInput::ChangeKeyShift(const string& Name, bool Shift)
{
	FBindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Shift = Shift;
}

bool CInput::InitInput()
{
	//키보드 
	//디바이스 객체 만든거고 
	//키보드 장치를 얻어온건 아니다!
	if (FAILED(mInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, nullptr)))
	{
		return false;
	}

	if (FAILED(mKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	//창모드냐 아니냐에 따라서 기본 설정값이 달라집니다. 
	//DISCL_FOREGROUND		: 활설화 시에만 키를 입력받게 한다. 
	//DISCL_BACKGROUND		: 백그라운드에서도 사용할 수 있게 설정한다.
	//DISCL_EXCLUSIVE		: 입력 디바이스를 독점모드로 설정한다. 
	//DISCL_NONEXCLUSIVE	: 입력 디바이스를 비독점 모드로설정한다. 
	if (CDevice::GetInst()->GetWindowMod())	//창모드
	{
		if(FAILED(mKeyboard->SetCooperativeLevel(mhWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		{
			return false;
		}
	}
	else
	{
		if (FAILED(mKeyboard->SetCooperativeLevel(mhWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			return false;
		}
	}

	//키보드 장치를 획득한다. 
	if (FAILED(mKeyboard->Acquire()))
	{
		return false;
	}

	/////////////마우스 
	if (FAILED(mInput->CreateDevice(GUID_SysMouse, &mMouse, nullptr)))
	{
		return false;
	}

	if (FAILED(mMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}

	if (CDevice::GetInst()->GetWindowMod())	//창모드
	{
		if (FAILED(mMouse->SetCooperativeLevel(mhWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		{
			return false;
		}
	}
	else
	{
		if (FAILED(mMouse->SetCooperativeLevel(mhWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			return false;
		}
	}

	//마우스 장치를 획득한다. 
	if (FAILED(mMouse->Acquire()))
	{
		return false;
	}



	return true;
}

bool CInput::UpdateKeyboard()
{
	if (!mKeyboard)
		return false;

	// 키보드 상태를 만들어 준다.
	HRESULT result = mKeyboard->GetDeviceState(256, (LPVOID) & mKeyState);

	if (FAILED(result))
	{
		//장치를 잃었거나 획득하지 못한경우 
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			// 재획득 시도
			mKeyboard->Acquire();
		}
	}
	return true;
}

bool CInput::UpdateMouse()
{
	if (!mMouse)
		return false;
	HRESULT result = mMouse->GetDeviceState(sizeof(mMouseState), (LPVOID)&mMouseState);

	if (FAILED(result))
	{
		//장치를 잃었거나 획득하지 못한경우 
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			// 재획득 시도
			mMouse->Acquire();
		}
	}
	return true;
}

FKeyState* CInput::FindKeyState(unsigned char Key)
{
	auto iter = mKeyStateMap.find(Key);

	if (iter == mKeyStateMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

FBindKey* CInput::FindBindKey(const string& Name)
{
	auto iter = mBindKeyMap.find(Name);

	if (iter == mBindKeyMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool CInput::Init()
{
	//입력장치 초기화하는데 
	// 윈도우의 Inst, hWnd 이 필요하다. 
	mhInst = CGameManager::GetInst()->GetWindowInstance();
	mhWnd = CGameManager::GetInst()->GetWindowHandle();

	if (FAILED(DirectInput8Create(mhInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mInput, nullptr)))
	{
		//다이렉트의 입력을 초기화 못하면 
		//윈도우의 입력을 사용하겠다. 
		mInputType = EInputSystem_Type::Window;
	}

	if (mInputType == EInputSystem_Type::DInput)
	{
		//다이렉트 인풋이 사용 가능하면 장치를 여기서 얻어올것이다. 
		if (!InitInput())
		{
			return false;
		}
	}

	return true;
}
void CInput::Update(float DeltaTime)
{
	if (mInputType == EInputSystem_Type::DInput)
	{
		//키보드 업데이트 
		UpdateKeyboard();
		//마우스 업데이트 
		UpdateMouse();
	}

	UpdateMousePos(DeltaTime);

	UpdateInput(DeltaTime);
	UpdateBind(DeltaTime);
}

void CInput::UpdateMousePos(float DeltaTime)
{
	//여기서는 마우스의 좌표를 얻어올것이다.

	//윈도우 창에서의 마우스 위치를 구한다.
	POINT MousePT;
	// GetCursorPos : 모니터 화면(스크린) 좌표를 구해준다. 
	GetCursorPos(&MousePT);

	// 스크린 좌표를 클라이언트 좌표로 변환시켜준다.
	// 기준은 좌상단.
	ScreenToClient(mhWnd, &MousePT);

	// 디바이스 비율
	FVector2D Ratio = CDevice::GetInst()->GetResolutionRatio();
	FResolution ViewportRS = CDevice::GetInst()->GetResolution();

	FVector2D MousePos;

	// 윈도우창에서의 마우스 좌표를 해상도 비율로 곱해서 인게임 Dx 해상도 상에서의 위치를 구해준다. 
	MousePos.x = MousePT.x * Ratio.x;
	MousePos.y = MousePT.y * Ratio.y;

	// 해준 이유 : 윈도우는 Y좌표가 아래가 +
	// DX에서는 Y좌표가 위가 + 이므로
	// Y좌표는 반전을 시켜줘야한다. 
	// 뷰포트 해상도를 이용해서 Y 좌표 반전 
	MousePos.y = ViewportRS.Height - MousePos.y;

	//프로그램 시작 첫프레임
	if (mMouseCompute)
	{
		// 마우스 이동량 = 지금 마우스위치 - 이전프레임 마우스위치 
		mMouseMove = MousePos - mMousePos;
	}
	else
	{
		// 첫번째 프레임은 아직 mMousePos 저장 안되어있기 때문에 
		mMouseCompute = true;
	}

	// 화면에서의 마우스 좌표를 구했고 
	mMousePos = MousePos;



	// 카메라 기준으로 월드 위치를 계산해줘야한다. 
	FVector3D WorldPos = mScene->GetCameraManager()->GetCameraWorldPos();

	// 카메라의 위치 - 사이즈 * 0.5 -> 왼쪽 상단 으로 이동
	// 왼쪽상단 기준으로 마우스 위치만큼 구하면 -> 카메라 기준에서 마우스 좌표의 월드위치

	mMouseWorldPos2D.x = WorldPos.x - ViewportRS.Width * 0.5f + mMousePos.x;
	mMouseWorldPos2D.y = WorldPos.y - ViewportRS.Height * 0.5f + mMousePos.y;

}

void CInput::UpdateInput(float DeltaTime)
{
	switch (mInputType)
	{
	case EInputSystem_Type::DInput:
		//DIK_LCONTROL 
		if (mKeyState[DIK_LCONTROL] & 0x80)
		{
			//왼쪽 컨트롤키 눌림 
			if (!mCtrl[EInputType::Down] && !mCtrl[EInputType::Hold])
			{
				mCtrl[EInputType::Down] = true;
				mCtrl[EInputType::Hold] = true;
			}
			else
			{
				mCtrl[EInputType::Down] = false;
			}
		}
		else if (mCtrl[EInputType::Hold])
		{
			mCtrl[EInputType::Hold] = false;
			mCtrl[EInputType::Down] = false;
			mCtrl[EInputType::Up] = true;
		}
		else if (mCtrl[EInputType::Up])
		{
			mCtrl[EInputType::Up] = false;
		}

		//ALT
		if (mKeyState[DIK_LALT] & 0x80)
		{
			//왼쪽 컨트롤키 눌림 
			if (!mAlt[EInputType::Down] && !mAlt[EInputType::Hold])
			{
				mAlt[EInputType::Down] = true;
				mAlt[EInputType::Hold] = true;
			}
			else
			{
				mAlt[EInputType::Down] = false;
			}
		}
		else if (mAlt[EInputType::Hold])
		{
			mAlt[EInputType::Hold] = false;
			mAlt[EInputType::Down] = false;
			mAlt[EInputType::Up] = true;
		}
		else if (mAlt[EInputType::Up])
		{
			mAlt[EInputType::Up] = false;
		}

		//Shift
		if (mKeyState[DIK_LSHIFT] & 0x80)
		{
			//왼쪽 컨트롤키 눌림 
			if (!mShift[EInputType::Down] && !mShift[EInputType::Hold])
			{
				mShift[EInputType::Down] = true;
				mShift[EInputType::Hold] = true;
			}
			else
			{
				mShift[EInputType::Down] = false;
			}
		}
		else if (mShift[EInputType::Hold])
		{
			mShift[EInputType::Hold] = false;
			mShift[EInputType::Down] = false;
			mShift[EInputType::Up] = true;
		}
		else if (mShift[EInputType::Up])
		{
			mShift[EInputType::Up] = false;
		}


		//마우스 
		// 눌림여부 
		// 처음 눌렸는지
		// 눌리고있는지 
		// 때졌는지 
		for (int i = 0; i < EMouseButtonType::End; ++i)
		{
			//마우스 클릭 
			if (mMouseState.rgbButtons[i] & 0x80)	//클릭이 된경우 
			{
				//마우스눌린상태를 저장할 것이다. 
				// Down이 False경우면 처음 버튼이 눌린 경우다. 
				if (!mMouseDown[i] && !mMouseHold[i])
				{
					mMouseDown[i] = true;
					mMouseHold[i] = true;
				}
				else
				{
					// 이전 프레임에도 눌렸고 지금도 누르고 있는 경우 
					mMouseDown[i] = false;
				}
			}

			// 키가 누르지 않았지만 Hold가 True 인경우 누르고 있다가 때는 경우이다. 
			else if (mMouseHold[i])
			{
				mMouseDown[i] = false;
				mMouseHold[i] = false;
				mMouseUp[i] = true;
			}
			else if (mMouseUp[i])
			{
				mMouseUp[i] = false;
			}
		}

		

		break;
	case EInputSystem_Type::Window:
		break;
	}

	//키입력 체크 
	auto iter = mKeyStateMap.begin();
	auto iterEnd = mKeyStateMap.end();

	for (; iter != iterEnd; ++iter)
	{
		bool KeyPush = false;

		switch (iter->second->Key)
		{
		case DIK_MOUSELBUTTON:
			if (mMouseState.rgbButtons[EMouseButtonType::LButton] & 0x80)
				KeyPush = true;
			break;
		case DIK_MOUSERBUTTON:
			if (mMouseState.rgbButtons[EMouseButtonType::RButton] & 0x80)
				KeyPush = true;
			break;
		case DIK_MOUSEWHEEL:
			if (mMouseState.rgbButtons[EMouseButtonType::MButton] & 0x80)
				KeyPush = true;
			break;
		default:
			if (mKeyState[iter->second->Key] & 0x80)
				KeyPush = true;
			break;
		}

		//키가 눌렸다
		if (KeyPush)
		{
			if (!iter->second->Down && !iter->second->Hold)
			{
				iter->second->Down = true;
				iter->second->Hold = true;
			}
			else
			{
				iter->second->Down = false;
			}
		}

		else if (iter->second->Hold)
		{
			iter->second->Down = false;
			iter->second->Hold = false;
			iter->second->Up = true;

		}
		else if (iter->second->Up)
		{
			iter->second->Up = false;
		}
	}
}

void CInput::UpdateBind(float DeltaTime)
{
	auto iter = mBindKeyMap.begin();
	auto iterEnd = mBindKeyMap.end();

	for (; iter != iterEnd; ++iter)
	{
		//Down이 충족됬는지
		if (iter->second->Key->Down &&
			iter->second->Ctrl == mCtrl[EInputType::Hold] &&
			iter->second->Alt == mAlt[EInputType::Hold] &&
			iter->second->Shift == mShift[EInputType::Hold])
		{
			size_t Size = iter->second->FunctionList[EInputType::Down].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->FunctionList[EInputType::Down][i].Func(DeltaTime);
			}
		}

		//Hold 충족됬는지
		if (iter->second->Key->Hold &&
			iter->second->Ctrl == mCtrl[EInputType::Hold] &&
			iter->second->Alt == mAlt[EInputType::Hold] &&
			iter->second->Shift == mShift[EInputType::Hold])
		{
			iter->second->KeyHold = true;
			size_t Size = iter->second->FunctionList[EInputType::Hold].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->FunctionList[EInputType::Hold][i].Func(DeltaTime);
			}
		}

		//원하는 조합키가 눌렸는지 여부를 검사한다. 
		bool Verification = false;

		if (iter->second->Ctrl)
		{
			if (mCtrl[EInputType::Up])
			{
				Verification = true;
			}
		}

		if (iter->second->Alt)
		{
			if (mAlt[EInputType::Up])
			{
				Verification = true;
			}
		}

		if (iter->second->Shift)
		{
			if (mShift[EInputType::Up])
			{
				Verification = true;
			}
		}

		//Up이 충족됬는지 확인할것이다.
		if ((iter->second->Key->Up || Verification) && iter->second->KeyHold)
		{
			iter->second->KeyHold = false;

			size_t Size = iter->second->FunctionList[EInputType::Up].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->FunctionList[EInputType::Up][i].Func(DeltaTime);
			}
		}
	}
}

unsigned char CInput::ConvertKey(unsigned char Key)
{
	/*
		등록은 윈도우 기반 VK 비트로 받고 
		사용은 DIK 다이렉트 인풋 기분으로 변경 해준다. 
	*/


	if (mInputType == EInputSystem_Type::DInput)
	{
		switch (Key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			//return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}

		return 0xff;
	}

	return Key;
}
