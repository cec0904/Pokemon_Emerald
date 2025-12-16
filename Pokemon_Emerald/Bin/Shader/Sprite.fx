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

cbuffer Sprite : register(b3)
{
    float4 gSpriteTint;
    int gTimeUse; // 0이면 사용안함 그외 사용함
    float gmsTime; //밀리 초 가져가기
    float2 gEmpty;
}


VS_Output_Tex SpriteVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;

    float3 Pos = input.Pos - gPivot;

    //반전여부
    if (gAnim2DFlip)
    {
        Pos.x = -Pos.x;
    }

    output.Pos = mul(float4(Pos, 1.f), gmatWVP);

    output.UV = UpdateAnimation2D(input.UV);

    return output;
}

PS_Output_Single SpritePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;

    float2 UV = input.UV;

    if (gTimeUse)
    {
        UV.x = input.UV.x + gmsTime;
    }
    

    //float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);
    float4 Color = gBaseTexture.Sample(gBaseSampler, UV);

    output.Color = Color * gSpriteTint;

    return output;
}