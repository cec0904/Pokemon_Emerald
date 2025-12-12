#include "Tile.h"

CTile::CTile()
{
}

CTile::~CTile()
{
}

void CTile::Save(FILE* File)
{
	fwrite(&mType, sizeof(ETileType), 1, File);
	fwrite(&mPos, sizeof(FVector2D), 1, File);
	fwrite(&mSize, sizeof(FVector2D), 1, File);
	fwrite(&mCenter, sizeof(FVector2D), 1, File);
	fwrite(&mTextureFrame, sizeof(int), 1, File);
}

void CTile::Load(FILE* File)
{
	fread(&mType, sizeof(ETileType), 1, File);
	fread(&mPos, sizeof(FVector2D), 1, File);
	fread(&mSize, sizeof(FVector2D), 1, File);
	fread(&mCenter, sizeof(FVector2D), 1, File);
	fread(&mTextureFrame, sizeof(int), 1, File);
}
