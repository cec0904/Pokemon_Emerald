#include "SoundManager.h"

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
	auto iter = mChannelGroupMap.begin();
	auto iterEnd = mChannelGroupMap.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->release();
	}

	if (mSystem)
	{
		mSystem->release();
		mSystem->close();
	}
}

bool CSoundManager::Init()
{
	//FMOD::System 객체를 생성하면된다.
	 FMOD_RESULT result = FMOD::System_Create(&mSystem);

	if (result != FMOD_OK)
	{
		return false;
	}

	//시스템 초기화
	result =  mSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
	{
		return false;
	}

	//그룹 만들어 주기
	//마스터 그룹
	result = mSystem->getMasterChannelGroup(&mMasterGroup);
	if (result != FMOD_OK)
	{
		return false;
	}

	//맵에 등록해주기 
	mChannelGroupMap.insert(make_pair("Master", mMasterGroup));

	//하위 그룹도 등록해주기 
	CreateChannelGroup("BGM");
	CreateChannelGroup("Effect");
	CreateChannelGroup("UI");

	//사운드 불러오기
	LoadSound("StartBGM", "BGM", true, "Texture/Pokemon/Sound/TitleTheme.mp3");

	LoadSound("LittlerootTown", "BGM", true, "Texture/Pokemon/Sound/LittlerootTown.mp3");
	LoadSound("PetalburgCity", "BGM", true, "Texture/Pokemon/Sound/PetalburgCity.mp3");
	LoadSound("PokemonCenter", "BGM", true, "Texture/Pokemon/Sound/PokemonCenter.mp3");


	LoadSound("Route1", "BGM", true, "Texture/Pokemon/Sound/Route1.mp3");
	LoadSound("Route2", "BGM", true, "Texture/Pokemon/Sound/Route2.mp3");

	LoadSound("WildPokemonBattle", "BGM", true, "Texture/Pokemon/Sound/WildPokemonBattle.mp3");
	LoadSound("WildPokemonDefeated", "BGM", true, "Texture/Pokemon/Sound/WildPokemonDefeated.mp3");
	LoadSound("LegendaryPokemonBattle", "BGM", true, "Texture/Pokemon/Sound/LegendaryPokemonBattle.mp3");

	LoadSound("HighJumpKick", "Effect", false, "Texture/Pokemon/Sound/HighJumpKick.mp3");
	LoadSound("Fly", "Effect", false, "Texture/Pokemon/Sound/Fly.mp3");
	LoadSound("LevelUp", "Effect", false, "Texture/Pokemon/Sound/LevelUp.mp3");


	LoadSound("1", "Effect", false, "Texture/Pokemon/Sound/Pokemon/1.mp3");
	LoadSound("2", "Effect", false, "Texture/Pokemon/Sound/Pokemon/2.mp3");
	LoadSound("3", "Effect", false, "Texture/Pokemon/Sound/Pokemon/3.mp3");
	LoadSound("4", "Effect", false, "Texture/Pokemon/Sound/Pokemon/4.mp3");
	LoadSound("5", "Effect", false, "Texture/Pokemon/Sound/Pokemon/5.mp3");
	LoadSound("6", "Effect", false, "Texture/Pokemon/Sound/Pokemon/6.mp3");
	LoadSound("7", "Effect", false, "Texture/Pokemon/Sound/Pokemon/7.mp3");
	LoadSound("8", "Effect", false, "Texture/Pokemon/Sound/Pokemon/8.mp3");
	LoadSound("9", "Effect", false, "Texture/Pokemon/Sound/Pokemon/9.mp3");
	LoadSound("10", "Effect", false, "Texture/Pokemon/Sound/Pokemon/10.mp3");
	LoadSound("11", "Effect", false, "Texture/Pokemon/Sound/Pokemon/11.mp3");
	LoadSound("12", "Effect", false, "Texture/Pokemon/Sound/Pokemon/12.mp3");
	LoadSound("13", "Effect", false, "Texture/Pokemon/Sound/Pokemon/13.mp3");
	LoadSound("14", "Effect", false, "Texture/Pokemon/Sound/Pokemon/14.mp3");
	LoadSound("15", "Effect", false, "Texture/Pokemon/Sound/Pokemon/15.mp3");
	LoadSound("16", "Effect", false, "Texture/Pokemon/Sound/Pokemon/16.mp3");
	LoadSound("17", "Effect", false, "Texture/Pokemon/Sound/Pokemon/17.mp3");
	LoadSound("18", "Effect", false, "Texture/Pokemon/Sound/Pokemon/18.mp3");
	LoadSound("19", "Effect", false, "Texture/Pokemon/Sound/Pokemon/19.mp3");
	LoadSound("20", "Effect", false, "Texture/Pokemon/Sound/Pokemon/20.mp3");
	LoadSound("21", "Effect", false, "Texture/Pokemon/Sound/Pokemon/21.mp3");
	LoadSound("22", "Effect", false, "Texture/Pokemon/Sound/Pokemon/22.mp3");
	LoadSound("23", "Effect", false, "Texture/Pokemon/Sound/Pokemon/23.mp3");
	LoadSound("24", "Effect", false, "Texture/Pokemon/Sound/Pokemon/24.mp3");
	LoadSound("25", "Effect", false, "Texture/Pokemon/Sound/Pokemon/25.mp3");
	LoadSound("26", "Effect", false, "Texture/Pokemon/Sound/Pokemon/26.mp3");
	LoadSound("27", "Effect", false, "Texture/Pokemon/Sound/Pokemon/27.mp3");
	LoadSound("28", "Effect", false, "Texture/Pokemon/Sound/Pokemon/28.mp3");

	LoadSound("Button", "Effect", false, "Texture/Pokemon/Sound/Button.mp3");





	LoadSound("Effect1", "Effect", false, "Sound/TeemoSmile.mp3");

	//사운드 플레이
	//Play("SceneMainBGM");
	//Play("Effect1");

	SetGroupVolume("BGM", 100);
	SetGroupVolume("Effect", 50);
	
	return true;
}

