 #include "SpriteEffect.h"
#include "../Component/SpriteComponent.h"
#include "../Animation/Animation2D.h"
#include "../Component/AudioComponent.h"

CSpriteEffect::CSpriteEffect()
{
}

CSpriteEffect::CSpriteEffect(const CSpriteEffect& Obj)
	: CSceneObject(Obj)
{
}

CSpriteEffect::CSpriteEffect(CSpriteEffect&& Obj)
	: CSceneObject(Obj)

{
}

CSpriteEffect::~CSpriteEffect()
{
}

bool CSpriteEffect::Init()
{
	CSceneObject::Init();

	mRoot = CreateComponent<CSpriteComponent>();
	mAudio = CreateComponent<CAudioComponent>();

	mRoot->SetPivot(0.5f, 0.5f);
	SetRootComponent(mRoot);

	mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();

	mAudio->SetSound("Hit");
	mRoot->AddChild(mAudio);

	return true;
}

void CSpriteEffect::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
}

void CSpriteEffect::AnimationEnd()
{
	if (!mLoop)
	{
		Destroy();
	}
}

void CSpriteEffect::SetAnimation(const string& Name, float PlayTime, float PlayRate, bool Loop, bool Reverse)
{
	mLoop = Loop;

	mAnimation->AddSequence(Name, PlayTime, PlayRate, Loop, Reverse);
	mAnimation->SetEndFunction<CSpriteEffect>(Name, this, &CSpriteEffect::AnimationEnd);
}
