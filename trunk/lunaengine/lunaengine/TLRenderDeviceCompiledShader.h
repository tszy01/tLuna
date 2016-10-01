#ifndef _TLRENDERDEVICECOMPILEDSHADER_H_
#define _TLRENDERDEVICECOMPILEDSHADER_H_

#include "TLCommonTypeDef.h"
namespace TLunaEngine
{
	// Shader
	class RenderDeviceCompiledShader
	{
	public:
		RenderDeviceCompiledShader(){};
		virtual ~RenderDeviceCompiledShader(){};
	public:
		// get buffer pointer
		virtual TVOID* getBufferPointer() = 0; 
		// get buffer size
		virtual TSIZE getBufferSize() = 0;
		// read from file
		virtual TBOOL readFromFile(const TCHAR* file) = 0;
		// write to file
		virtual TBOOL writeToFile(const TCHAR* file) = 0;
		// compile shader
		virtual TBOOL compileShader(const TCHAR* szFile, const TCHAR* szEntry, const TCHAR* szShaderType) = 0;
	};
}

#endif