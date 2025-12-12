#include "ProfileManager.h"

CProfileManager::CProfileManager()
{

}

CProfileManager::~CProfileManager()
{
	auto iter = mProfileMap.begin();
	auto iterEnd = mProfileMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CProfileManager::Init()
{
	//우리가 사용할 프로파일 등록 
	//프로파일등록 
	CreateProfile("Default", ECollisionChannel::Default, true, ECollisionInteraction::Collision);
	//플레이어 
	CreateProfile("Player", ECollisionChannel::Player, true, ECollisionInteraction::Collision);
	//몬스터 
	CreateProfile("Monster", ECollisionChannel::Monster, true, ECollisionInteraction::Collision);
	//플레이어 공격
	CreateProfile("PlayerAttack", ECollisionChannel::PlayerAttack, true, ECollisionInteraction::Ignore);
	//몬스터 공격 
	CreateProfile("MonsterAttack", ECollisionChannel::MonsterAttack, true, ECollisionInteraction::Ignore);
	// npc 
	CreateProfile("Npc", ECollisionChannel::Npc, true, ECollisionInteraction::Collision);

	//플레이어 프로파일 셋팅 
	/*
		Interaction[Default] = Collision;
		Interaction[Player] = Ignore;
		Interaction[Monster] = Collision;
		Interaction[PlayerAttack] = Ignore;
		Interaction[MonsterAttack] = Collision;
	*/
	SetInteraction("Player", ECollisionChannel::Player, ECollisionInteraction::Ignore);
	SetInteraction("Player", ECollisionChannel::PlayerAttack, ECollisionInteraction::Ignore);

	//PlayerAttack
	SetInteraction("PlayerAttack", ECollisionChannel::Default, ECollisionInteraction::Collision);
	SetInteraction("PlayerAttack", ECollisionChannel::Monster, ECollisionInteraction::Collision);

	//MonsterAttack
	SetInteraction("MonsterAttack", ECollisionChannel::Default, ECollisionInteraction::Collision);
	SetInteraction("MonsterAttack", ECollisionChannel::Player, ECollisionInteraction::Collision);

	//Monster
	SetInteraction("Monster", ECollisionChannel::MonsterAttack, ECollisionInteraction::Ignore);
	SetInteraction("Monster", ECollisionChannel::Monster, ECollisionInteraction::Ignore);

	// Npc
	SetInteraction("Npc", ECollisionChannel::Default, ECollisionInteraction::Collision);
	SetInteraction("Npc", ECollisionChannel::Npc, ECollisionInteraction::Collision);


	return true;
}

bool CProfileManager::CreateProfile(const string& Name, ECollisionChannel::Type Channel, bool Enable, ECollisionInteraction::Type Interaction)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (Profile != nullptr)
	{
		return false;
	}

	Profile = new FCollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->Enable = Enable;

	for (int i = 0; i < ECollisionChannel::End; ++i)
	{
		Profile->Interaction[i] = Interaction;
	}

	mProfileMap.insert(make_pair(Name, Profile));

	return true;
}

bool CProfileManager::SetInteraction(const string& Name, ECollisionChannel::Type Channel, ECollisionInteraction::Type Interaction)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (Profile == nullptr)
	{
		return false;
	}
	Profile->Interaction[Channel] = Interaction;

	return true;
}

bool CProfileManager::SetEnable(const string& Name, bool Enable)
{
	FCollisionProfile* Profile = FindProfile(Name);

	if (Profile == nullptr)
	{
		return false;
	}
	Profile->Enable = Enable;

	return true;
}

FCollisionProfile* CProfileManager::FindProfile(const string& Name)
{
	//auto iter;
	unordered_map<string, FCollisionProfile*>::iterator iter = mProfileMap.find(Name);

	if (iter == mProfileMap.end())
	{
		return nullptr;
	}

	//iter->first;	// 키값
	//iter->second;	// 벨류값
	return iter->second;
}
