#pragma once

#include "Sound.h"

class CSoundManager
{
	friend class CAssetManager;

private:
	CSoundManager();
	~CSoundManager();

private:
	//1
	// 사운드 관련 관리자 
	FMOD::System* mSystem = nullptr;
	//채널 최상위 그룹 
	FMOD::ChannelGroup* mMasterGroup = nullptr;
	//채널 하위그룹
	unordered_map<string, FMOD::ChannelGroup*> mChannelGroupMap;
	//사운드 종류 
	unordered_map<string, CSharedPtr<CSound>> mSoundMap;

public:
	bool Init();
	void Update();

	bool CreateChannelGroup(const string& Name);

	//사운드 불러오기
	bool LoadSound(const string& Name, const string& GroupName, bool Loop, const char* FileName);
	bool LoadSoundFullPath(const string& Name, const string& GroupName, bool Loop, const char* FullPath);

	//기능
	void Play(const string& Name);
	void Stop(const string& Name);
	void Pause(const string& Name);
	void Resume(const string& Name);

	void SetMasterVolume(int Volume);
	void SetGroupVolume(const string& GroupName, int Volume);



private:
	FMOD::ChannelGroup* FindChannelGroup(const string& Name);

public:
	void ReleaseSound(class CAsset* Sound);

	CSound* FindSound(const string& Name);


};

