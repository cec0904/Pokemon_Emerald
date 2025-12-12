#pragma once
#include "../Widget.h"

class CImage :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CImage();
	virtual ~CImage();

protected:
	//출력용 브러시
	FUIBrush mBrush;

public:
	//텍스쳐 셋팅
	//Name으로 찾아와서 텍스쳐를 넣어주는 바업
	void SetTexture(const string& Name);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void SetTexture(const string& Name, const TCHAR* FileName);
	//셋팅 바로 해주기 
	void SetTexture(class CTexture* Texture);

	//색상 셋팅
	void SetTint(float r, float g, float b, float a = 1.f);


	// 애니메이션 사용 유무
	//AnimationEnable
	void SetBrushAnimation(bool Animation);

	//애니메이션 프레임
	void AddBurshFrame(const FVector2D& Start, const FVector2D& Size);
	void AddBurshFrame(float StartX, float StartY, float SizeX, float SizeY);

	// 출력할 프레임
	void SetCurrentFrame(int Frame);
	// 프레임 재생 시간
	void SetAnimationPlayTime(float PlayTime);
	// 재생 속도
	void SetAnimationPlayRate(float PlayRate);


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);

};

