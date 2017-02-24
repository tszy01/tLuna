#ifndef _TLRENDERDEVICECOMPILEDSHADER_H_
#define _TLRENDERDEVICECOMPILEDSHADER_H_

#include "TSCommonTypeDef.h"
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
		virtual TSun::TVOID* getBufferPointer() = 0; 
		// get buffer size
		virtual TSun::TSIZE getBufferSize() = 0;
		// read from file
		virtual TSun::TBOOL readFromFile(const TSun::TCHAR* file) = 0;
		// write to file
		virtual TSun::TBOOL writeToFile(const TSun::TCHAR* file) = 0;
		// compile shader
		virtual TSun::TBOOL compileShader(const TSun::TCHAR* szFile, const TSun::TCHAR* szEntry, const TSun::TCHAR* szShaderType) = 0;
	};
}

#endif