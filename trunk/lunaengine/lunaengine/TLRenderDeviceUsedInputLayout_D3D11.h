#ifndef _TLRENDERDEVICEUSEDINPUTLAYOUT_D3D11_H_
#define _TLRENDERDEVICEUSEDINPUTLAYOUT_D3D11_H_

#include "TLRenderDeviceUsedInputLayout.h"
#include <d3d11.h>
namespace TLunaEngine
{
	class RenderDeviceUsedInputLayout_D3D11 : public RenderDeviceUsedInputLayout
	{
	public:
		RenderDeviceUsedInputLayout_D3D11();
		virtual ~RenderDeviceUsedInputLayout_D3D11();
	private:
		ID3D11InputLayout* mD3DInputLayout;
	public:
		// get d3d input layout ptr
		ID3D11InputLayout* getD3DInputLayout()
		{
			return mD3DInputLayout;
		}
		// init with d3d input layout
		bool initWithD3DInputLayout(ID3D11InputLayout* d3dInputLayout);
	};
}

#endif