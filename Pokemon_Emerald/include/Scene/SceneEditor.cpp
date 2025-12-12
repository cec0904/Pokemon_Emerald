#include "SceneEditor.h"
#include "../Object/EditorPlayer.h"
#include "../Object/TileMapObj.h"
#include "Input.h"
#include "../GameManager.h"

CSceneEditor::CSceneEditor()
{
}

CSceneEditor::~CSceneEditor()
{
}

bool CSceneEditor::Init()
{
	CScene::Init();

	CEditorPlayer* EidtorPlayer = CreateObj<CEditorPlayer>("EditorPlayer");

	mTileMapObj = CreateObj<CTileMapObj>("TileMap");

	//입력
	mInput->AddBindKey("EditorMode", '1');
	mInput->AddBindFunction("EditorMode", EInputType::Down, this, &CSceneEditor::EditorMode);

	mInput->AddBindKey("TileType", '2');
	mInput->AddBindFunction("TileType", EInputType::Down, this, &CSceneEditor::TileTypeKey);

	mInput->AddBindKey("TileFrame", '3');
	mInput->AddBindFunction("TileFrame", EInputType::Down, this, &CSceneEditor::TileFrameKey);


	mInput->AddBindKey("Save", 'S');
	mInput->ChangeKeyCtrl("Save", true);
	mInput->AddBindFunction("Save", EInputType::Down, this, &CSceneEditor::SaveKey);

	mInput->AddBindKey("Load", 'O');
	mInput->ChangeKeyCtrl("Load", true);
	mInput->AddBindFunction("Load", EInputType::Down, this, &CSceneEditor::LoadKey);


	return true;
}

void CSceneEditor::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);
}

void CSceneEditor::EditorMode(float DeltaTime)
{
	mEditorMode = static_cast<EEditorMode>((int)mEditorMode + 1);

	if (mEditorMode == EEditorMode::End)
	{
		mEditorMode = EEditorMode::TileType;
	}

	mTileMapObj->SetEditorMod(mEditorMode);

}

void CSceneEditor::TileTypeKey(float DeltaTime)
{
	mTileMapObj->AddTileType();
}

void CSceneEditor::TileFrameKey(float DeltaTime)
{
	mTileMapObj->AddTileFrame();
}

void CSceneEditor::SaveKey(float DeltaTime)
{
	OPENFILENAME ofn = {};

	//경로 만들기
	TCHAR FullPath[MAX_PATH] = {};
	TCHAR Filter [] = TEXT("*.tlm\0");

	TCHAR DefaultPath[MAX_PATH] = {};

	lstrcpy(DefaultPath, gRootPath);
	lstrcat(DefaultPath, TEXT("Asset\\Data\\"));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CGameManager::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = Filter;
	ofn.lpstrDefExt = TEXT("tlm");
	ofn.lpstrInitialDir = DefaultPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = FullPath;

	if (GetSaveFileName(&ofn) !=0)
	{
		//저장을 시작한다.
		mTileMapObj->Save(FullPath);
	}
}

void CSceneEditor::LoadKey(float DeltaTime)
{
	OPENFILENAME ofn = {};

	//경로 만들기
	TCHAR FullPath[MAX_PATH] = {};
	TCHAR Filter[] = TEXT("*.tlm\0");

	TCHAR DefaultPath[MAX_PATH] = {};

	lstrcpy(DefaultPath, gRootPath);
	lstrcat(DefaultPath, TEXT("Asset\\Data\\"));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CGameManager::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = Filter;
	ofn.lpstrDefExt = TEXT("tlm");
	ofn.lpstrInitialDir = DefaultPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = FullPath;

	if (GetOpenFileName(&ofn) != 0)
	{
		//불러오기 시작한다. 
		mTileMapObj->Load(FullPath);
	}
}
