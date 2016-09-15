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

	bool RenderDeviceUsedVS_D3D11::initWithD3DVS(ID3D11VertexShader* d3dShader)
	{
		if(mD3DVS)
			return false;
		if(!d3dShader)
			return false;
		mD3DVS = d3dShader;
		return true;
	}
}