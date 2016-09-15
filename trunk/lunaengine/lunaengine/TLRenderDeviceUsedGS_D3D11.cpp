#include "TLRenderDeviceUsedGS_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedGS_D3D11::RenderDeviceUsedGS_D3D11():RenderDeviceUsedGS(),mD3DGS(0)
	{
	}

	RenderDeviceUsedGS_D3D11::~RenderDeviceUsedGS_D3D11()
	{
		if(mD3DGS)
		{
			mD3DGS->Release();
			mD3DGS = 0;
		}
	}

	bool RenderDeviceUsedGS_D3D11::initWithD3DGS(ID3D11GeometryShader* d3dShader)
	{
		if(mD3DGS)
			return false;
		if(!d3dShader)
			return false;
		mD3DGS = d3dShader;
		return true;
	}
}