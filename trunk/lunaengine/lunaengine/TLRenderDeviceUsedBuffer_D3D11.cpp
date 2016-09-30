#include "TLRenderDeviceUsedBuffer_D3D11.h"
namespace TLunaEngine
{
	RenderDeviceUsedBuffer_D3D11::RenderDeviceUsedBuffer_D3D11():
		RenderDeviceUsedBuffer(),RenderDeviceUsedResource_D3D11(),mD3DBuffer(TNULL)
	{
	}

	RenderDeviceUsedBuffer_D3D11::~RenderDeviceUsedBuffer_D3D11()
	{
		if(mD3DBuffer)
		{
			mD3DBuffer->Release();
			mD3DBuffer = TNULL;
		}
	}

	TBOOL RenderDeviceUsedBuffer_D3D11::initWithD3DBuffer(ID3D11Buffer* d3dBuffer)
	{
		if(mD3DBuffer)
			return TFALSE;
		if(!d3dBuffer)
			return TFALSE;
		mD3DBuffer = d3dBuffer;
		return TTRUE;
	}
}