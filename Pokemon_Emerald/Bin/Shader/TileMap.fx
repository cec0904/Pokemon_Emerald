#include "Share.fx"

//정점정보
// 정점위치, UV좌표
struct VS_Input_Tex
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_Output_Tex
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

cbuffer TileMap : register(b3)
{
    	//LT Left-Top
    float2 gTileFrameStart; //8
	//RB Right-Bottom
    float2 gTileFrameEnd; //8
}


VS_Output_Tex TileMapVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;

    float3 Pos = input.Pos - gPivot;

    output.Pos = mul(float4(Pos, 1.f), gmatWVP);
    output.UV = input.UV;

    return output;
}

PS_Output_Single TileMapPS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;

    float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);

    output.Color = Color;

    return output;
}


//타일
VS_Output_Tex TileVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;

    float3 Pos = input.Pos - gPivot;

    output.Pos = mul(float4(Pos, 1.f), gmatWVP);

    if (input.UV.x == 0.f) //좌
    {
        output.UV.x = gTileFrameStart.x;
    }
    else //우
    {
        output.UV.x = gTileFrameEnd.x;
    }

    if (input.UV.y == 0.f) //위
    {
        output.UV.y = gTileFrameStart.y;
    }
    else //아래
    {
        output.UV.y = gTileFrameEnd.y;
    }

    return output;
}

PS_Output_Single TilePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;

    float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);

    output.Color = Color;

    return output;
}
