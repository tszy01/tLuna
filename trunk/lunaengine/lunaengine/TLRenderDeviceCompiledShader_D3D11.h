#ifndef _TLRENDERDEVICECOMPILEDSHADER_D3D11_H_
#define _TLRENDERDEVICECOMPILEDSHADER_D3D11_H_

#include "TLRenderDeviceCompiledShader.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceCompiledShader_D3D11 : public RenderDeviceCompiledShader
	{
	public:
		RenderDeviceCompiledShader_D3D11();
		virtual ~RenderDeviceCompiledShader_D3D11();
	public:
		// get buffer pointer
		virtual TSun::TVOID* getBufferPointer(); 
		// get buffer size
		virtual TSun::TSIZE getBufferSize();
		// read from file
		virtual TSun::TBOOL readFromFile(const TSun::TCHAR* file);
		// write to file
		virtual TSun::TBOOL writeToFile(const TSun::TCHAR* file);
		// compile shader
		virtual TSun::TBOOL compileShader(const TSun::TCHAR* szFile, const TSun::TCHAR* szEntry, const TSun::TCHAR* szShaderType);
	private:
		ID3DBlob* mD3DBlob;
	};
}

#endif