#pragma once

#include "Gameinfo.h"

class CProfileManager
{
	DECLARE_SINGLETON(CProfileManager)

private:
	//맵이라는 자료구조를 사용할거다 <키값: string, 벨류값 : FCollisionProfile> 형식으로 저장할것이다. 
	unordered_map<string, FCollisionProfile*> mProfileMap;

public:
	bool Init();
	/*
	string Name;
	//현재 프로파일이 사용하는 충돌 채널 
	ECollisionChannel::Type Channel = ECollisionChannel::Default;
	bool Enable = true;
	ECollisionInteraction::Type Interaction[ECollisionChannel::End] ;
		
	*/
	//프로파일 생성 
	bool CreateProfile(const string& Name, ECollisionChannel::Type Channel, bool Enable, ECollisionInteraction::Type Interaction);
	//원하는 프로파일의 상호작용할 채널의 충돌 여부를 변경해준다. 
	bool SetInteraction(const string& Name, ECollisionChannel::Type Channel, ECollisionInteraction::Type Interaction);
	//충돌여부 
	bool SetEnable(const string& Name, bool Enable);




	FCollisionProfile* FindProfile(const string& Name);


};

