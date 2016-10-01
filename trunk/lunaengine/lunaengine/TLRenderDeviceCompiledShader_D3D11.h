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
		virtual TVOID* getBufferPointer(); 
		// get buffer size
		virtual TSIZE getBufferSize();
		// read from file
		virtual TBOOL readFromFile(const TCHAR* file);
		// write to file
		virtual TBOOL writeToFile(const TCHAR* file);
		// compile shader
		virtual TBOOL compileShader(const TCHAR* szFile, const TCHAR* szEntry, const TCHAR* szShaderType);
	private:
		ID3DBlob* mD3DBlob;
	};
}

#endif