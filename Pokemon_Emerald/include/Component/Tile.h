#pragma once

#include "../Gameinfo.h"

//자료구조의 노드와 같은 존재이다. 
class CTile
{
	friend class CTileMapComponent;

private:
	CTile();
	~CTile();

private:
	ETileType mType = ETileType::Nomal;
	FVector2D mPos;
	FVector2D mSize;
	FVector2D mCenter;
	//타일은 스프라이트 이미지를 사용할거기에 텍스쳐 프레임
	int mTextureFrame = -1;

public:
	ETileType GetTileType() const
	{
		return mType;
	}

	FVector2D GetPos() const
	{
		return mPos;
	}

	FVector2D GetSize() const
	{
		return mSize;
	}

	FVector2D GetCenter() const
	{
		return mCenter;
	}

	int GetTextureFrame() const
	{
		return mTextureFrame;
	}

public:
	void SetTileType(ETileType Type)
	{
		mType = Type;
	}

	void SetTextureFrame(int Frame)
	{
		mTextureFrame = Frame;
	}


public:
	void Save(FILE* File);
	void Load(FILE* File);

};

