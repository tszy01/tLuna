#include "TLRenderDeviceCompiledShader_D3D11.h"
#include "TSTxtFileReader.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice.h"
#include <d3dcompiler.h>

namespace TLunaEngine
{
	RenderDeviceCompiledShader_D3D11::RenderDeviceCompiledShader_D3D11():RenderDeviceCompiledShader(),mD3DBlob(TSun::TNULL)
	{
	}

	RenderDeviceCompiledShader_D3D11::~RenderDeviceCompiledShader_D3D11()
	{
		if(mD3DBlob)
		{
			mD3DBlob->Release();
			mD3DBlob = TSun::TNULL;
		}
	}

	TSun::TVOID* RenderDeviceCompiledShader_D3D11::getBufferPointer()
	{
		if(mD3DBlob)
		{
			return mD3DBlob->GetBufferPointer();
		}
		return TSun::TNULL;
	}

	TSun::TSIZE RenderDeviceCompiledShader_D3D11::getBufferSize()
	{
		if(mD3DBlob)
		{
			return mD3DBlob->GetBufferSize();
		}
		return 0;
	}

	TSun::TBOOL RenderDeviceCompiledShader_D3D11::readFromFile(const TSun::TCHAR* file)
	{
		if(!file)
			return TSun::TFALSE;
		if(mD3DBlob)
			return TSun::TFALSE;
		TSun::TVOID* buffer = 0;
		TSun::TU64 readSize = 0;
		if(!TSun::TxtFileReader::ReadAllFile(file,"rb",&buffer,&readSize))
			return TSun::TFALSE;
		if(!buffer || readSize<=0)
			return TSun::TFALSE;
		if(FAILED(D3DCreateBlob((TSun::TSIZE)readSize,&mD3DBlob)))
		{
			TSun::getBlockMemAllocator()->freeMem(buffer, __FILE__, __LINE__);
			return TSun::TFALSE;
		}
		memcpy(mD3DBlob->GetBufferPointer(),buffer,(TSun::TSIZE)readSize);
		TSun::getBlockMemAllocator()->freeMem(buffer, __FILE__, __LINE__);
		return TSun::TTRUE;
	}

	TSun::TBOOL RenderDeviceCompiledShader_D3D11::writeToFile(const TSun::TCHAR* file)
	{
		if(!file)
			return TSun::TFALSE;
		if(!mD3DBlob)
			return TSun::TFALSE;
		if(mD3DBlob->GetBufferSize() == 0)
			return TSun::TFALSE;
		FILE* stream = 0;
		TSun::TS32 re = fopen_s(&stream,file,"wb");
		if(re!=0)
		{
			return TSun::TFALSE;
		}
		TSun::TSIZE writeSize = (TSun::TSIZE)fwrite(mD3DBlob->GetBufferPointer(),1,mD3DBlob->GetBufferSize(),stream);
		if(writeSize!=(TSun::TSIZE)mD3DBlob->GetBufferSize())
		{
			fclose(stream);
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL RenderDeviceCompiledShader_D3D11::compileShader(const TSun::TCHAR* szFile, const TSun::TCHAR* szEntry, const TSun::TCHAR* szShaderType)
	{
		if(!szFile || !szEntry || !szShaderType)
			return TSun::TFALSE;
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		if(!pDevice)
			return TSun::TFALSE;
		TSun::String strModelVersion = pDevice->getShaderProfileVersion();
		if(strModelVersion=="")
			return TSun::TFALSE;
		TSun::String strShaderModel = TSun::String(szShaderType) + TSun::String("_") + strModelVersion;

		HRESULT hr = S_OK;
		TSun::TU32 dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob;
		TSun::TU32 strCount = (TSun::TU32)strlen(szFile);
		TSun::TWCHAR* wStr = T_NEW_ARRAY(TSun::getStringMemAllocator(), TSun::TWCHAR, strCount + 1);
		//mbstowcs(wStr,szFile,strCount);
		::MultiByteToWideChar(CP_ACP, 0, szFile, strCount, wStr, strCount);
		wStr[strCount] = L'\0';
		hr = D3DCompileFromFile( wStr, TSun::TNULL, TSun::TNULL, szEntry, strShaderModel.GetString(), dwShaderFlags, 0, &mD3DBlob, &pErrorBlob );
		if( FAILED(hr) )
		{
			if( pErrorBlob != TSun::TNULL )
				OutputDebugStringA( (TSun::TCHAR*)pErrorBlob->GetBufferPointer() );
			if( pErrorBlob ) pErrorBlob->Release();
			T_DELETE_ARRAY(TSun::getStringMemAllocator(), TSun::TWCHAR, wStr);
			return TSun::TFALSE;
		}
		if( pErrorBlob ) pErrorBlob->Release();
		T_DELETE_ARRAY(TSun::getStringMemAllocator(), TSun::TWCHAR, wStr);
		return TSun::TTRUE;
	}
}