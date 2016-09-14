#include "stdafx.h"
#include "TLRenderDeviceUsedBuffer_D3D11.h"
namespace TLunaEngine
{
	TLRenderDeviceUsedBuffer_D3D11::TLRenderDeviceUsedBuffer_D3D11():
		TLIRenderDeviceUsedBuffer(),TLIRenderDeviceUsedResource_D3D11(),mD3DBuffer(NULL)
	{
	}

	TLRenderDeviceUsedBuffer_D3D11::~TLRenderDeviceUsedBuffer_D3D11()
	{
		if(mD3DBuffer)
		{
			mD3DBuffer->Release();
			mD3DBuffer = NULL;
		}
	}

	bool TLRenderDeviceUsedBuffer_D3D11::initWithD3DBuffer(ID3D11Buffer* d3dBuffer)
	{
		if(mD3DBuffer)
			return false;
		if(!d3dBuffer)
			return false;
		mD3DBuffer = d3dBuffer;
		return true;
	}
}