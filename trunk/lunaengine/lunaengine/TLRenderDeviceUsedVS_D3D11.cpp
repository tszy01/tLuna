#include "TLRenderDeviceUsedVS_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedVS_D3D11::RenderDeviceUsedVS_D3D11():RenderDeviceUsedVS(),mD3DVS(0)
	{
	}

	RenderDeviceUsedVS_D3D11::~RenderDeviceUsedVS_D3D11()
	{
		if(mD3DVS)
		{
			mD3DVS->Release();
			mD3DVS = 0;
		}
	}

	TSun::TBOOL RenderDeviceUsedVS_D3D11::initWithD3DVS(ID3D11VertexShader* d3dShader)
	{
		if(mD3DVS)
			return TSun::TFALSE;
		if(!d3dShader)
			return TSun::TFALSE;
		mD3DVS = d3dShader;
		return TSun::TTRUE;
	}
}