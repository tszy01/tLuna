//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

cbuffer cbNeverChanges
{
    matrix View;
};

cbuffer cbChangeOnResize
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame
{
    matrix World;
    float4 vMeshColor;
    float4x4 transMatrix;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

struct GS_INPUT
{
		float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex;
    
    return output;
}

GS_INPUT AddVS( VS_INPUT input )
{
	  GS_INPUT output = (GS_INPUT)0;
	  output.Tex = input.Tex;
	  output.Pos = mul( input.Pos, transMatrix );
	  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    //return txDiffuse.Sample( samLinear, input.Tex ) * vMeshColor;
    return txDiffuse.Sample( samLinear, input.Tex );
}


//--------------------------------------------------------------------------------------
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

technique10 Cal
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, AddVS() ) );
        SetGeometryShader(ConstructGSWithSO(CompileShader(vs_4_0, AddVS()),"POSITION.xyz;TEXCOORD.xy"));
        //SetGeometryShader( NULL );
        SetPixelShader( NULL );
        SetDepthStencilState( DisableDepth, 0 );
    }
}
