#include "TLRenderDeviceCompiledShader_D3D11.h"
#include "TLTxtFileReader.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice.h"
#include <d3dcompiler.h>

namespace TLunaEngine
{
	RenderDeviceCompiledShader_D3D11::RenderDeviceCompiledShader_D3D11():RenderDeviceCompiledShader(),mD3DBlob(TNULL)
	{
	}

	RenderDeviceCompiledShader_D3D11::~RenderDeviceCompiledShader_D3D11()
	{
		if(mD3DBlob)
		{
			mD3DBlob->Release();
			mD3DBlob = TNULL;
		}
	}

	void* RenderDeviceCompiledShader_D3D11::getBufferPointer()
	{
		if(mD3DBlob)
		{
			return mD3DBlob->GetBufferPointer();
		}
		return TNULL;
	}

	size_t RenderDeviceCompiledShader_D3D11::getBufferSize()
	{
		if(mD3DBlob)
		{
			return mD3DBlob->GetBufferSize();
		}
		return 0;
	}

	bool RenderDeviceCompiledShader_D3D11::readFromFile(const char* file)
	{
		if(!file)
			return false;
		if(mD3DBlob)
			return false;
		void* buffer = 0;
		size_t readSize = 0;
		if(!TxtFileReader::ReadAllFile(file,"rb",&buffer,&readSize))
			return false;
		if(!buffer || readSize<=0)
			return false;
		if(FAILED(D3DCreateBlob(readSize,&mD3DBlob)))
		{
			free(buffer);
			return false;
		}
		memcpy(mD3DBlob->GetBufferPointer(),buffer,readSize);
		free(buffer);
		return true;
	}

	bool RenderDeviceCompiledShader_D3D11::writeToFile(const char* file)
	{
		if(!file)
			return false;
		if(!mD3DBlob)
			return false;
		if(mD3DBlob->GetBufferSize() == 0)
			return false;
		FILE* stream = 0;
		int re = fopen_s(&stream,file,"wb");
		if(re!=0)
		{
			return false;
		}
		size_t writeSize = fwrite(mD3DBlob->GetBufferPointer(),1,mD3DBlob->GetBufferSize(),stream);
		if(writeSize!=mD3DBlob->GetBufferSize())
		{
			fclose(stream);
			return false;
		}
		return true;
	}

	bool RenderDeviceCompiledShader_D3D11::compileShader(const char* szFile, const char* szEntry, const char* szShaderType)
	{
		if(!szFile || !szEntry || !szShaderType)
			return false;
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		if(!pDevice)
			return false;
		String strModelVersion = pDevice->getShaderProfileVersion();
		if(strModelVersion=="")
			return false;
		String strShaderModel = String(szShaderType) + String("_") + strModelVersion;

		HRESULT hr = S_OK;
		TU32 dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob;
		size_t strCount = strlen(szFile);
		wchar_t* wStr = new wchar_t[strCount+1];
		mbstowcs(wStr,szFile,strCount);
		wStr[strCount] = L'\0';
		hr = D3DCompileFromFile( wStr, TNULL, TNULL, szEntry, strShaderModel.GetString(), dwShaderFlags, 0, &mD3DBlob, &pErrorBlob );
		if( FAILED(hr) )
		{
			if( pErrorBlob != TNULL )
				OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			if( pErrorBlob ) pErrorBlob->Release();
			delete [] wStr;
			return false;
		}
		if( pErrorBlob ) pErrorBlob->Release();
		delete [] wStr;
		return true;
	}
}