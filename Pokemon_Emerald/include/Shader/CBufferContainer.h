#pragma once

#include "../Vector2D.h"
#include "../Vector3D.h"
#include "../Vector4D.h"
#include "../Matrix.h"

//레지스터 0
struct FTransformCBufferInfo
{
	FMatrix matWorld;
	FMatrix matView;
	FMatrix matProj;
	FMatrix matWV;
	FMatrix matWVP;
	FVector3D Pivot;
	float Empty;
};

//레지스터 1
struct FMaterialCBufferInfo
{
	// 재질 기본 색상
	FVector4D BaseColor;
	// 재질 투명도
	float Opacity;
	// 재질 텍스쳐 가로 크기
	int TextureWidth;
	// 재질 텍스쳐 세로 크기
	int TextureHeight;
	// 16 바이트 맞추기용 더미 하나 추가해준다. 
	float Empty;
};

//애니메이션 정보
struct FAnimation2DBufferInfo
{
	//LT Left-Top
	FVector2D LTUV;	//8
	//RB Right-Bottom
	FVector2D RBUV; //8
	// 애니메이션 사용 여부
	int AnimEnable; // 4
	//반전여부
	int AnimFlip;
	//빈 데이터 16 배수로 보내줘야한다. 
	FVector2D Empty;
};

struct FTileMapCBufferInfo
{
	//LT Left-Top
	FVector2D LTUV;	//8
	//RB Right-Bottom
	FVector2D RBUV; //8
};


//UI
struct FUICBufferInfo
{
	//텍스쳐에 대한 보정 색상 
	FVector4D BrushTint = FVector4D::White;      //16
	//Widget에 대한 보정 색상 
	FVector4D Color = FVector4D::White;     //16
	//LT Left-Top
	FVector2D LTUV; //8
	//RB Right-Bottom
	FVector2D RBUV; //8
	// 애니메이션 사용 여부
	int BrushAnimEnable; // 4
	// 텍스쳐 사용 여부 
	int BrushTextureEnable; // 4
	FVector2D BrushEmpty;     //8
};




//스프라이트용 상수버퍼
//레지스터 2 
struct FSpriteCBufferInfo
{
	FVector4D Tint;
	int TimeUse;	// 0이면 사용안함 그외 사용함
	float msTime;		//밀리 초 가져가기
	FVector2D Empty;
};

//레지스터 3
struct FColliderCBufferInfo
{
	FVector4D Color;
};


