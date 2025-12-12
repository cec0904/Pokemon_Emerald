#pragma once
#include "../Gameinfo.h"

//UI 이미지 또는 애니메이션 정보를 가지고 있는 구조체
struct FUIBrush
{
	//이미지
	CSharedPtr<class CTexture> Texture;
	//색상
	FVector4D Tint = FVector4D::White;
	//애니메이션 정보
	vector<FAnimationFrame> Frames;
	bool	AnimationEnable = false;
	int		Frame = 0;
	float	Time = 0.f;
	float	FrameTime = 0.f;
	float	PlayTime = 1.f;
	float	PlayRate = 1.f;
};

