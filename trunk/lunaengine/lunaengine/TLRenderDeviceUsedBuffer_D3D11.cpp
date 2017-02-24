#include "TLRenderDeviceUsedBuffer_D3D11.h"
namespace TLunaEngine
{
	RenderDeviceUsedBuffer_D3D11::RenderDeviceUsedBuffer_D3D11():
		RenderDeviceUsedBuffer(),RenderDeviceUsedResource_D3D11(),mD3DBuffer(TSun::TNULL)
	{
	}

	RenderDeviceUsedBuffer_D3D11::~RenderDeviceUsedBuffer_D3D11()
	{
		if(mD3DBuffer)
		{
			mD3DBuffer->Release();
			mD3DBuffer = TSun::TNULL;
		}
	}

	TSun::TBOOL RenderDeviceUsedBuffer_D3D11::initWithD3DBuffer(ID3D11Buffer* d3dBuffer)
	{
		if(mD3DBuffer)
			return TSun::TFALSE;
		if(!d3dBuffer)
			return TSun::TFALSE;
		mD3DBuffer = d3dBuffer;
		return TSun::TTRUE;
	}
}