void CSoundManager::Update()
{
	// 오디오 시스템을 갱신해준다. 매 프레임마다 실행 해줘야한다. 
	mSystem->update();
}

bool CSoundManager::CreateChannelGroup(const string& Name)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(Name);

	if (Group)
	{
		return true;
	}

	FMOD_RESULT result =  mSystem->createChannelGroup(Name.c_str(), &Group);

	if (result != FMOD_OK)
	{
		return false;
	}

	mMasterGroup->addGroup(Group, false);

	mChannelGroupMap.insert(make_pair(Name, Group));

	return true;
}

bool CSoundManager::LoadSound(const string& Name, const string& GroupName, bool Loop, const char* FileName)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
	{
		return true;
	}

	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	//만약 지정한 그룹이 없으면 Master그룹을 지정해준다. 
	if (!Group)
	{
		Group = mMasterGroup;
	}

	Sound = new CSound;

	if (!Sound->LoadSound(mSystem, Group, Loop, FileName))
	{
		SAFE_DELETE(Sound);
		return false;
	}

	mSoundMap.insert(make_pair(Name, Sound));

	return true;
}

bool CSoundManager::LoadSoundFullPath(const string& Name, const string& GroupName, bool Loop,
	const char* FullPath)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
	{
		return true;
	}

	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	//만약 지정한 그룹이 없으면 Master그룹을 지정해준다. 
	if (!Group)
	{
		Group = mMasterGroup;
	}

	Sound = new CSound;

	if (!Sound->LoadSoundFullPath(mSystem, Group, Loop, FullPath))
	{
		SAFE_DELETE(Sound);
		return false;
	}

	mSoundMap.insert(make_pair(Name, Sound));

	return true;
}

void CSoundManager::Play(const string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
	{
		return;
	}

	Sound->Play();
}

void CSoundManager::Stop(const string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
	{
		return;
	}

	Sound->Stop();
}

void CSoundManager::Pause(const string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
	{
		return;
	}

	Sound->Pause();
}

void CSoundManager::Resume(const string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
	{
		return;
	}

	Sound->Resume();
}

void CSoundManager::SetMasterVolume(int Volume)
{
	// int값이 몇이들어오든
	// 0 ~ 100
	// Volume > 100 -> 100 | 그 사이값은 그대로 사용 | Volume < 0 -> 0
	// Clamp
	Volume = Clamp(Volume, 0, 100);
	mMasterGroup->setVolume(static_cast<float>(Volume)/ 100.f);
}

void CSoundManager::SetGroupVolume(const string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
	{
		return;
	}

	Volume = Clamp(Volume, 0, 100);

	Group->setVolume(static_cast<float>(Volume) / 100.f);
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const string& Name)
{
	auto iter = mChannelGroupMap.find(Name);

	if (iter == mChannelGroupMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CSoundManager::ReleaseSound(class CAsset* Sound)
{
	auto iter = mSoundMap.find(Sound->GetName());

	if (iter != mSoundMap.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mSoundMap.erase(iter);
		}
	}
}

CSound* CSoundManager::FindSound(const string& Name)
{
	auto iter = mSoundMap.find(Name);

	if (iter == mSoundMap.end())
	{
		return nullptr;
	}

	return iter->second;
}
