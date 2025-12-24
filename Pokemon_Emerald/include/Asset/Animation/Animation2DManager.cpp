#include "Animation2DManager.h"
#include "Animation2DData.h"
#include "../../Animation/Animation2D.h"
#include "../../Share/Log.h"

CAnimation2DManager::CAnimation2DManager()
{
}

CAnimation2DManager::~CAnimation2DManager()
{
	//
	CAnimation2D::DestroyCBuffer();
}

bool CAnimation2DManager::Init()
{
	//애니메이션용 상수 버퍼 생성
	CAnimation2D::CreateCBuffer();


	// 애니메이션 하나 등록하기 
	// Texture/Player/Player.png
	//애니메이션을 하나 등록하였다.
	/*CreateAnimation("PlayerIdle");
	SetAnimationTextureType("PlayerIdle", EAnimationTextureType::SpriteSheet);
	SetTexture("PlayerIdle", "PlayerSprite", TEXT("Texture/Player/Player.png"));

	for (int i =0 ; i < 5; ++i)
	{
		AddFrame("PlayerIdle", i * 200.f, 0.f, 200.f, 200.f);
	}*/

	// 플레이어 위로 걷기
	// May(또는 Player) 스프라이트시트
	CreateAnimation("PlayerIdleUp");
	SetAnimationTextureType("PlayerIdleUp", EAnimationTextureType::SpriteSheet);
	SetTexture("PlayerIdleUp", "PlayerSheet", TEXT("Texture\\Pokemon\\Player\\May.png"));
	// Idle = col 2, Down = row 0
	AddFrame("PlayerIdleUp", 0.f, 33.f, 16.f, 32.f);

	// 걷기 Up (col 0,1)
	CreateAnimation("PlayerWalkUp");
	SetAnimationTextureType("PlayerWalkUp", EAnimationTextureType::SpriteSheet);
	SetTexture("PlayerWalkUp", "PlayerSheet", TEXT("Texture\\Pokemon\\Player\\May.png"));
	AddFrame("PlayerWalkUp", 17.f, 33.f, 16.f, 32.f);
	AddFrame("PlayerWalkUp", 34.f, 33.f, 16.f, 32.f);



	// 플레이어 아래로 걷기
	// May(또는 Player) 스프라이트시트
	CreateAnimation("PlayerIdleDown");
	SetAnimationTextureType("PlayerIdleDown", EAnimationTextureType::SpriteSheet);
	SetTexture("PlayerIdleDown", "PlayerSheet", TEXT("Texture\\Pokemon\\Player\\May.png"));
	// Idle = col 2, Down = row 0
	AddFrame("PlayerIdleDown", 0.f, 0.f, 16.f, 32.f);

	// 걷기 Down (col 0,1)
	CreateAnimation("PlayerWalkDown");
	SetAnimationTextureType("PlayerWalkDown", EAnimationTextureType::SpriteSheet);
	SetTexture("PlayerWalkDown", "PlayerSheet", TEXT("Texture\\Pokemon\\Player\\May.png"));
	AddFrame("PlayerWalkDown", 17.f, 0.f, 16.f, 32.f);
	AddFrame("PlayerWalkDown", 34.f, 0.f, 16.f, 32.f);


	// 플레이어 오른쪽으로 걷기
	CreateAnimation("PlayerIdleLeft");
	SetAnimationTextureType("PlayerIdleLeft", EAnimationTextureType::SpriteSheet);
	SetTexture("PlayerIdleLeft", "PlayerSheet", TEXT("Texture\\Pokemon\\Player\\May.png"));

	AddFrame("PlayerIdleLeft", 0.f, 66.f, 16.f, 32.f);

	// 걷기 Left (col 0,1)
	CreateAnimation("PlayerWalkLeft");
	SetAnimationTextureType("PlayerWalkLeft", EAnimationTextureType::SpriteSheet);
	SetTexture("PlayerWalkLeft", "PlayerSheet", TEXT("Texture\\Pokemon\\Player\\May.png"));
	AddFrame("PlayerWalkLeft", 17.f, 66.f, 16.f, 32.f);
	AddFrame("PlayerWalkLeft", 34.f, 66.f, 16.f, 32.f);





	//폭발 이펙트
	vector<const TCHAR*> FileNames;
	CreateAnimation("Explosion");
	SetAnimationTextureType("Explosion", EAnimationTextureType::Frame);
	FileNames.clear();

	TCHAR Path[89][MAX_PATH] = {};

	//Texture\\Explosion\\Explosion1.png
	for (int i = 1; i <= 89; ++i)
	{
		wsprintf(Path[i - 1], TEXT("Texture\\Explosion\\Explosion%d.png"), i);
		FileNames.emplace_back(Path[i - 1]);
	}
	SetTexture("Explosion", "Explosion", FileNames);
	AddFrameCount("Explosion", 89, 0.f, 0.f, 320.f, 240.f);


	CreateAnimation("TitleBackGround");
	vector<const TCHAR*> TitleBack;
	SetAnimationTextureType("TitleBackGround", EAnimationTextureType::Frame);
	TitleBack.clear();

	static TCHAR TitlePath[160][MAX_PATH] = {};

	//Texture\\Explosion\\Explosion1.png
	for (int i = 1; i <= 160; ++i)
	{
		wsprintf(TitlePath[i - 1], TEXT("Texture\\Pokemon\\Title\\TitleBack\\frame_%04d.png"), i);
		TitleBack.emplace_back(TitlePath[i - 1]);
	}
	SetTexture("TitleBackGround", "TitleBackGround", TitleBack);
	AddFrameCount("TitleBackGround", 160, 190.f, 51.f, 256.f, 256.f);
	


	////////////////////////////////////////////


	return true;
}

