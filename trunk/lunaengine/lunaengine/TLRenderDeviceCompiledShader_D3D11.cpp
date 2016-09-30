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

	TVOID* RenderDeviceCompiledShader_D3D11::getBufferPointer()
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

	TBOOL RenderDeviceCompiledShader_D3D11::readFromFile(const TCHAR* file)
	{
		if(!file)
			return TFALSE;
		if(mD3DBlob)
			return TFALSE;
		TVOID* buffer = 0;
		size_t readSize = 0;
		if(!TxtFileReader::ReadAllFile(file,"rb",&buffer,&readSize))
			return TFALSE;
		if(!buffer || readSize<=0)
			return TFALSE;
		if(FAILED(D3DCreateBlob(readSize,&mD3DBlob)))
		{
			free(buffer);
			return TFALSE;
		}
		memcpy(mD3DBlob->GetBufferPointer(),buffer,readSize);
		free(buffer);
		return TTRUE;
	}

	TBOOL RenderDeviceCompiledShader_D3D11::writeToFile(const TCHAR* file)
	{
		if(!file)
			return TFALSE;
		if(!mD3DBlob)
			return TFALSE;
		if(mD3DBlob->GetBufferSize() == 0)
			return TFALSE;
		FILE* stream = 0;
		int re = fopen_s(&stream,file,"wb");
		if(re!=0)
		{
			return TFALSE;
		}
		size_t writeSize = fwrite(mD3DBlob->GetBufferPointer(),1,mD3DBlob->GetBufferSize(),stream);
		if(writeSize!=mD3DBlob->GetBufferSize())
		{
			fclose(stream);
			return TFALSE;
		}
		return TTRUE;
	}

	TBOOL RenderDeviceCompiledShader_D3D11::compileShader(const TCHAR* szFile, const TCHAR* szEntry, const TCHAR* szShaderType)
	{
		if(!szFile || !szEntry || !szShaderType)
			return TFALSE;
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		if(!pDevice)
			return TFALSE;
		String strModelVersion = pDevice->getShaderProfileVersion();
		if(strModelVersion=="")
			return TFALSE;
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
		TWCHAR* wStr = new TWCHAR[strCount+1];
		mbstowcs(wStr,szFile,strCount);
		wStr[strCount] = L'\0';
		hr = D3DCompileFromFile( wStr, TNULL, TNULL, szEntry, strShaderModel.GetString(), dwShaderFlags, 0, &mD3DBlob, &pErrorBlob );
		if( FAILED(hr) )
		{
			if( pErrorBlob != TNULL )
				OutputDebugStringA( (TCHAR*)pErrorBlob->GetBufferPointer() );
			if( pErrorBlob ) pErrorBlob->Release();
			delete [] wStr;
			return TFALSE;
		}
		if( pErrorBlob ) pErrorBlob->Release();
		delete [] wStr;
		return TTRUE;
	}
}