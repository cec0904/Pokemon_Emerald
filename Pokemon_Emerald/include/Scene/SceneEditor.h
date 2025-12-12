#pragma once
#include "Scene.h"
class CSceneEditor :
    public CScene
{
	friend class CSceneManager;

protected:
	CSceneEditor();
	virtual ~CSceneEditor();

protected:
	CSharedPtr<class CTileMapObj> mTileMapObj;
	EEditorMode mEditorMode = EEditorMode::TileType;



public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void EditorMode(float DeltaTime);
	void TileTypeKey(float DeltaTime);
	void TileFrameKey(float DeltaTime);

	// 세이브 로드
	void SaveKey(float DeltaTime);
	void LoadKey(float DeltaTime);

};

