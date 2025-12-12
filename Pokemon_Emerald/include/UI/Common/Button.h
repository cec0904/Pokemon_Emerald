#pragma once
#include "../Widget.h"

//버튼 상태값
namespace EButtonState
{
	enum Type
	{
		Normal,
		Hovered,
		Click,
		Disable,
		End
	};
}

//버튼 이벤트 상태값
namespace  EButtonEventState
{
	enum Type
	{
		Hovered,
		Click,
		End
	};
}

class CButton :
	public CWidget
{
	friend class CSceneUIManager;

protected:
	CButton();
	virtual ~CButton();

protected:
	// 버튼은 하나의 자식만 가질수 있게 한다.
	// 버튼안에 텍스트나 이미지를 출력해서 어떤 버튼인지 알게한다.
	CSharedPtr<CWidget> mChild;
	// 버튼 이미지 정보
	FUIBrush mBrush[EButtonState::End];
	//현재 버튼 상태
	EButtonState::Type mState = EButtonState::Normal;
	// 버튼 색상
	FVector4D mColor = FVector4D::White;
	// 상태값에 따라서 사운드를 출력한다.
	CSharedPtr<class CSound> mSound[EButtonEventState::End];
	//버튼 상태값에 따라서 함수를 호출해준다. 
	function<void()> mEventCallBack[EButtonEventState::End];

public:
	void SetChild(CWidget* Child)
	{
		mChild = Child;
	}

	void ButtonEnable(bool Enable)
	{
		mState = Enable ? EButtonState::Normal : EButtonState::Disable;
	}

	void SetColor(const FVector4D& Color)
	{
		mColor = Color;
	}

	void SetColor(float r, float g, float b, float a)
	{
		mColor = FVector4D(r, g, b, a);
	}

	void SetColor(float r, float g, float b)
	{
		mColor.x = r;
		mColor.y = g;
		mColor.z = b;
	}

	void SetOpacity(float Opacity)
	{
		mColor.w = Opacity;
	}


public:
	//브러시 셋팅
	//텍스쳐 셋팅 
	
	//텍스쳐 셋팅
	//Name으로 찾아와서 텍스쳐를 넣어주는 바업
	void SetTexture(EButtonState::Type State,const string& Name);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void SetTexture(EButtonState::Type State, const string& Name, const TCHAR* FileName);
	//셋팅 바로 해주기 
	void SetTexture(EButtonState::Type State, class CTexture* Texture);


	//색상 셋팅
	void SetTint(EButtonState::Type State, float r, float g, float b, float a = 1.f);

	//애니메이션 프레임
	void AddBurshFrame(EButtonState::Type State, const FVector2D& Start, const FVector2D& Size);
	void AddBurshFrame(EButtonState::Type State, float StartX, float StartY, float SizeX, float SizeY);


	// 애니메이션 사용 유무
	//AnimationEnable
	void SetBrushAnimation(EButtonState::Type State, bool Animation);
	// 출력할 프레임
	void SetCurrentFrame(EButtonState::Type State, int Frame);
	// 프레임 재생 시간
	void SetAnimationPlayTime(EButtonState::Type State, float PlayTime);
	// 재생 속도
	void SetAnimationPlayRate(EButtonState::Type State, float PlayRate);

	//사운드
	void SetSound(EButtonState::Type State, const string& Name);
	void SetSound(EButtonState::Type State, const string& Name, const char* FileName);

	//시점 
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);

	virtual void MouseHovered();
	virtual void MouseUnHovered();

public:
	template<typename T>
	void SetEventCallBack(EButtonEventState::Type State, T* Obj, void(T::* Func)())
	{
		mEventCallBack[State] = bind(Func, Obj);
	}


private:
	void ButtonClick();
};

