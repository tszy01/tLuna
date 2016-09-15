#include "TLRenderDeviceUsedDS_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedDS_D3D11::RenderDeviceUsedDS_D3D11():RenderDeviceUsedDS(),mD3DDS(0)
	{
	}

	RenderDeviceUsedDS_D3D11::~RenderDeviceUsedDS_D3D11()
	{
		if(mD3DDS)
		{
			mD3DDS->Release();
			mD3DDS = 0;
		}
	}

	bool RenderDeviceUsedDS_D3D11::initWithD3DDS(ID3D11DomainShader* d3dShader)
	{
		if(mD3DDS)
			return false;
		if(!d3dShader)
			return false;
		mD3DDS = d3dShader;
		return true;
	}
}