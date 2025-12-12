#pragma once
#include "../Widget.h"

enum class EProgressBarImageType
{
	Back,
	Fill
};


// 바가 작아지는 방향을 
enum class EProgressBarDir
{
	RightToLeft,
	TopToBottom
};

class CProgressBar :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CProgressBar();
	virtual ~CProgressBar();

protected:
	//출력용 브러시
	//백그라운드
	FUIBrush mBackBrush;
	//게이지용도
	FUIBrush mFillBrush;
	//바가 줄어드는 방향
	EProgressBarDir mBarDir = EProgressBarDir::RightToLeft;
	float mPercent = 1.f;


public:
	//텍스쳐 셋팅
	//Name으로 찾아와서 텍스쳐를 넣어주는 바업
	void SetTexture(EProgressBarImageType Type, const string& Name);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void SetTexture(EProgressBarImageType Type, const string& Name, const TCHAR* FileName);
	//셋팅 바로 해주기 
	void SetTexture(EProgressBarImageType Type, class CTexture* Texture);

	//색상 셋팅
	void SetTint(EProgressBarImageType Type, float r, float g, float b);
	//투명도
	void SetOpacity(EProgressBarImageType Type, float Opacity);

	// 애니메이션 사용 유무
	//AnimationEnable
	void SetBrushAnimation(EProgressBarImageType Type,  bool Animation);

	//애니메이션 프레임
	void AddBurshFrame(EProgressBarImageType Type, const FVector2D& Start, const FVector2D& Size);
	void AddBurshFrame(EProgressBarImageType Type, float StartX, float StartY, float SizeX, float SizeY);

	// 출력할 프레임
	void SetCurrentFrame(EProgressBarImageType Type, int Frame);
	// 프레임 재생 시간
	void SetAnimationPlayTime(EProgressBarImageType Type, float PlayTime);
	// 재생 속도
	void SetAnimationPlayRate(EProgressBarImageType Type, float PlayRate);

	//progressBar 고유
	void SetPercent(float Percent);
	void SetBarDir(EProgressBarDir Dir);

public:
	float GetPercent() const
	{
		return mPercent;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);

private:
	void UpdateBrush(FUIBrush& Brush, float DeltaTime);
	void RenderBrush(FUIBrush& Brush, const FVector2D& Size);
	void RenderBrush(FUIBrush& Brush, const FVector2D& Pos, const FVector2D& Size);
};

