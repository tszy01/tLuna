#ifndef _TLRENDERDEVICECOMPILEDSHADER_D3D11_H_
#define _TLRENDERDEVICECOMPILEDSHADER_D3D11_H_

#include "TLIRenderDeviceCompiledShader.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class TLRenderDeviceCompiledShader_D3D11 : public TLIRenderDeviceCompiledShader
	{
	public:
		TLRenderDeviceCompiledShader_D3D11();
		virtual ~TLRenderDeviceCompiledShader_D3D11();
	public:
		// get buffer pointer
		virtual void* getBufferPointer(); 
		// get buffer size
		virtual size_t getBufferSize();
		// read from file
		virtual bool readFromFile(const char* file);
		// write to file
		virtual bool writeToFile(const char* file);
		// compile shader
		virtual bool compileShader(const char* szFile, const char* szEntry, const char* szShaderType);
	private:
		ID3DBlob* mD3DBlob;
	};
}

#endif