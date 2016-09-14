#ifndef _TLIRENDERDEVICECOMPILEDSHADER_H_
#define _TLIRENDERDEVICECOMPILEDSHADER_H_

namespace TLunaEngine
{
	// Shader
	class TLIRenderDeviceCompiledShader
	{
	public:
		TLIRenderDeviceCompiledShader(){};
		virtual ~TLIRenderDeviceCompiledShader(){};
	public:
		// get buffer pointer
		virtual void* getBufferPointer() = 0; 
		// get buffer size
		virtual size_t getBufferSize() = 0;
		// read from file
		virtual bool readFromFile(const char* file) = 0;
		// write to file
		virtual bool writeToFile(const char* file) = 0;
		// compile shader
		virtual bool compileShader(const char* szFile, const char* szEntry, const char* szShaderType) = 0;
	};
}

#endif