bool CAnimation2DManager::CreateAnimation(const string& Name)
{
	// 생성해야하는데 같은 이름이 있으면 false
	if (FindAnimation(Name))
	{
		return false;
	}

	CAnimation2DData* Animation = new CAnimation2DData;

	Animation->SetName(Name);

	mAnimationMap.insert(make_pair(Name, Animation));

	return true;
}

class CAnimation2DData* CAnimation2DManager::FindAnimation(const string& Name)
{

	auto iter = mAnimationMap.find(Name);

	if (iter == mAnimationMap.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CAnimation2DManager::ReleaseAnimation(class CAsset* Animation)
{
	auto iter = mAnimationMap.find(Animation->GetName());

	if (iter != mAnimationMap.end())
	{
		mAnimationMap.erase(iter);
	}
}

bool CAnimation2DManager::SetAnimationTextureType(const string& Name, EAnimationTextureType Type)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetAnimationTextureType(Type);

	return true;
}

bool CAnimation2DManager::SetTexture(const string& Name, class CTexture* Texture)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetTexture(Texture);

	return true;
}

bool CAnimation2DManager::SetTexture(const string& Name, const string& TextureName)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetTexture(TextureName);

	return true;
}

bool CAnimation2DManager::SetTexture(const string& Name, const string& TextureName, const TCHAR* FileName)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetTexture(TextureName, FileName);

	return true;
}

bool CAnimation2DManager::SetTexture(const string& Name, const string& TextureName,
	vector<const TCHAR*> FileName)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetTexture(TextureName, FileName);

	return true;
}

bool CAnimation2DManager::SetTexture(const string& Name, const string& TextureName, const TCHAR* FileName,
	const TCHAR* Ext, int Count)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetTexture(TextureName, FileName, Ext, Count);

	return true;
}

bool CAnimation2DManager::SetTextureFullPath(const string& Name, const string& TextureName,
	const TCHAR* FullPath)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetTextureFullPath(TextureName, FullPath);

	return true;
}

bool CAnimation2DManager::SetTextureFullPath(const string& Name, const string& TextureName,
	const vector<const TCHAR*>& FullPath)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->SetTextureFullPath(TextureName, FullPath);

	return true;
}

//프레임 설정 
bool CAnimation2DManager::AddFrame(const string& Name, const FVector2D& Start, const FVector2D& Size)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->AddFrame(Start, Size);

	return true;
}

bool CAnimation2DManager::AddFrame(const string& Name, float StartX, float StartY, float SizeX, float SizeY)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->AddFrame(StartX, StartY, SizeX, SizeY);

	return true;
}

bool CAnimation2DManager::AddFrameCount(const string& Name, int Count, const FVector2D& Start,
	const FVector2D& Size)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->AddFrameCount(Count, Start, Size);

	return true;
}

bool CAnimation2DManager::AddFrameCount(const string& Name, int Count, float StartX, float StartY,
	float SizeX, float SizeY)
{
	CAnimation2DData* Animation = FindAnimation(Name);

	if (!Animation)
	{
		return false;
	}

	Animation->AddFrameCount(Count, StartX, StartY, SizeX, SizeY);

	return true;
}
