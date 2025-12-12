#pragma once
#include "../Asset.h"

// FMOD 헤더파일
#include "fmod.hpp"


class CSound :
    public CAsset
{
    friend class CSoundManager;

protected:
    CSound();
    virtual ~CSound();

protected:
    FMOD::System* mSystem = nullptr;
    //사운드가 포함된 채널
    FMOD::ChannelGroup* mGroup = nullptr;
    // 어떤 사운드가 재생되고있는지 저장한다. 
    FMOD::Channel* mChannel = nullptr;
    // 실제 사운드 데이터
    FMOD::Sound* mSound = nullptr;

    bool mLoop = false;
    //동시에 사운드가 몇개 플레이 되고 있는가 
    int mPlayCount  = 0;

public:
    //파일명 
    bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop, const char* FileName);
    bool LoadSoundFullPath(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop, const char* FullPath);

    // 실행
    void Play();
    // 스탑
    void Stop();
    // 일시정지
    void Pause();
    // 재시작
    void Resume();

    static FMOD_RESULT SoundEnd(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);

};

