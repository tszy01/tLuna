#ifndef _TLRENDERDEVICEUSEDBUFFER_D3D11_H_
#define _TLRENDERDEVICEUSEDBUFFER_D3D11_H_

#include "TLIRenderDeviceUsedBuffer.h"
#include "TLIRenderDeviceUsedResource_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	// buffer class d3d11
	class TLRenderDeviceUsedBuffer_D3D11 : public TLIRenderDeviceUsedBuffer, public TLIRenderDeviceUsedResource_D3D11
	{
	public:
		TLRenderDeviceUsedBuffer_D3D11();
		virtual ~TLRenderDeviceUsedBuffer_D3D11();
	public:
		// get d3d resource ptr
		virtual ID3D11Resource* getD3DResource()
		{
			return (ID3D11Resource*)mD3DBuffer;
		}
	private:
		ID3D11Buffer* mD3DBuffer;
	public:
		// get d3d11 buffer
		ID3D11Buffer* getD3DBuffer()
		{
			return mD3DBuffer;
		}
		// init with d3d buffer
		bool initWithD3DBuffer(ID3D11Buffer* d3dBuffer);
	};
}

#endif