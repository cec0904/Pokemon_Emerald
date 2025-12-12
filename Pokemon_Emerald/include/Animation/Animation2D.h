#pragma once

#include "../Gameinfo.h"
#include "Animation2DSequence.h"
#include "../Shader/Animation2DCBuffer.h"

class CAnimation2D
{
	friend class CSpriteComponent;

private:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& Anim);
	virtual ~CAnimation2D();

	//상수버퍼
private:
	static CAnimation2DCBuffer* mAnimCBuffer;

public:
	static void CreateCBuffer();
	static void DestroyCBuffer();
	static void DisableAnimation();
	static void SetAnimFlip(bool Flip);

private:
	//현재 씬
	class CScene* mScene = nullptr;
	//주인 SpriteComponent
	class CSpriteComponent* mOwner = nullptr;
	//애니메이션 시퀀스 맵 
	unordered_map<string, CAnimation2DSequence*> mSequenceMap;
	//현재 재생중인 시퀀스
	CAnimation2DSequence* mCurrentSequence = nullptr;

public:
	bool Init();
	void Update(float DeltaTime);

private:
	CAnimation2DSequence* FindSequence(const string& Name);

public:

	// 시퀀스 추가
	bool AddSequence(const string& Name, float PlayTime = 1.f, float PlayRate = 1.f, bool Loop = false, bool Reverse = false);

	bool AddSequence(class CAnimation2DData* Asset, float PlayTime = 1.f, float PlayRate = 1.f, bool Loop = false, bool Reverse = false);

	void ResetFrame();

	//Setter
	void SetPlayTime(const string& Name, float PlayTime);
	void SetPlayRate(const string& Name, float PlayRate);
	void SetLoop(const string& Name, bool Loop);
	void SetReverse(const string& Name, bool Reverse);

	//애니메이션 변경
	void ChangeAnimation(const string& Name);

	CAnimation2D* Clone();
	void SetShader();

public:
	//노티파이
	template<typename T>
	void SetEndFunction(const string& Name, T* Obj, void(T::* Func)())
	{
		CAnimation2DSequence* Sequence = FindSequence(Name);

		if (!Sequence)
		{
			return;
		}

		Sequence->SetEndFunction<T>(Obj, Func);
	}

	//노티파이 바인드 
	template<typename T>
	void AddNotify(const string& Name, int Frame, T* Obj, void(T::* Func)())
	{
		CAnimation2DSequence* Sequence = FindSequence(Name);

		if (!Sequence)
		{
			return;
		}

		Sequence->AddNotify<T>(Name, Frame, Obj, Func);
	}

};

