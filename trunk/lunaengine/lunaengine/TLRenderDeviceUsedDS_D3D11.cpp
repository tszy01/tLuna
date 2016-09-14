#include "stdafx.h"
#include "TLRenderDeviceUsedDS_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedDS_D3D11::TLRenderDeviceUsedDS_D3D11():TLIRenderDeviceUsedDS(),mD3DDS(NULL)
	{
	}

	TLRenderDeviceUsedDS_D3D11::~TLRenderDeviceUsedDS_D3D11()
	{
		if(mD3DDS)
		{
			mD3DDS->Release();
			mD3DDS = NULL;
		}
	}

	bool TLRenderDeviceUsedDS_D3D11::initWithD3DDS(ID3D11DomainShader* d3dShader)
	{
		if(mD3DDS)
			return false;
		if(!d3dShader)
			return false;
		mD3DDS = d3dShader;
		return true;
	}
}